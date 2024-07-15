//
// Created by LL06p on 24-7-12.
//

#include <sstream>
#include <filesystem>

#include <CharStream.h>
#include <ANTLRFileStream.h>
#include <ANTLRInputStream.h>
#include "SysYLexer.h"
#include "SysYParser.h"

#include "AST/SyntaxTreeBuilder.h"
#include "AST/SyntaxTreeString.h"
#include "utility/Logger.h"
#include "utility/StreamLogger.h"

#include "Interpreter/Interpreter.h"

int main(int argc, char *argv[]) {

//    auto thePath = R"(E:\Source\SysYust\compiler2023\TestCase\functional\61_sort_test7.sy)";
    auto thePath = argv[1];
    std::filesystem::path programPath{thePath};
    std::ifstream program(programPath);
    auto outFilePath = programPath.filename().string() + std::string(".out");
    auto logFilePath = programPath.filename().string() + std::string(".log");

    std::ofstream out_file(outFilePath);
//    std::ofstream log_file(logFilePath);

    SysYust::Logger::setLogger(new SysYust::StreamLogger(std::clog));

    antlr4::ANTLRInputStream input(program);
    SysYLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    auto tree = parser.compUnit();

//    out_file << tree->toStringTree(&parser, true) << std::endl;

    auto astTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

    SysYust::AST::SyntaxTreeString str(*astTree);

    out_file << (std::string)str << std::endl;

    SysYust::AST::Interpreter::Interpreter interp;
    // assert(0);
    int ret = interp.enter(&*astTree);

    out_file << interp.log_data.str() << std::endl;

    out_file << "Ans: " << ret << std::endl;

    std::cout << ret << std::endl;

    return 0;
}