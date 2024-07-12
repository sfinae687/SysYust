
// Generated from /home/cli/home/wsp/tcsb/comp/SysYust/src/frontend/SysY.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SysYListener.h"


/**
 * This class provides an empty implementation of SysYListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  SysYBaseListener : public SysYListener {
public:

  virtual void enterCompUnit(SysYParser::CompUnitContext * /*ctx*/) override { }
  virtual void exitCompUnit(SysYParser::CompUnitContext * /*ctx*/) override { }

  virtual void enterConstDecl(SysYParser::ConstDeclContext * /*ctx*/) override { }
  virtual void exitConstDecl(SysYParser::ConstDeclContext * /*ctx*/) override { }

  virtual void enterVarDecl(SysYParser::VarDeclContext * /*ctx*/) override { }
  virtual void exitVarDecl(SysYParser::VarDeclContext * /*ctx*/) override { }

  virtual void enterConstDef(SysYParser::ConstDefContext * /*ctx*/) override { }
  virtual void exitConstDef(SysYParser::ConstDefContext * /*ctx*/) override { }

  virtual void enterUninitVarDef(SysYParser::UninitVarDefContext * /*ctx*/) override { }
  virtual void exitUninitVarDef(SysYParser::UninitVarDefContext * /*ctx*/) override { }

  virtual void enterInitVarDef(SysYParser::InitVarDefContext * /*ctx*/) override { }
  virtual void exitInitVarDef(SysYParser::InitVarDefContext * /*ctx*/) override { }

  virtual void enterExprInit(SysYParser::ExprInitContext * /*ctx*/) override { }
  virtual void exitExprInit(SysYParser::ExprInitContext * /*ctx*/) override { }

  virtual void enterListInit(SysYParser::ListInitContext * /*ctx*/) override { }
  virtual void exitListInit(SysYParser::ListInitContext * /*ctx*/) override { }

  virtual void enterConstExp(SysYParser::ConstExpContext * /*ctx*/) override { }
  virtual void exitConstExp(SysYParser::ConstExpContext * /*ctx*/) override { }

  virtual void enterConstInit(SysYParser::ConstInitContext * /*ctx*/) override { }
  virtual void exitConstInit(SysYParser::ConstInitContext * /*ctx*/) override { }

  virtual void enterConstListInit(SysYParser::ConstListInitContext * /*ctx*/) override { }
  virtual void exitConstListInit(SysYParser::ConstListInitContext * /*ctx*/) override { }

  virtual void enterType(SysYParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(SysYParser::TypeContext * /*ctx*/) override { }

  virtual void enterFuncDef(SysYParser::FuncDefContext * /*ctx*/) override { }
  virtual void exitFuncDef(SysYParser::FuncDefContext * /*ctx*/) override { }

  virtual void enterFuncFParams(SysYParser::FuncFParamsContext * /*ctx*/) override { }
  virtual void exitFuncFParams(SysYParser::FuncFParamsContext * /*ctx*/) override { }

  virtual void enterFuncFParam(SysYParser::FuncFParamContext * /*ctx*/) override { }
  virtual void exitFuncFParam(SysYParser::FuncFParamContext * /*ctx*/) override { }

  virtual void enterBlock(SysYParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(SysYParser::BlockContext * /*ctx*/) override { }

  virtual void enterBlockItem(SysYParser::BlockItemContext * /*ctx*/) override { }
  virtual void exitBlockItem(SysYParser::BlockItemContext * /*ctx*/) override { }

  virtual void enterAssign(SysYParser::AssignContext * /*ctx*/) override { }
  virtual void exitAssign(SysYParser::AssignContext * /*ctx*/) override { }

  virtual void enterExpr(SysYParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(SysYParser::ExprContext * /*ctx*/) override { }

  virtual void enterBlockStmt(SysYParser::BlockStmtContext * /*ctx*/) override { }
  virtual void exitBlockStmt(SysYParser::BlockStmtContext * /*ctx*/) override { }

  virtual void enterIf(SysYParser::IfContext * /*ctx*/) override { }
  virtual void exitIf(SysYParser::IfContext * /*ctx*/) override { }

  virtual void enterWhile(SysYParser::WhileContext * /*ctx*/) override { }
  virtual void exitWhile(SysYParser::WhileContext * /*ctx*/) override { }

  virtual void enterBreak(SysYParser::BreakContext * /*ctx*/) override { }
  virtual void exitBreak(SysYParser::BreakContext * /*ctx*/) override { }

  virtual void enterContinue(SysYParser::ContinueContext * /*ctx*/) override { }
  virtual void exitContinue(SysYParser::ContinueContext * /*ctx*/) override { }

  virtual void enterReturn(SysYParser::ReturnContext * /*ctx*/) override { }
  virtual void exitReturn(SysYParser::ReturnContext * /*ctx*/) override { }

  virtual void enterExp(SysYParser::ExpContext * /*ctx*/) override { }
  virtual void exitExp(SysYParser::ExpContext * /*ctx*/) override { }

  virtual void enterCond(SysYParser::CondContext * /*ctx*/) override { }
  virtual void exitCond(SysYParser::CondContext * /*ctx*/) override { }

  virtual void enterLVal(SysYParser::LValContext * /*ctx*/) override { }
  virtual void exitLVal(SysYParser::LValContext * /*ctx*/) override { }

  virtual void enterClosedExpr(SysYParser::ClosedExprContext * /*ctx*/) override { }
  virtual void exitClosedExpr(SysYParser::ClosedExprContext * /*ctx*/) override { }

  virtual void enterLValValue(SysYParser::LValValueContext * /*ctx*/) override { }
  virtual void exitLValValue(SysYParser::LValValueContext * /*ctx*/) override { }

  virtual void enterLiteralValue(SysYParser::LiteralValueContext * /*ctx*/) override { }
  virtual void exitLiteralValue(SysYParser::LiteralValueContext * /*ctx*/) override { }

  virtual void enterIntNumber(SysYParser::IntNumberContext * /*ctx*/) override { }
  virtual void exitIntNumber(SysYParser::IntNumberContext * /*ctx*/) override { }

  virtual void enterFloatNumber(SysYParser::FloatNumberContext * /*ctx*/) override { }
  virtual void exitFloatNumber(SysYParser::FloatNumberContext * /*ctx*/) override { }

  virtual void enterFromPrimary(SysYParser::FromPrimaryContext * /*ctx*/) override { }
  virtual void exitFromPrimary(SysYParser::FromPrimaryContext * /*ctx*/) override { }

  virtual void enterCall(SysYParser::CallContext * /*ctx*/) override { }
  virtual void exitCall(SysYParser::CallContext * /*ctx*/) override { }

  virtual void enterOpUnary(SysYParser::OpUnaryContext * /*ctx*/) override { }
  virtual void exitOpUnary(SysYParser::OpUnaryContext * /*ctx*/) override { }

  virtual void enterUnaryOP(SysYParser::UnaryOPContext * /*ctx*/) override { }
  virtual void exitUnaryOP(SysYParser::UnaryOPContext * /*ctx*/) override { }

  virtual void enterFuncRParams(SysYParser::FuncRParamsContext * /*ctx*/) override { }
  virtual void exitFuncRParams(SysYParser::FuncRParamsContext * /*ctx*/) override { }

  virtual void enterMulOp(SysYParser::MulOpContext * /*ctx*/) override { }
  virtual void exitMulOp(SysYParser::MulOpContext * /*ctx*/) override { }

  virtual void enterFromUnary(SysYParser::FromUnaryContext * /*ctx*/) override { }
  virtual void exitFromUnary(SysYParser::FromUnaryContext * /*ctx*/) override { }

  virtual void enterAddOp(SysYParser::AddOpContext * /*ctx*/) override { }
  virtual void exitAddOp(SysYParser::AddOpContext * /*ctx*/) override { }

  virtual void enterFromMul(SysYParser::FromMulContext * /*ctx*/) override { }
  virtual void exitFromMul(SysYParser::FromMulContext * /*ctx*/) override { }

  virtual void enterFromAdd(SysYParser::FromAddContext * /*ctx*/) override { }
  virtual void exitFromAdd(SysYParser::FromAddContext * /*ctx*/) override { }

  virtual void enterRelOp(SysYParser::RelOpContext * /*ctx*/) override { }
  virtual void exitRelOp(SysYParser::RelOpContext * /*ctx*/) override { }

  virtual void enterEqOp(SysYParser::EqOpContext * /*ctx*/) override { }
  virtual void exitEqOp(SysYParser::EqOpContext * /*ctx*/) override { }

  virtual void enterFromRel(SysYParser::FromRelContext * /*ctx*/) override { }
  virtual void exitFromRel(SysYParser::FromRelContext * /*ctx*/) override { }

  virtual void enterFromEq(SysYParser::FromEqContext * /*ctx*/) override { }
  virtual void exitFromEq(SysYParser::FromEqContext * /*ctx*/) override { }

  virtual void enterLAndOp(SysYParser::LAndOpContext * /*ctx*/) override { }
  virtual void exitLAndOp(SysYParser::LAndOpContext * /*ctx*/) override { }

  virtual void enterLOrOp(SysYParser::LOrOpContext * /*ctx*/) override { }
  virtual void exitLOrOp(SysYParser::LOrOpContext * /*ctx*/) override { }

  virtual void enterFromLAnd(SysYParser::FromLAndContext * /*ctx*/) override { }
  virtual void exitFromLAnd(SysYParser::FromLAndContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

