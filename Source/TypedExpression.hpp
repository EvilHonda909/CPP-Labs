//
//  TypedExpression.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 03.09.2021.
//

#ifndef TypedExpression_hpp
#define TypedExpression_hpp

#include <string>
#include <optional>

template <typename StringType>
struct TypedExpression {
    std::string expression;
    TypedExpression(std::string& expression) {
        this->expression = expression;
    }
};

struct ComplexExpr {};
struct OperationExpr {};
struct FunctionExpr {};
struct SpaceExpr {};
struct MenuExpr {};
struct ErrorExpr {};

#endif /* TypedExpression_hpp */
