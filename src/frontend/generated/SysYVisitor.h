
// Generated from /home/cli/home/wsp/tcsb/comp/SysYust/src/frontend/SysY.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SysYParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SysYParser.
 */
class  SysYVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SysYParser.
   */
    virtual std::any visitCompUnit(SysYParser::CompUnitContext *context) = 0;

    virtual std::any visitConstDecl(SysYParser::ConstDeclContext *context) = 0;

    virtual std::any visitVarDecl(SysYParser::VarDeclContext *context) = 0;

    virtual std::any visitConstDef(SysYParser::ConstDefContext *context) = 0;

    virtual std::any visitUninitVarDef(SysYParser::UninitVarDefContext *context) = 0;

    virtual std::any visitInitVarDef(SysYParser::InitVarDefContext *context) = 0;

    virtual std::any visitExprInit(SysYParser::ExprInitContext *context) = 0;

    virtual std::any visitListInit(SysYParser::ListInitContext *context) = 0;

    virtual std::any visitConstExp(SysYParser::ConstExpContext *context) = 0;

    virtual std::any visitConstInit(SysYParser::ConstInitContext *context) = 0;

    virtual std::any visitConstListInit(SysYParser::ConstListInitContext *context) = 0;

    virtual std::any visitType(SysYParser::TypeContext *context) = 0;

    virtual std::any visitFuncDef(SysYParser::FuncDefContext *context) = 0;

    virtual std::any visitFuncFParams(SysYParser::FuncFParamsContext *context) = 0;

    virtual std::any visitFuncFParam(SysYParser::FuncFParamContext *context) = 0;

    virtual std::any visitBlock(SysYParser::BlockContext *context) = 0;

    virtual std::any visitBlockItem(SysYParser::BlockItemContext *context) = 0;

    virtual std::any visitAssign(SysYParser::AssignContext *context) = 0;

    virtual std::any visitExpr(SysYParser::ExprContext *context) = 0;

    virtual std::any visitBlockStmt(SysYParser::BlockStmtContext *context) = 0;

    virtual std::any visitIf(SysYParser::IfContext *context) = 0;

    virtual std::any visitWhile(SysYParser::WhileContext *context) = 0;

    virtual std::any visitBreak(SysYParser::BreakContext *context) = 0;

    virtual std::any visitContinue(SysYParser::ContinueContext *context) = 0;

    virtual std::any visitReturn(SysYParser::ReturnContext *context) = 0;

    virtual std::any visitExp(SysYParser::ExpContext *context) = 0;

    virtual std::any visitCond(SysYParser::CondContext *context) = 0;

    virtual std::any visitLVal(SysYParser::LValContext *context) = 0;

    virtual std::any visitClosedExpr(SysYParser::ClosedExprContext *context) = 0;

    virtual std::any visitLValValue(SysYParser::LValValueContext *context) = 0;

    virtual std::any visitLiteralValue(SysYParser::LiteralValueContext *context) = 0;

    virtual std::any visitIntNumber(SysYParser::IntNumberContext *context) = 0;

    virtual std::any visitFloatNumber(SysYParser::FloatNumberContext *context) = 0;

    virtual std::any visitFromPrimary(SysYParser::FromPrimaryContext *context) = 0;

    virtual std::any visitCall(SysYParser::CallContext *context) = 0;

    virtual std::any visitOpUnary(SysYParser::OpUnaryContext *context) = 0;

    virtual std::any visitUnaryOP(SysYParser::UnaryOPContext *context) = 0;

    virtual std::any visitFuncRParams(SysYParser::FuncRParamsContext *context) = 0;

    virtual std::any visitMulOp(SysYParser::MulOpContext *context) = 0;

    virtual std::any visitFromUnary(SysYParser::FromUnaryContext *context) = 0;

    virtual std::any visitAddOp(SysYParser::AddOpContext *context) = 0;

    virtual std::any visitFromMul(SysYParser::FromMulContext *context) = 0;

    virtual std::any visitFromAdd(SysYParser::FromAddContext *context) = 0;

    virtual std::any visitRelOp(SysYParser::RelOpContext *context) = 0;

    virtual std::any visitEqOp(SysYParser::EqOpContext *context) = 0;

    virtual std::any visitFromRel(SysYParser::FromRelContext *context) = 0;

    virtual std::any visitFromEq(SysYParser::FromEqContext *context) = 0;

    virtual std::any visitLAndOp(SysYParser::LAndOpContext *context) = 0;

    virtual std::any visitLOrOp(SysYParser::LOrOpContext *context) = 0;

    virtual std::any visitFromLAnd(SysYParser::FromLAndContext *context) = 0;


};

