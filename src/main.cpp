//
// Created by LL06p on 24-8-7.
//
#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>
#include <SysYLexer.h>
#include <SysYParser.h>

#include <argparse/argparse.hpp>

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include "AST/SyntaxTreeBuilder.h"
#include "AST/SyntaxTreeString.h"
#include "Interpreter/Interpreter.h"

#include "IREmitter/IREmitter.h"

#include "utility/StreamLogger.h"

//namespace ranges = ::ranges
namespace views = ranges::views;
namespace fs = std::filesystem;

using namespace std::string_literals;
using namespace std::chrono_literals;

enum class CompilerStage {
    AST,
    Interpreter,
    IR,
    NativeCode,
    arg_parser_test,
};

SysYParser * Antlr4Tree(std::istream &input_stream);
std::unique_ptr<SysYust::AST::SyntaxTree> AST_Tree(const fs::path& input_file);
std::string AST_Print(const fs::path& input_file);
void AST_Interpreter(const fs::path& input_file);
std::unique_ptr<SysYust::IR::Code> IR_Code(fs::path input_file);
std::string IR_Print(fs::path input_file);

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("SysYCompiler");

    // arguments
    fs::path output_file;
    fs::path input_file;
    fs::path log_file;

    // arguments parser

    program.add_argument("-o", "--output")
        .default_value("./a.out"s)
        .help("Specify the output file");
    program.add_argument("--log")
        .default_value("-")
        .help("Specify the file used to receive log");
    program.add_argument("files")
        .required()
        .remaining();

    auto &&stageArgumentGroup = program.add_mutually_exclusive_group(false);
    stageArgumentGroup.add_argument("-S")
        .help("Generate asm code for RSIC-V")
        .flag();
    stageArgumentGroup.add_argument("--IR")
        .help("Generate ir text")
        .flag();
    stageArgumentGroup.add_argument("--Interpreter")
        .help("run as Interpreter")
        .flag();
    stageArgumentGroup.add_argument("--AST")
        .help("Generate syntax tree text")
        .flag();
    stageArgumentGroup.add_argument("--debug-arg")
        .flag();

    // parse command line argument

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr <<program;
        std::exit(EXIT_FAILURE);
    }

    // input file and output file

    output_file = program.get("-o");
    log_file = program.get("--log");
    auto input_files = program.get<std::vector<std::string>>("files");
    if (input_files.size() != 1) {
        std::cerr << "Just support one input file, but provide " << input_files.size() << std::endl;
        std::cerr << "input files are as follow:" << std::endl;
        for (auto &i : input_files) {
            std::cerr << i << std::endl;
        }
    }
    input_file = input_files.front();

    // compile mode
    CompilerStage stage;
    if (program.get<bool>("--debug-arg")) {
        stage = CompilerStage::arg_parser_test;
    } else if (program.get<bool>("--AST")) {
        stage = CompilerStage::AST;
    } else if (program.get<bool>("--Interpreter"))  {
        stage = CompilerStage::Interpreter;
    } else if (program.get<bool>("--IR")) {
        stage = CompilerStage::IR;
    } else /* if (program.get<bool>("-S")) */ {
        stage = CompilerStage::NativeCode;
    }

    // run

    // 打开输出和日志文件

    std::ostream *output_stream;
    std::ostream *log_stream;

    std::ofstream output_file_stream;
    std::ofstream log_file_stream;

    if (program.get("-o") == "-") {
        output_stream = &std::cout;
    } else {
        if (!fs::exists(output_file)) {
            fs::create_directories(output_file.parent_path());
        }
        output_file_stream.open(output_file, std::ios::trunc);
        output_stream = &output_file_stream;
    }

    if (program.get("--log") == "-") {
        log_stream = &std::clog;
    } else {
        if (!fs::exists(log_file)) {
            fs::create_directories(log_file.parent_path());
        }
        log_file_stream.open(log_file, std::ios::out | std::ios::ate);
        log_stream = &log_file_stream;
    }

    SysYust::Logger::setLogger(new SysYust::StreamLogger(*log_stream));

    switch (stage) {
        case CompilerStage::arg_parser_test:
            std::cout << "output file:" << output_file << std::endl;
            std::cout << "log file:" << log_file << std::endl;
            std::cout << "input files:" << std::endl;
            for (auto &i : input_files) {
                std::cout << '\t' << i << std::endl;
            }
            break;
        case CompilerStage::AST: {
            *output_stream << AST_Print(input_file);
            break;
        }
        case CompilerStage::Interpreter: {
            AST_Interpreter(input_file);
            break;
        }
        case CompilerStage::IR: {
            *output_stream << IR_Print(input_file);
            break;
        }
        default:
            std::cerr << "Unexpected compile stage" << std::endl;
            std::exit(EXIT_FAILURE);
    }
}


SysYParser * Antlr4Tree(std::istream &input_stream) {
    auto input = new antlr4::ANTLRInputStream(input_stream);
    auto lexer = new SysYLexer(input);
    auto tokenStream = new antlr4::CommonTokenStream(lexer);
    auto parser = new SysYParser(tokenStream);
    return parser;
}

std::unique_ptr<SysYust::AST::SyntaxTree> AST_Tree(const fs::path& input_file) {
    if (!fs::exists(input_file)) {
        std::cerr << "No such file:" << input_file << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::ifstream input_stream(input_file);

    auto parser = Antlr4Tree(input_stream);
    auto tree = parser->compUnit();

    auto ast = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

    return ast;
}

std::string AST_Print(const fs::path& input_file) {
    auto ast = AST_Tree(input_file);
    SysYust::AST::SyntaxTreeString ast_string(*ast);
    return static_cast<std::string>(ast_string);
}

void AST_Interpreter(const fs::path& input_file) {
    auto ast = AST_Tree(input_file);
    SysYust::AST::Interpreter::Interpreter interpreter{};
    interpreter.enter(ast.get());
}

std::unique_ptr<SysYust::IR::Code> IR_Code(fs::path input_file) {
    auto ast = AST_Tree(input_file);
    SysYust::AST::IREmitter::IREmitter ir_emitter{};
    auto code = ir_emitter.enter(ast.get());
    return std::unique_ptr<SysYust::IR::Code>(code);
}

std::string IR_Print(fs::path input_file) {
    auto code = IR_Code(input_file);
    return fmt::to_string(*code);
}
