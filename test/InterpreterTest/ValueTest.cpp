#include <gtest/gtest.h>

#include "AST/Type/Pointer.h"
#include "Interpreter/Value.h"
#include "Logger.h"
#include "StreamLogger.h"

using namespace SysYust;
using namespace SysYust::AST;
using namespace SysYust::AST::Interpreter;
using namespace std;

TEST(ValueTest, CreateValueTest) {
    Logger::setLogger(new StreamLogger(std::clog));
    auto val = Value();

    // SysYust::Logger::setLogger();
    auto val_int = Value(&Int_v, 333);
    cout << val_int.toString() << endl;
    auto val_float = Value(&Float_v, .233f);
    cout << val_float.toString() << endl;
    

    auto &a1 = AST::Array::create(AST::Int_v, {1, 2, 3});
    int ta1[3][2][1] = {1, 2, 3, 4, 5, 6};
    MemorySlice ma1(a1);
    auto va1 = Value(&a1, ma1); 
    cout << va1.toString() << endl;

    auto &p0 = AST::Pointer::create(Int_v);
    MemorySlice ma2(Int_v);
    *ma2.access<Int>() = 2;
    auto vp0 = Value(&p0, ma2);
    cout << vp0.toString() << endl;

    auto &a2 = AST::Array::create(AST::Int_v, {1, 2});
    auto &p1 = AST::Pointer::create(a2);
    auto vp1 = Value(&p1, ma1); 
    
    cout << vp1.toString() << endl;
}