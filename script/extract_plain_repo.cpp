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
};

std::string exclude[] = {
        "cmake-build.*",
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

    auto last = fs::recursive_directory_iterator();

    for (auto first = fs::recursive_directory_iterator(p); first != last; ++first) {
        for (auto &i : r) {
            if (std::regex_match(first->path().filename().string(), i)) {
                if (first->is_directory()) {
                    first.disable_recursion_pending();
                } else {
                    goto skip_current_path;
                }
            }
        }
        if (first->exists()
        && first->is_regular_file()
        && checkExtension(first->path())) {
            auto to_path = target / first->path().lexically_relative(root);
            auto parent_path = to_path.parent_path();

            std::clog << "Founded:" << first->path() << std::endl;

            fs::create_directories(parent_path);

            fs::copy_file(first->path(), to_path,
                          fs::copy_options::overwrite_existing
                          );
        }
        if (first->exists() && first->is_directory()) {
            auto include_me_path = first->path() / "PleaseIncludeMe.h";
            std::ofstream include_me(include_me_path, std::ios::trunc);
            include_me << "#pragma once" << std::endl;
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