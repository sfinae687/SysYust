//
// Created by LL06p on 24-7-15.
//
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <filesystem>
#include <algorithm>
#include <format>
#include <vector>
#include <ranges>

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

int main(int argc, char *argv[]) {
    if (argc > 3) {
        std::cerr << "FETAL:Must pass the file2 to compare by command line arguments" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<fs::path> file_to_compare;
    std::vector<std::pair<std::ifstream, std::string>> stream_to_compare;

    std::cout << fs::current_path() << std::endl;
    for (int i=1; i<argc; ++i) {
        file_to_compare.emplace_back(argv[i]);
        stream_to_compare.emplace_back(std::piecewise_construct,
                                       std::tuple{argv[i], std::ios::binary},
                                       std::tuple{});
    }

    int lineno = 0;
    while (ranges::all_of(stream_to_compare, [](auto &i) -> bool {
        auto &[stream, line] = i;
        return std::getline(stream, line).good();
    }) ){
        lineno++;
        if (ranges::any_of(stream_to_compare, [&stream_to_compare](auto &i) {
            return i.second != stream_to_compare.front().second;
        })) {
            std::cerr << "FAIL:the first different line : " << lineno << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    if (ranges::all_of(stream_to_compare, [](auto &i) {
        return i.first.eof();
    })) {
        return 0;
    } else {
        std::cerr << "FAIL:the files have different length" << std::endl;
        std::exit(EXIT_FAILURE);
    }

}