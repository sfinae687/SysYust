/// @file Function 类型标识的定义

#ifndef SYSYUST_AST_FUNCTION_H
#define SYSYUST_AST_FUNCTION_H

#include <vector>

#include "AST/TypeBase.h"
#include "AST/Int.h"

namespace SysYust::AST {

    class Function: public TypeBase<Function> {
    public:
        using paramatreList_t = std::vector<const Type *>;

        /**
         * @brief 通过编译时不可知的Type设置Function类型标识
         * @param returned 返回类型标识，只能是 Int 或 Float，由主调方检查
         * @param param 参数类型标识的 vector，每个元素必须是可以作为参数的类型，由主调方检查。
         */
        Function(const Type &returned, std::vector<const Type*> param);

        /**
         * @brief 通过编译时可知的返回值类型和参数类型列表构造 Function
         * @bug 虽然提供了这个构造函数，但是根据C++的重载决议，这些编译时检查的版本应该竞争不过非模板的版本。
         */
        template<typename R, typename... Args,
            std::enable_if_t<
                TypeTrait<R>::isReturnedType &&
                (TypeTrait<Args>::isParamType && ...)
            , bool> = false
        >
        explicit Function(const R& returned, const Args&... args)
        : _returnedType(returned)
        , _paramType({&args...}) {

        }

        /**
         * @brief 通过返回类型标识和 vector 类型标识列表构造 Function
         */
        template<typename R, std::enable_if_t<TypeTrait<R>::isReturnedType, bool> = false>
        Function(const R& returned, std::vector<const Type*> args)
        : _returnedType(returned)
        , _paramType(std::move(args)) {

        }

        /**
         * @brief 获取函数类型的返回类型
         */
        [[nodiscard]] const Type &getResult() const;

        /**
         * @brief 获取函数的参数类型列表
         */
         [[nodiscard]] std::vector<const Type*> getParam() const;

         [[nodiscard]] bool match(const SysYust::AST::Function &) const override;

         [[nodiscard]] bool invokable(const std::vector<const Type*> &args) const;

    private:
        const Type &_returnedType; ///< 返回类型
        const std::vector<const Type*> _paramType; ///< 参数列表类型
    };
} // AST

#endif //SYSYUST_AST_FUNCTION_H
