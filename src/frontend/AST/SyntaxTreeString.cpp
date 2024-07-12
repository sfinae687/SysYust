//
// Created by LL06p on 24-7-12.
//

#include <format>
#include <ranges>

#include "AST/SyntaxTreeString.h"

namespace ranges = std::ranges;
namespace views = std::views;

namespace SysYust {
    namespace AST {
        void SyntaxTreeString::execute(const FuncDecl &decl) {
            NodeExecutorBase::execute(decl);
        }

        void SyntaxTreeString::execute(const ParamDecl &decl) {
            auto infoId = decl.info_id;
            auto info = currentEnv->var_table.getInfo(infoId);

            std::string buffer = std::format("PARAM {}(ID{}):{}", info.name, infoId, info.type->toString());
            strings.emplace_back(std::move(buffer));
        }

        void SyntaxTreeString::execute(const IntLiteral &literal) {
            strings.emplace_back(std::format("<I:{}>", std::to_string(literal.value)));
        }

        void SyntaxTreeString::execute(const FloatLiteral &literal) {
            strings.emplace_back(std::format("<F:{}>", std::to_string(literal.value)));
        }

        void SyntaxTreeString::execute(const Call &call) {
            auto funcId = call.func_info;
            auto info = currentEnv->func_table.getInfo(funcId);
            std::string buffer = std::format("CALL[{}](", info.name);
            auto sPos = strings.size();
            for (auto i : call.argumentExpr) {
                tree.getNode(i)->execute(this);
            }
            auto len = strings.size();
            for (auto i=sPos; i<len; ++i) {
                buffer += std::format("{}, ", strings[i]);
            }
            buffer.pop_back();
            buffer.back() = ')';
            strings.emplace_back(std::move(buffer));
        }

        void SyntaxTreeString::execute(const UnaryOp &op) {
            tree.getNode(op.subexpr)->execute(this);
            auto substr = strings.back();
            strings.pop_back();
            std::string opS;
            switch (op.type) {
                case UnaryOp::Negative:
                    opS = "-";
                    break;
                case UnaryOp::Positive:
                    opS = "+";
                    break;
                default:;
            }
            strings.emplace_back(std::format("{}{}", opS, substr));
        }

        void SyntaxTreeString::execute(const BinaryOp &op) {
            auto [lS, rS] = executeTwo(op.lhs, op.rhs);
            std::string str;
            switch (op.type) {
                case BinaryOp::Add:
                    str = "+";
                    break;
                case BinaryOp::Sub:
                    str = "-";
                    break;
                case BinaryOp::Mul:
                    str = "*";
                    break;
                case BinaryOp::Div:
                    str = "/";
                    break;
                case BinaryOp::Mod:
                    str = "%";
                    break;
                default:;
            }
            strings.emplace_back(std::format("({}{}{})", lS, str, rS));
        }

        void SyntaxTreeString::execute(const DeclRef &decl) {
            auto varInfo = currentEnv->var_table.getInfo(decl.var_id);
            strings.emplace_back(std::format("{}(ID{})", varInfo.name, decl.var_id));
        }

        void SyntaxTreeString::execute(const ArrayRef &array) {
            auto varInfo = currentEnv->var_table.getInfo(array.var_id);
            std::string buffer = std::format("{}(ID{})", varInfo.name, array.var_id);
            for (auto i : array.subscripts) {
                tree.getNode(i)->execute(this);
                buffer += std::format("[{}]", strings.back());
                strings.pop_back();
            }
            strings.emplace_back(std::move(buffer));
        }

        void SyntaxTreeString::execute(const ToInt &anInt) {
            tree.getNode(anInt.operant)->execute(this);
        }

        void SyntaxTreeString::execute(const ToFloat &aFloat) {
            tree.getNode(aFloat.operant)->execute(this);
        }

        void SyntaxTreeString::execute(const Not &aNot) {
            tree.getNode(aNot.subexpr)->execute(this);
            strings.back() = std::format("!{}", strings.back());
        }

        void SyntaxTreeString::execute(const And &anAnd) {
            auto [lS, rS] = executeTwo(anAnd.lhs, anAnd.rhs);
            strings.emplace_back(std::format("({} && {})", lS, rS));
        }

        void SyntaxTreeString::execute(const Or &anOr) {
            auto [lS, rS] = executeTwo(anOr.lhs, anOr.rhs);
            strings.emplace_back(std::format("({} || {})", lS, rS));
        }

        void SyntaxTreeString::execute(const Compare &compare) {
            auto op = compare.type;
            auto pos = strings.size();
            tree.getNode(compare.lhs)->execute(this);
            std::string opS;
            switch (op) {
                case Compare::NE:
                    opS = "!=";
                    break;
                case Compare::EQ:
                    opS = "==";
                    break;
                case Compare::LE:
                    opS = "<=";
                    break;
                case Compare::LT:
                    opS = "<";
                    break;
                case Compare::GE:
                    opS = ">=";
                    break;
                case Compare::GT:
                    opS = ">";
                    break;
                default:;
            }
            tree.getNode(compare.rhs)->execute(this);
            strings[pos] = std::format("({}{}{})", strings[pos], opS, strings[pos+1]);
            strings.pop_back();
        }

