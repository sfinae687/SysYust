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
#include "AST/SyntaxTreeString.h"
#include "utility/Logger.h"
#include "utility/StreamLogger.h"

std::string program = R"(
int a;
const int b = 1;
int c[12];
float d;

int func(int a) {
    return func(a + 1);
}

int main() {
    int a;
    if (a == 1) {
        a = a + 1;
    }
    while (a == 1) {
        a = a + 1;
        if (a == 100) {
            break;
        } else {
            continue;
        }
    }
    return 1 + 1;
}
)";

int main() {

    SysYust::Logger::setLogger(new SysYust::StreamLogger(std::cerr));

    std::stringstream in(program);
    antlr4::ANTLRInputStream input(in);
    SysYLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    auto tree = parser.compUnit();

    std::cout << tree->toStringTree(&parser, true) << std::endl;

    auto astTree = SysYust::AST::SyntaxTreeBuilder::getTree(tree);

    SysYust::AST::SyntaxTreeString str(*astTree);

    std::cout << (std::string)str << std::endl;

    return 0;
}