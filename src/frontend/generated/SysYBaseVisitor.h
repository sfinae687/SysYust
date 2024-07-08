
// Generated from E:/Source/SysYust/src/frontend/SysY.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SysYVisitor.h"


/**
 * This class provides an empty implementation of SysYVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SysYBaseVisitor : public SysYVisitor {
public:

  virtual std::any visitCompUnit(SysYParser::CompUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstDef(SysYParser::ConstDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitVarDef(SysYParser::InitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprInit(SysYParser::ExprInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitListInit(SysYParser::ListInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstExp(SysYParser::ConstExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstInit(SysYParser::ConstInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstListInit(SysYParser::ConstListInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(SysYParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(SysYParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItem(SysYParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign(SysYParser::AssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(SysYParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockStmt(SysYParser::BlockStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIf(SysYParser::IfContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhile(SysYParser::WhileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreak(SysYParser::BreakContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinue(SysYParser::ContinueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn(SysYParser::ReturnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExp(SysYParser::ExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCond(SysYParser::CondContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLVal(SysYParser::LValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClosedExpr(SysYParser::ClosedExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLValValue(SysYParser::LValValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteralValue(SysYParser::LiteralValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntNumber(SysYParser::IntNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloatNumber(SysYParser::FloatNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromPrimary(SysYParser::FromPrimaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCall(SysYParser::CallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOpUnary(SysYParser::OpUnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOP(SysYParser::UnaryOPContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulOp(SysYParser::MulOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromUnary(SysYParser::FromUnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddOp(SysYParser::AddOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromMul(SysYParser::FromMulContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromAdd(SysYParser::FromAddContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelOp(SysYParser::RelOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqOp(SysYParser::EqOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromRel(SysYParser::FromRelContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromEq(SysYParser::FromEqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLAndOp(SysYParser::LAndOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLOrOp(SysYParser::LOrOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromLAnd(SysYParser::FromLAndContext *ctx) override {
    return visitChildren(ctx);
  }


};