        void SyntaxTreeString::execute(const ToCond &cond) {
            tree.getNode(cond.operant)->execute(this);
        }

        void SyntaxTreeString::execute(const If &anIf) {
            tree.getNode(anIf.cond)->execute(this);
            strings.back() = "IF COND:[" + strings.back() + "]";
            tree.getNode(anIf.stmts)->execute(this);
            if (anIf.else_stmt != -1) {
                auto sPos = strings.size();
                tree.getNode(anIf.else_stmt)->execute(this);
                strings[sPos] = std::format("ELSE {}", strings[sPos]);
                strings.back() = std::format("ELSE {}", strings.back());
            }
        }

        void SyntaxTreeString::execute(const Assign &assign) {
            auto l_val_pos = strings.size();
            tree.getNode(assign.l_val)->execute(this);
            tree.getNode(assign.r_val)->execute(this);
            strings[l_val_pos] = std::format("({} = {})", strings[l_val_pos], strings[l_val_pos+1]);
            strings.pop_back();
        }

        void SyntaxTreeString::execute(const While &aWhile) {
            tree.getNode(aWhile.cond)->execute(this);
            strings.back() = std::format("WhileCond:[{}]", strings.back());
            tree.getNode(aWhile.stmt)->execute(this);
        }

        void SyntaxTreeString::execute(const Break &aBreak) {
            strings.emplace_back("Break");
        }

        void SyntaxTreeString::execute(const Continue &aContinue) {
            strings.emplace_back("Continue");
        }

        void SyntaxTreeString::execute(const Return &aReturn) {
            tree.getNode(aReturn.returned)->execute(this);
            strings.back() = "Return[" + strings.back() + "]";
        }

        void SyntaxTreeString::execute(const Block &block) {
            strings.emplace_back("BEGIN BLOCK");


            currentEnv = tree.seekEnv(&block);

            auto sPos = strings.size();
            for (auto i : block.stmts) {
                tree.getNode(i)->execute(this);
            }
            for (auto i=sPos; i<strings.size(); ++i) {
                strings[i] = "|\t" + strings[i];
            }

            currentEnv = currentEnv->getParent();

            strings.emplace_back("END   BLOCK");
        }

        void SyntaxTreeString::execute(const Empty &empty) {
            strings.emplace_back("[]");
        }

        void SyntaxTreeString::execute(const List &list) {
            std::string str = "{";
            auto sPos = strings.size();
            for (auto i : list.vals) {
                tree.getNode(i)->execute(this);
            }
            for (auto i=sPos; i<strings.size(); ++i) {
                str += std::format("{}, ", strings[i]);
            }
            str.push_back('}');
            strings.resize(sPos);
            strings.emplace_back(std::move(str));
        }

        SyntaxTreeString::SyntaxTreeString(const SyntaxTree &tree)
        : tree(tree) {

        }

        SyntaxTreeString::operator std::string() {
            if (strings.empty()) {
                auto rootVar = tree.topEnv()->var_table;
                auto rootFunc = tree.topEnv()->func_table;

                // 遍历顶层变量
                for (auto [varId, info] : rootVar) {
                    std::string buff;
                    buff += std::format("Var {}({}) : {}", info.name, varId, info.type->toString());

                    auto node = tree.getNode<VarDecl>(info.decl);
                    if (node->init_expr) {
                        tree.getNode(node->init_expr.value())->execute(this);
                        buff += std::format(" = {}", std::move(strings.back()));
                        strings.pop_back();
                    }

                    strings.emplace_back(std::move(buff));
                }

                // 变量顶层函数
                for (auto [funcId, info] : rootFunc) {
                    std::string buff;
                    buff += std::format("Func {}({}) : {}", info.name, funcId, info.type->toString());
                    strings.emplace_back(std::move(buff));
                    auto decl = tree.getNode<FuncDecl>(info.node);

                    currentEnv = tree.seekEnv(tree.getNode(info.node));

                    // 形参
                    for (auto i : decl->param) {
                        tree.getNode(i)->execute(this);
                    }

                    // 函数体
                    tree.getNode(decl->entry_node)->execute(this);
                    currentEnv = currentEnv->getParent();
                }
            }

            for (auto &i : strings) {
                i.push_back('\n');
            }

            auto all = strings
            | views::join;
            return {all.begin(), all.end()};
        }

        std::pair<std::string, std::string> SyntaxTreeString::executeTwo(HNode lhs, HNode rhs) {
            auto sPos = strings.size();
            tree.getNode(lhs)->execute(this);
            tree.getNode(rhs)->execute(this);
            std::pair rt{strings[sPos], strings[sPos+1]};
            strings.resize(sPos);
            return rt;
        }

        void SyntaxTreeString::execute(const VarDecl &decl) {
            auto info = currentEnv->var_table.getInfo(decl.info_id);
            std::string buffer = std::format("Var {}(ID{}):{}", info.name, decl.info_id, info.type->toString());

            if (decl.init_expr) {
                tree.getNode(decl.init_expr.value())->execute(this);
                buffer += " = " + strings.back();
                strings.pop_back();
            }

            strings.emplace_back(std::move(buffer));
        }
    } // AST
} // SysYust