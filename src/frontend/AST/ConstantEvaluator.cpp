//
// Created by LL06p on 24-7-10.
//

#include <functional>

#include "AST/ConstantEvaluator.h"
#include "utility/Logger.h"

#ifdef __cpp_lib_ranges
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#else
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
    namespace views = ranges::views;
#endif

namespace ranges = std::ranges;
namespace views = std::views;

namespace SysYust::AST {
    ConstantEvaluator::ConstantEvaluator(const SyntaxTree &tree)
    : tree(tree) {

    }

    std::variant<std::int32_t, float> ConstantEvaluator::compute(HNode exp) {
        tree.getNode<Expr>(exp)->execute(this);
        auto rt = valueStack.top();
        valueStack.pop();
        return rt;
    }

    void ConstantEvaluator::execute(const BinaryOp &e) {
        auto lhs = e.lhs;
        auto rhs = e.rhs;
        tree.getNode(lhs)->execute(this);
        tree.getNode(rhs)->execute(this);

        auto rV = valueStack.top();
        valueStack.pop();
        auto lV = valueStack.top();
        valueStack.pop();
        switch (e.type) {
            case BinaryOp::Add:
                valueStack.push(std::visit([](auto l, auto r) {
                        return value_t(l + r);
                    }, lV, rV));
                break;
            case BinaryOp::Sub:
                valueStack.push(std::visit([](auto l, auto r) {
                    return value_t(l - r);
                }, lV, rV));
                break;
            case BinaryOp::Mul:
                valueStack.push(std::visit([](auto l, auto r) {
                    return value_t(l * r);
                }, lV, rV));
                break;
            case BinaryOp::Div:
                valueStack.push(std::visit([](auto l, auto r) {
                    return value_t(l / r);
                }, lV, rV));
                break;
            case BinaryOp::Mod: {
                auto lVI = std::get<std::int32_t>(lV);
                auto rVI = std::get<std::int32_t>(rV);
                valueStack.emplace(lVI % rVI);
                break;
            }
            default:
                break;
        }
    }

    void ConstantEvaluator::execute(const ToFloat &e) {
        tree.getNode(e.operant)->execute(this);
        auto topInt = std::get<std::int32_t>(valueStack.top());
        valueStack.pop();
        valueStack.emplace(static_cast<float>(topInt));
    }

    void ConstantEvaluator::execute(const UnaryOp &e) {
        tree.getNode(e.subexpr)->execute(this);
        if (e.type == UnaryOp::Negative) {
            auto tV = valueStack.top();
            valueStack.pop();
            valueStack.push(std::visit([](auto v) {
                return value_t(-v);
            }, tV));
        }

    }

    void ConstantEvaluator::execute(const DeclRef &e) {
        auto vId = e.var_id;
        auto info = tree.topEnv()->var_table.getInfo(vId);
        auto declNodeId = info.decl;
        auto initNodeId = tree.getNode<VarDecl>(declNodeId)->init_expr.value();
        if (info.isConstant) {
            auto value = compute(initNodeId);
            valueStack.push(value);
        } else {
            LOG_ERROR("reference to non-constant variable");
            std::exit(EXIT_FAILURE);
        }
    }

    void ConstantEvaluator::execute(const ArrayRef &e) {
        auto index = e.subscripts;
        auto arrId = e.var_id;
        auto arrDeclNode = tree.getNode<VarDecl>(tree.topEnv()->var_table.getInfo(arrId).decl);
        auto initList = arrDeclNode->init_expr.value();
        for (auto i : index) {
            tree.getNode(i)->execute(this);
        }
        tree.getNode(initList)->execute(this);
    }

    void ConstantEvaluator::execute(const List &e) {
        auto topIndex = std::get<std::int32_t>(valueStack.top());
        valueStack.pop();
        auto subNodes = e.vals[topIndex];
        tree.getNode(subNodes)->execute(this);
    }

    void ConstantEvaluator::execute(const FloatLiteral &e) {
        valueStack.emplace(e.value);
    }

    void ConstantEvaluator::execute(const IntLiteral &e) {
        valueStack.emplace(e.value);
    }
} // AST