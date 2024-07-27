//
// Created by LL06p on 24-7-24.
//

#ifndef SYSYUST_CODEUTIL_H
#define SYSYUST_CODEUTIL_H

#include <cassert>
#include <stack>

#include "IR/Code.h"
#include "IR/CodeContext.h"
#include "IR/ProcedureContext.h"
#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"
#include "Logger.h"
#include "ContextualMixin.h"

namespace SysYust::IR {

    /**
     * @brief 用于构建 Code 的混入类
     */
    class CodeBuildMixin : protected ContextualMixin {
        using Instruction = IR::instruction;
    protected:

        /**
         * @brief 初始化Code和上下文
         * @details 新建一个code对象和上下文，如果先前已经存在一个code对象，该对象将会被释放
         */
        void setup_code();
        /**
         * @brief 获取构造的 Code 对象
         */
        Code* getCode();
        /**
         * @brief 设置一个全局变量名字
         */
        void setGlobalVar(const var_symbol& sym);
        /**
         * @brief 检测该符号是否是全局变量
         */
         [[nodiscard]] bool is_global(const var_symbol &sym);

        // function //

        /**
         * @brief 进入一个函数
         */
        void entry_function(SysYust::IR::func_symbol name, SysYust::IR::func_info info);
        /**
         * @brief 获取当前函数
         */
        Procedure* current_procedure();

        /**
         * @brief 离开一个函数
         */
        void exit_function();

        /**
         * @brief RAII 的函数进出控制。请在具有明确块作用域的情况下使用
         */
        friend class function_guard;
        class function_guard {
        public:
            function_guard(CodeBuildMixin *context, func_symbol name, func_info info)
                : _context(*context)
                , _name(name)
            {
                _context.entry_function(name, info);
            }
            ~function_guard() {
                if (_context.current_procedure()->name() == _name) {
                    _context.exit_function();
                } else {
                    LOG_WARN("Function guard broken");
                }
            }
        private:
            CodeBuildMixin &_context;
            func_symbol _name;
        };


        // basic block && control flow graph //

        /**
         * @brief 在当前函数建立新的基本块。
         */
        BasicBlock* newBasicBlock();
        /**
         * @brief 创建一个新的基本块，并将它设置为当前基本块
         */
        void entry_block();
        /**
         * @brief 将特定的基本块设置为当前基本块
         */
        void entry_block(BasicBlock *blk);
        /**
         * @brief 进入后继节点
         * @details 如果后继节点不存在，则新建后继节点。
         */
        void entry_next();
        /**
         * @brief 进入否定后继节点
         * @details 进入否定后继节点不存在，则新建否定后继节点。
         */
        void entry_else();
        /**
         * @brief 将当前基本块入栈
         */
        void push_block();
        /**
         * @brief 将一个特定的基本块设置为当前基本块并入栈
         * @details 如果 blk 与当前基本块相同，那么仅会将当前基本块入栈
         */
        void save_and_entry(BasicBlock *blk);
        /**
         * @brief 将当前基本块保存并进入它的后继节点
         */
        void save_and_next();
        /**
         * @brief 将当前基本块保存并进入它的否定后继节点
         */
        void save_and_else();
        /**
         * @brief 回到上一个入栈的基本块
         */
        void pop_block();
        /**
         * @brief 获取当前基本块
         */
        [[nodiscard]] BasicBlock* current_block() const;
        /**
         * @brief 设置当前基本块的后继节点
         */
        void setNext(BasicBlock *blk);
        /**
         * @brief 设置当前基本块的否定后继节点
         */
        void setElse(BasicBlock *blk);

        /**
         * @brief 从当前函数上下文获取一个新的符号名字
         */
        [[nodiscard]] var_symbol nxt_sym();
        /**
         * @brief 从当前函数获取指定符号的下一个修订号的符号
         */
        [[nodiscard]] var_symbol nxt_sym(std::string sym);
        /**
         * @brief 从当前函数获取指定符号的下一个修订号的符号
         */
        [[nodiscard]] var_symbol nxt_sym(var_symbol sym);

