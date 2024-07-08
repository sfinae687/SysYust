//
// Created by LL06p on 24-7-6.
//

#include <ranges>

#include "utility/Logger.h"
#include "AST/SyntaxTreeBuilder.h"

namespace SysYust::AST {

    namespace ranges = std::ranges;
    namespace views = std::views;

    SyntaxTreeBuilder::SyntaxTreeBuilder(SysYParser::CompUnitContext *tree)
    : rawTree(tree)
    , v(*this) {

    }

    std::unique_ptr<SyntaxTree> SyntaxTreeBuilder::getTree() {
        if (rawTree) {
            rawTree = nullptr;
            currentEnv = nullptr;
            return std::move(tree);
        } else {
            return nullptr;
        }
    }

    std::unique_ptr<SyntaxTree> SyntaxTreeBuilder::getTree(SysYParser::CompUnitContext *tree) {
        SyntaxTreeBuilder builder(tree);
        return builder.getTree();
    }

    SyntaxTreeBuilder::Visitor::Visitor(SyntaxTreeBuilder &b)
    : global(b) {

    }


    // 节点构建

    std::any SyntaxTreeBuilder::Visitor::visitCompUnit(SysYParser::CompUnitContext *ctx) {
        for (auto decl : ctx->children) {
            decl->accept(this);
        }
        return nullptr;
    }

    std::any SyntaxTreeBuilder::Visitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
        auto &baseType = toType(ctx->type()->toString());

        for (auto constDef : ctx->constDef()) {
            auto nodeId = global.tree->pushNode();
            auto node = new VarDecl;
            auto name = constDef->Ident()->toString();
            auto nameId = global.currentEnv->getId(name);
            auto arrayRank = constDef->constExp();

            if (arrayRank.empty()) { // 非数组
                VarInfo info{std::move(name), &baseType, true, false};
                global.currentEnv->var_table.setInfo(nameId, std::move(info));

            } else { // 数组

                // 将声明数组维度的声明列表的值计算出来
                auto dimensions = arrayRank
                        | views::transform([&](auto constExpr) {
                            return std::any_cast<std::int32_t>(constExpr->accept(this));
                        })
                        | views::reverse;
                std::vector<std::size_t> dim(dimensions.begin(), dimensions.end());

                auto &type = Array::create(baseType, dim);
                VarInfo info{std::move(name), &type, true, false};
                global.currentEnv->var_table.setInfo(nameId, std::move(info));
            }

            node->info_id = nameId;
//            LOG_TRACE("constant {}:{} declared", name,
            global.tree->setNode(nodeId, node);
        }

        return 0; // 顶层声明，无需返回编号
    }

    const Type &SyntaxTreeBuilder::Visitor::toType(std::string_view type) {
        if (type == "void") {
            return Void_v;
        } else if (type == "int") {
            return Int_v;
        } else if (type == "float") {
            return Float_v;
        } else {
            /// @todo 添加一个匹配，使得允许某些类型确实

            LOG_ERROR("Unrecognised type {}", type);
            std::exit(EXIT_FAILURE);
        }
    }
} // AST