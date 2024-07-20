//
// Created by LL06p on 24-7-16.
//
#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

fs::path path_to_extract[] = {
        "./include",
        "./src",
        "./lib/antlr4-cpp-runtime/runtime/src",
        "./lib/argparse/include",
        "./lib/fmt/include",
        "./lib/fmt/src",
        "./lib/range-v3/include",
        "./script/CommandGenerator.java",
};

std::string exclude[] = {
        "cmake-build.*",
        "fmt.cc"
};

using path_regex = std::basic_regex<fs::path::value_type>;

bool checkExtension(fs::path p) {
    auto ext = p.extension().string();
    if (ext == ".cpp" || ext == ".h" || ext == ".cc" || ext == ".hpp") {
        return true;
    }
    return false;
}

void extract_path(std::vector<path_regex>& r, fs::path p, fs::path root, fs::path target) {

    if (is_regular_file(p)) {
        auto tp = target / p.filename();
        auto parent_path = tp.parent_path();
        fs::create_directories(parent_path);
        fs::copy_file(p, tp, fs::copy_options::overwrite_existing);
        std::clog << "Founded:" << p << std::endl;
        return ;
    }

    auto last = fs::recursive_directory_iterator();

    for (auto first = fs::recursive_directory_iterator(p); first != last; ++first) {

        // 跳过排除目录

        for (auto &i : r) {
            if (std::regex_match(first->path().filename().string(), i)) {
                if (first->is_directory()) {
                    first.disable_recursion_pending();
                } else {
                    goto skip_current_path;
                }
            }
        }

        { // 处理条目
            auto to_path = target / first->path().lexically_relative(root);
            auto parent_path = to_path.parent_path();

            if (first->exists()
                && first->is_regular_file()
                && checkExtension(first->path())) { // 需要拷贝的文件

                std::clog << "Founded:" << first->path() << std::endl;

                fs::create_directories(parent_path);

                fs::copy_file(first->path(), to_path,
                              fs::copy_options::overwrite_existing
                );
            } else if (first->exists() && first->is_directory()) { // 目录，添加一个 PleaseIncludeMe.h 的文件
                fs::create_directories(parent_path);
                auto include_me_path = parent_path / "PleaseIncludeMe.h";
                std::ofstream include_me(include_me_path, std::ios::out | std::ios::trunc);
                if (include_me) {
                    include_me << "#pragma once" << std::endl;
                    include_me.close();
                } else {
                    std::cerr << "unable to open file:" << include_me_path << std::endl;
                }
            }
        }
        skip_current_path:;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Specify the path to extract by command line argument" << std::endl;
    }
    fs::path project_root = argv[1];
    fs::path target_path = argv[2];

    if (exists(target_path)) {
        fs::remove_all(target_path);
    }

    std::vector<path_regex> r;
    for (auto i : exclude) {
        r.emplace_back(i);
    }

    for (auto i : path_to_extract) {
        extract_path(r, project_root/i, project_root, target_path);
    }

}