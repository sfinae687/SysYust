//
// Created by LL06p on 24-7-12.
//

#include <sstream>

#include <CharStream.h>
#include <ANTLRFileStream.h>
#include <ANTLRInputStream.h>
#include "SysYLexer.h"
#include "SysYParser.h"

#include "AST/SyntaxTreeBuilder.h"

std::string program = R"(
int main() {
    const int a = 1;
    const int b = 2;
    a + b;
}
)";

int main() {
    std::stringstream in(program);
    antlr4::ANTLRInputStream input(in);
    SysYLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    auto tree = parser.compUnit();
    auto astTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);
    return 0;
}