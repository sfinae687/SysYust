#include <gtest/gtest.h>

#include "AST/Type/Pointer.h"
#include "Interpreter/Interpreter.h"
#include "Interpreter/Value.h"
#include "Logger.h"

using namespace SysYust;
using namespace SysYust::AST;
using namespace SysYust::AST::Interpreter;
using namespace std;

TEST(InterpreterTest, InterpreterFunctinoalTest) {
    using namespace SysYust::AST;

    SyntaxTree tree;
    auto currentEnv = tree.topEnv();

    /*
     * 一个语法树 for
     * int main(void) {
     *  return 0 + 1 - 1;
     * }
     */

    // 创建一个函数 main: int->void
    auto mainFuncNo = tree.pushNode(); // 分配节点的编号
    auto &mainFuncType = Function::create(Int_v, {&Void_v}); // 创建函数的类型
    auto nameId = currentEnv->getId("main"); // 分配函数名字符号的 Id
    FuncInfo mainInfo("main", &mainFuncType, mainFuncNo); // 创建函数符号信息

    // 创建函数内的语句 {return 0 + 1 - 1};
    auto blockNo = tree.pushNode();
        auto returnNo = tree.pushNode();
            auto subNo = tree.pushNode();
                auto addNo = tree.pushNode();
                    auto v1No = tree.pushNode();
                    auto v1N = new IntLiteral(0);
                    tree.setNode(v1No, v1N);
                    auto v2No = tree.pushNode();
                    auto v2N = new IntLiteral(1);
                    tree.setNode(v2No, v2N);
                    auto addN = new BinaryOp(&Int_v, BinaryOp::Add, v1No, v2No);
                tree.setNode(addNo, addN);
                auto v3No = tree.pushNode();
                auto v3N = new IntLiteral(1);
                tree.setNode(v3No, v3N);
                auto subN = new BinaryOp(&Int_v, BinaryOp::Sub, addNo, v3No);
            tree.setNode(subNo, subN);
            auto returnN = new Return(subNo);
        tree.setNode(returnNo, returnN);
        auto blockN = new Block({returnNo});
    tree.setNode(blockNo, blockN);

    auto mainN = new FuncDecl();
    mainN->entry_node = blockNo;
    mainN->info_id = nameId;
    tree.setNode(mainFuncNo, mainN);
    currentEnv->func_table.setInfo(nameId, mainInfo);

    EXPECT_TRUE(tree.checkComplete());
    testing::internal::CaptureStderr();
    AST::Interpreter::Interpreter interp;
    int ret = interp.enter(&tree);
    cout << "return " << ret << endl;
}