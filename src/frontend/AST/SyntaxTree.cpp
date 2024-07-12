//
// Created by LL06p on 24-7-5.
//

#include "utility/Logger.h"

#include "AST/SyntaxTree.h"

namespace SysYust::AST {
    HNode SyntaxTree::pushNode() {
        return pushNode(nullptr);
    }

    HNode SyntaxTree::pushNode(Node *node) {
        auto rt = _all_nodes.size();
        _all_nodes.push_back(node);
        return rt;
    }

    void SyntaxTree::setNode(HNode no, Node *node) {
        if (!_all_nodes[no]) {
            _all_nodes[no] = node;
        } else {
            LOG_WARN("Set Node for a non-nullptr node");
        }
    }

    std::shared_ptr<Env> SyntaxTree::pushEnv() {
        auto newEnv = std::make_shared<Env>(_current_top_env);
        _current_top_env = newEnv;
        return newEnv;
    }

    std::shared_ptr<Env> SyntaxTree::popEnv() {
        auto newEvn = _current_top_env->getParent();
        _current_top_env = newEvn;
        return newEvn;
    }

    std::shared_ptr<Env> SyntaxTree::topEnv() const {
        return _current_top_env;
    }

    std::shared_ptr<Env> SyntaxTree::seekEnv(Node *n) const {
        return _env_map.at(n);
    }

    void SyntaxTree::setupEnv(Node *n, std::shared_ptr<Env> env) {
        _env_map[n] = std::move(env);
    }

    void SyntaxTree::release() {
        for (auto node : _all_nodes) {
            delete node;
        }
        reset();
    }

    void SyntaxTree::reset() {
        _all_nodes = {};
        _current_top_env = _root_env = std::make_shared<Env>();
        _env_map = {};
    }

    bool SyntaxTree::checkComplete() const {
        bool rt = true;

        auto len = _all_nodes.size();
        for (int i=0; i<len; ++i) {
            if (!_all_nodes[i]) {
                LOG_WARN("Node missing at {}", i);
                rt = false;
            }
        }
        return rt;
    }

    std::variant<std::int32_t, float> SyntaxTree::getLit(HNode n) {
        auto e = getNode<Expr>(n);
        if (typeid(e) == typeid(FloatLiteral)) {
            auto fe = static_cast<FloatLiteral*>(e);
            return fe->value;
        } else if (typeid(e) == typeid(IntLiteral)) {
            auto ie = static_cast<IntLiteral*>(e);
            return ie->value;
        } else {
            LOG_ERROR("Node isn't a literal");
            return {};
        }
    }

    std::int32_t SyntaxTree::getIntLit(HNode n) const {
        auto e = getNode<IntLiteral>(n);
        return e->value;
    }

    float SyntaxTree::getFloatLit(HNode n) const {
        auto e = getNode<FloatLiteral>(n);
        return e->value;
    }

    std::vector<HNode> SyntaxTree::getList(HNode n) const {
        auto e = getNode<List>(n);
        return e->vals;
    }
} // AST