/// @file
/// Function 类型标识的实现

#include <utility>

#include "fmt/core.h"
#include "AST/Type/Function.h"

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
namespace views = ranges::views;

namespace SysYust::AST {
    const Type &Function::getResult() const {
        return _returnedType;
    }

    std::vector<const Type *> Function::getParam() const {
        return _paramType;
    }

    bool Function::match(const Function &rhs) const {
        bool rtEqual = _returnedType.match(rhs._returnedType);
        bool paramEqual = std::equal(_paramType.begin(), _paramType.end(), rhs._paramType.begin());
        return rtEqual && paramEqual;
    }

    Function::Function(const Type &returned, std::vector<const Type *> param)
    : _returnedType(returned)
    , _paramType(std::move(param)) {

    }

    bool Function::invokable(const std::vector<const Type *> &args) const {
        return args.size() == _paramType.size() &&
            std::equal(args.begin(), args.end(), _paramType.begin(),
                       [](const Type *lhs, const Type *rhs) {
                            return *lhs == *rhs;
                        });
    }

    std::set<Function> Function::_pool{};

    std::string Function::toString() const noexcept {
        std::string rt = "function(";
        auto resultType = getResult().toString();
        rt += getParam().front()->toString();
        auto params = getParam();
        for (auto &type : params | views::drop(1)) {
            rt += fmt::format(",{}", type->toString());
        }
        rt += ") -> ";
        rt += resultType;
        return rt;
    }
} // AST