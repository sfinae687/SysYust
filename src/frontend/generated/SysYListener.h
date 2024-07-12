
// Generated from /home/cli/home/wsp/tcsb/comp/SysYust/src/frontend/SysY.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SysYParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by SysYParser.
 */
class  SysYListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterCompUnit(SysYParser::CompUnitContext *ctx) = 0;
  virtual void exitCompUnit(SysYParser::CompUnitContext *ctx) = 0;

  virtual void enterConstDecl(SysYParser::ConstDeclContext *ctx) = 0;
  virtual void exitConstDecl(SysYParser::ConstDeclContext *ctx) = 0;

  virtual void enterVarDecl(SysYParser::VarDeclContext *ctx) = 0;
  virtual void exitVarDecl(SysYParser::VarDeclContext *ctx) = 0;

  virtual void enterConstDef(SysYParser::ConstDefContext *ctx) = 0;
  virtual void exitConstDef(SysYParser::ConstDefContext *ctx) = 0;

  virtual void enterUninitVarDef(SysYParser::UninitVarDefContext *ctx) = 0;
  virtual void exitUninitVarDef(SysYParser::UninitVarDefContext *ctx) = 0;

  virtual void enterInitVarDef(SysYParser::InitVarDefContext *ctx) = 0;
  virtual void exitInitVarDef(SysYParser::InitVarDefContext *ctx) = 0;

  virtual void enterExprInit(SysYParser::ExprInitContext *ctx) = 0;
  virtual void exitExprInit(SysYParser::ExprInitContext *ctx) = 0;

  virtual void enterListInit(SysYParser::ListInitContext *ctx) = 0;
  virtual void exitListInit(SysYParser::ListInitContext *ctx) = 0;

  virtual void enterConstExp(SysYParser::ConstExpContext *ctx) = 0;
  virtual void exitConstExp(SysYParser::ConstExpContext *ctx) = 0;

  virtual void enterConstInit(SysYParser::ConstInitContext *ctx) = 0;
  virtual void exitConstInit(SysYParser::ConstInitContext *ctx) = 0;

  virtual void enterConstListInit(SysYParser::ConstListInitContext *ctx) = 0;
  virtual void exitConstListInit(SysYParser::ConstListInitContext *ctx) = 0;

  virtual void enterType(SysYParser::TypeContext *ctx) = 0;
  virtual void exitType(SysYParser::TypeContext *ctx) = 0;

  virtual void enterFuncDef(SysYParser::FuncDefContext *ctx) = 0;
  virtual void exitFuncDef(SysYParser::FuncDefContext *ctx) = 0;

  virtual void enterFuncFParams(SysYParser::FuncFParamsContext *ctx) = 0;
  virtual void exitFuncFParams(SysYParser::FuncFParamsContext *ctx) = 0;

  virtual void enterFuncFParam(SysYParser::FuncFParamContext *ctx) = 0;
  virtual void exitFuncFParam(SysYParser::FuncFParamContext *ctx) = 0;

  virtual void enterBlock(SysYParser::BlockContext *ctx) = 0;
  virtual void exitBlock(SysYParser::BlockContext *ctx) = 0;

  virtual void enterBlockItem(SysYParser::BlockItemContext *ctx) = 0;
  virtual void exitBlockItem(SysYParser::BlockItemContext *ctx) = 0;

  virtual void enterAssign(SysYParser::AssignContext *ctx) = 0;
  virtual void exitAssign(SysYParser::AssignContext *ctx) = 0;

  virtual void enterExpr(SysYParser::ExprContext *ctx) = 0;
  virtual void exitExpr(SysYParser::ExprContext *ctx) = 0;

  virtual void enterBlockStmt(SysYParser::BlockStmtContext *ctx) = 0;
  virtual void exitBlockStmt(SysYParser::BlockStmtContext *ctx) = 0;

  virtual void enterIf(SysYParser::IfContext *ctx) = 0;
  virtual void exitIf(SysYParser::IfContext *ctx) = 0;

  virtual void enterWhile(SysYParser::WhileContext *ctx) = 0;
  virtual void exitWhile(SysYParser::WhileContext *ctx) = 0;

  virtual void enterBreak(SysYParser::BreakContext *ctx) = 0;
  virtual void exitBreak(SysYParser::BreakContext *ctx) = 0;

  virtual void enterContinue(SysYParser::ContinueContext *ctx) = 0;
  virtual void exitContinue(SysYParser::ContinueContext *ctx) = 0;

  virtual void enterReturn(SysYParser::ReturnContext *ctx) = 0;
  virtual void exitReturn(SysYParser::ReturnContext *ctx) = 0;

  virtual void enterExp(SysYParser::ExpContext *ctx) = 0;
  virtual void exitExp(SysYParser::ExpContext *ctx) = 0;

  virtual void enterCond(SysYParser::CondContext *ctx) = 0;
  virtual void exitCond(SysYParser::CondContext *ctx) = 0;

  virtual void enterLVal(SysYParser::LValContext *ctx) = 0;
  virtual void exitLVal(SysYParser::LValContext *ctx) = 0;

  virtual void enterClosedExpr(SysYParser::ClosedExprContext *ctx) = 0;
  virtual void exitClosedExpr(SysYParser::ClosedExprContext *ctx) = 0;

  virtual void enterLValValue(SysYParser::LValValueContext *ctx) = 0;
  virtual void exitLValValue(SysYParser::LValValueContext *ctx) = 0;

  virtual void enterLiteralValue(SysYParser::LiteralValueContext *ctx) = 0;
  virtual void exitLiteralValue(SysYParser::LiteralValueContext *ctx) = 0;

  virtual void enterIntNumber(SysYParser::IntNumberContext *ctx) = 0;
  virtual void exitIntNumber(SysYParser::IntNumberContext *ctx) = 0;

  virtual void enterFloatNumber(SysYParser::FloatNumberContext *ctx) = 0;
  virtual void exitFloatNumber(SysYParser::FloatNumberContext *ctx) = 0;

  virtual void enterFromPrimary(SysYParser::FromPrimaryContext *ctx) = 0;
  virtual void exitFromPrimary(SysYParser::FromPrimaryContext *ctx) = 0;

  virtual void enterCall(SysYParser::CallContext *ctx) = 0;
  virtual void exitCall(SysYParser::CallContext *ctx) = 0;

  virtual void enterOpUnary(SysYParser::OpUnaryContext *ctx) = 0;
  virtual void exitOpUnary(SysYParser::OpUnaryContext *ctx) = 0;

  virtual void enterUnaryOP(SysYParser::UnaryOPContext *ctx) = 0;
  virtual void exitUnaryOP(SysYParser::UnaryOPContext *ctx) = 0;

  virtual void enterFuncRParams(SysYParser::FuncRParamsContext *ctx) = 0;
  virtual void exitFuncRParams(SysYParser::FuncRParamsContext *ctx) = 0;

  virtual void enterMulOp(SysYParser::MulOpContext *ctx) = 0;
  virtual void exitMulOp(SysYParser::MulOpContext *ctx) = 0;

  virtual void enterFromUnary(SysYParser::FromUnaryContext *ctx) = 0;
  virtual void exitFromUnary(SysYParser::FromUnaryContext *ctx) = 0;

  virtual void enterAddOp(SysYParser::AddOpContext *ctx) = 0;
  virtual void exitAddOp(SysYParser::AddOpContext *ctx) = 0;

  virtual void enterFromMul(SysYParser::FromMulContext *ctx) = 0;
  virtual void exitFromMul(SysYParser::FromMulContext *ctx) = 0;

  virtual void enterFromAdd(SysYParser::FromAddContext *ctx) = 0;
  virtual void exitFromAdd(SysYParser::FromAddContext *ctx) = 0;

  virtual void enterRelOp(SysYParser::RelOpContext *ctx) = 0;
  virtual void exitRelOp(SysYParser::RelOpContext *ctx) = 0;

  virtual void enterEqOp(SysYParser::EqOpContext *ctx) = 0;
  virtual void exitEqOp(SysYParser::EqOpContext *ctx) = 0;

  virtual void enterFromRel(SysYParser::FromRelContext *ctx) = 0;
  virtual void exitFromRel(SysYParser::FromRelContext *ctx) = 0;

  virtual void enterFromEq(SysYParser::FromEqContext *ctx) = 0;
  virtual void exitFromEq(SysYParser::FromEqContext *ctx) = 0;

  virtual void enterLAndOp(SysYParser::LAndOpContext *ctx) = 0;
  virtual void exitLAndOp(SysYParser::LAndOpContext *ctx) = 0;

  virtual void enterLOrOp(SysYParser::LOrOpContext *ctx) = 0;
  virtual void exitLOrOp(SysYParser::LOrOpContext *ctx) = 0;

  virtual void enterFromLAnd(SysYParser::FromLAndContext *ctx) = 0;
  virtual void exitFromLAnd(SysYParser::FromLAndContext *ctx) = 0;


};

