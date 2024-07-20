#include <gtest/gtest.h>
#include <iostream>

#include "AST/Env/VarInfo.h"
#include "AST/Type/Int.h"
#include "AST/Type/Pointer.h"
#include "Interpreter/Interpreter.h"
#include "Interpreter/Value.h"
#include "Logger.h"
#include "StreamLogger.h"

using namespace SysYust;
using namespace SysYust::AST;
using namespace SysYust::AST::Interpreter;
using namespace std;

TEST(InterpreterTest, FunctinoalTest1) {
    using namespace SysYust::AST;

    Logger::setLogger(new StreamLogger(std::clog));

    SyntaxTree tree;
    auto currentEnv = tree.topEnv();

    /* current none */
}