        // instruction //

        /**
         * @brief 带有上下文信息的指令包装器
         */
        friend class ContextualInst;
        template<typename inst_t>
        class ContextualInst {
        public:

            /**
             * @brief 从当前上下文根据当前上下文创建一条命令
             */
            ContextualInst(inst_t aInst, CodeBuildMixin *context)
                : ContextualInst(aInst, context->_ir_code, context->current_procedure(), context->current_block())
            {

            }
            /**
             * @brief 根据给定句柄初始化上下文
             */
            ContextualInst(inst_t aInst, Code *global, Procedure *procedure, BasicBlock *basicBlock)
                : inst(aInst)
                , _global(global)
                , _global_context(&global->context)
                , _procedure(procedure)
                , _procedure_context(procedure->context())
                , _currentBlock(basicBlock)
            {

            }

            // 带有上下文的操作

            /**
             * @brief 将当前指令推出关联的基本块
             * @details 当当前基本块上下文为空引用时无行为。
             */
            void push() {
                if (_currentBlock) {
                    _currentBlock->push(inst);
                } else {
                    LOG_WARN("Try push a inst into nullptr basic block");
                }
            }

            // 解引用运算

            inst_t& operator* () {
                return inst;
            }
            const inst_t& operator* () const {
                return inst;
            }
            inst_t* operator-> () {
                return &inst;
            }
            const inst_t* operator-> () const {
                return &inst;
            }

            inst_t inst;

            // 上下文获取

            Code* code() {
                return _global;
            }
            Procedure* procedure() {
                return _procedure;
            }
            BasicBlock* block() {
                return _currentBlock;
            }


        private:
            Code *_global;
            CodeContext *_global_context;
            Procedure *_procedure;
            ProcedureContext *_procedure_context;
            BasicBlock* _currentBlock;
        };
        /**
         * @brief 构建一个新的指令，可以自动生成被赋值的
         * @return 一个被 ContextualInst 包装的指令对象
         */
        template<instruct_type t, typename... Args>
        auto auto_inst(Args&&... args) {
            constexpr auto ct = cate(t);
            if constexpr (ct == instruct_cate::with_1
            || ct == instruct_cate::with_2
            || ct == instruct_cate::index
            || ct == instruct_cate::alloc
            ) { // 上述几种类型需要自动生成符号
                assert(_procedure_context);
                if (_procedure_context)
                return ContextualInst(inst<t>(_procedure_context->nextSymbol(), std::forward<Args>(args)...), this);
            } else if constexpr (ct == instruct_cate::call) { // 处理调用的清理
                if (_procedure_context) {
                    auto rt_type = _code_context->get_fun_type(std::get<0>(std::forward_as_tuple(args...)));
                    if (rt_type->id() == Type::none) { // 返回类型未空的情况，返回值无意义，使用弃用的符号
                        auto rt_inst = inst<call>(_procedure_context->depressed_symbol, std::forward<Args>(args)...);
                        return ContextualInst(rt_inst, this);
                    } else { // 具有返回类型，设置符号的类型
                        auto nxtSym = _procedure_context->nextSymbol();
                        auto rt_inst = inst<call>(var_symbol{nxtSym.symbol, nxtSym.revision, rt_type},
                                                  std::forward<Args>(args)...);
                        return ContextualInst(rt_inst, this);
                    }
                } else {
                    LOG_ERROR("create a instruction with context that leak function");
                    __builtin_unreachable();
                }
            }
            else {
                return
                ContextualInst(inst<t>(std::forward<Args>(args)...), this);
            }
            __builtin_unreachable();
        }



    private:

        void reset();
        BasicBlock*& top_block();

    }; // CodeBuildMixin

} // IR

#endif //SYSYUST_CODEUTIL_H
