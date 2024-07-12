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



int main(int argc, char *argv[]) {

    auto thePath = R"(E:\Source\SysYust\compiler2023\TestCase\functional\21_if_test2.sy)";
//    auto thePath = argv[1];
    std::filesystem::path programPath{thePath};
    std::ifstream program(programPath);
    std::string outFilePath = programPath.filename().string() + std::string(".out");

    std::ofstream out_file(outFilePath);

    SysYust::Logger::setLogger(new SysYust::StreamLogger(std::cerr));

    antlr4::ANTLRInputStream input(program);
    SysYLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    auto tree = parser.compUnit();

//    out_file << tree->toStringTree(&parser, true) << std::endl;

    auto astTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

    SysYust::AST::SyntaxTreeString str(*astTree);

    out_file << (std::string)str << std::endl;

    return 0;
}