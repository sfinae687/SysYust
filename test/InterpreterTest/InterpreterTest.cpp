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
#include "utility/Logger.h"
#include "utility/StreamLogger.h"
#include "Interpreter/Interpreter.h"

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;
namespace Interpreter = SysYust::AST::Interpreter;

using namespace std::literals::string_literals;

int main(int argc, char *argv[]) {
    fs::create_directories("output");
    fs::path output_dir("output");
    fs::path input_file;
    if (argc == 2) {
        input_file = argv[1];
    } else {
        throw std::invalid_argument("Use command line to specify the input file");
    }

    std::ifstream input_stream(input_file);

    auto build_output_file = output_dir / input_file.stem().concat(".Interpreter.txt");
//    auto interpret_output_file = output_dir / input_file.stem().concat(".Interpreter.out");
    auto log_file = output_dir / input_file.stem().concat(".Interpreter.log");
    std::ofstream output_stream(build_output_file, std::ios::trunc);
    std::ofstream log_stream(log_file, std::ios::trunc);
    SysYust::Logger::setLogger(new SysYust::StreamLogger(log_stream));


    antlr4::ANTLRInputStream antlr_input(input_stream);
    SysYLexer lexer(&antlr_input);
    antlr4::CommonTokenStream token_stream(&lexer);
    SysYParser parser(&token_stream);
    auto tree = parser.compUnit();

    auto ASTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

    LOG_INFO("AST build complete.");

    Interpreter::Interpreter interpreter{};
    auto rt = interpreter.enter(ASTree.get());

    std::cout << std::endl;

    std::cout << rt << std::endl;

    return 0;

}
