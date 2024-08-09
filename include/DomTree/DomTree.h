#ifndef SYSYUST_DOMTREE_H
#define SYSYUST_DOMTREE_H

#include <vector>

#include "IR/BasicBlock.h"
#include "IR/Procedure.h"
#include "fmt/core.h"
#include "fmt/format.h"
namespace SysYust::IR {

class DomTree {
   public:
    std::vector<unsigned> dsu_fat;

    unsigned find(unsigned x) {
        if (x == dsu_fat[x]) return x;
        unsigned fa = dsu_fat[x];
        dsu_fat[x] = find(dsu_fat[x]);
        if (dfn[sdom[smin[fa]]] < dfn[sdom[smin[x]]]) smin[x] = smin[fa];
        return dsu_fat[x];
    }

    std::vector<unsigned> dfn, pos, fat;
    std::vector<BasicBlock *> bbpos;
    std::vector<unsigned> idom, sdom, smin;
    std::vector<std::vector<unsigned>> vec;
    unsigned dfc = 0;
    unsigned root;

    void dfs(BasicBlock *bb) {
        auto u = bb->id();
        dfn[u] = ++dfc;
        pos[dfc] = u;
        bbpos[dfc] = bb;
        auto looper = [&](BasicBlock *bb) {
            if (bb && !dfn[bb->id()]) {
                dfs(bb);
                fat[bb->id()] = u;
            }
        };
        looper(bb->nextBlock());
        looper(bb->elseBlock());
    }

    DomTree build(Procedure *ir) {
        unsigned n = ir->getGraph().head()->block_num();
        dsu_fat.resize(n + 1);

        dfn.resize(n + 1);
        fat.resize(n + 1);
        pos.resize(n + 1);
        bbpos.resize(n + 1);

        idom.resize(n + 1);
        sdom.resize(n + 1);
        smin.resize(n + 1);
        vec.resize(n + 1);

        root = ir->getGraph().head()->id();
        dfs(ir->getGraph().head());
        for (unsigned i = 1; i <= n; ++i) sdom[i] = dsu_fat[i] = i;
        for (unsigned i = dfc; i >= 2; --i) {
            unsigned u = pos[i];
            for (auto v : vec[u]) {
                find(v);
                if (sdom[smin[v]] == u)
                    idom[v] = u;
                else
                    idom[v] = smin[v];
            }
            if (i != 1) {
                auto looper = [&](BasicBlock *bb) {
                    if (bb && dfn[bb->id()]) {
                        auto v = bb->id();
                        if (dfn[v] < dfn[sdom[u]])
                            sdom[u] = v;
                        else if (dfn[v] > dfn[u]) {
                            find(v);
                            if (dfn[sdom[smin[v]]] < dfn[sdom[u]])
                                sdom[u] = sdom[smin[v]];
                        }
                    }
                };
                auto bb = bbpos[u];
                looper(bb->nextBlock());
                looper(bb->elseBlock());
                vec[sdom[u]].emplace_back(u);
                dsu_fat[u] = fat[u];
            }
        }
    }

    std::string toString() {
        std::string res;
        for (auto i = dfc; i >= 1; --i) {
            auto u = pos[i];
            for (auto v : vec[u]) {
                res += fmt::format("{} {}\n", u, v);
            }
        }
        return res;
    }
};

}  // namespace SysYust::IR

#endif