//
// Created by LL06p on 24-7-16.
//

#include <utility>
#include <filesystem>
#include <ranges>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>

#include "SysYLexer.h"
#include "SysYParser.h"

#include "AST/SyntaxTreeBuilder.h"
#include "fmt/format.h"
#include "utility/Logger.h"
#include "utility/StreamLogger.h"
#include "IREmitter/IREmitter.h"
#include "AST/SyntaxTreeString.h"

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;
namespace IREmitter = SysYust::AST::IREmitter;

using namespace std::literals::string_literals;

int main(int argc, char *argv[]) {
    fs::create_directories("output");
    fs::path output_dir("output");
    fs::path input_file;
    if (argc == 2) {
        auto len = std::strlen(argv[1]);
        for (auto i = 0; i<len; ++i) {
            if (argv[1][i] == '\\') {
                argv[1][i] = '/';
            }
        }
        input_file = argv[1];
    } else {
        throw std::invalid_argument("Use command line to specify the input file");
    }

    std::ifstream input_stream(input_file);

    auto build_output_file = output_dir / input_file.stem().concat(".IREmitter.txt");
//    auto interpret_output_file = output_dir / input_file.stem().concat(".IREmitter.out");
    auto log_file = output_dir / input_file.stem().concat(".IREmitter.log");
    std::ofstream output_stream(build_output_file, std::ios::trunc);
    std::ofstream log_stream(log_file, std::ios::trunc);
    SysYust::Logger::setLogger(new SysYust::StreamLogger(log_stream));


    antlr4::ANTLRInputStream antlr_input(input_stream);
    SysYLexer lexer(&antlr_input);
    antlr4::CommonTokenStream token_stream(&lexer);
    SysYParser parser(&token_stream);
    auto tree = parser.compUnit();

    auto ASTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

#ifndef NDEBUG
    SysYust::AST::SyntaxTreeString formatted_tree(*ASTree);
    output_stream << static_cast<std::string>(formatted_tree) << std::endl;
#endif

    LOG_INFO("AST build complete.");

    IREmitter::IREmitter iremmiter{};
    auto code = iremmiter.enter(ASTree.get());

    std::cout << fmt::to_string(code) << std::endl;

    return 0;
}
