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
#include <vector>
#include <ranges>

#include <fmt/core.h>

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "You must specify just two files to compare";
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Begin compare" << std::endl;

    fs::path file1 = argv[1];
    fs::path file2 = argv[2];
    std::ifstream stream1(file1);
    std::ifstream stream2(file2);

    std::istream_iterator<std::string> last_iter{};
    std::istream_iterator<std::string> token1(stream1);
    std::istream_iterator<std::string> token2(stream2);
    auto [word_on_1, word_on_2] = std::mismatch(token1, last_iter, token2, last_iter);
    if (word_on_1 != last_iter || word_on_2 != last_iter) {
        std::cerr << fmt::format("FAIL:token difference founded") << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return 0;
}