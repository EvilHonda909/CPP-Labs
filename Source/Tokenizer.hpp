//
//  Tokenizer.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 03.09.2021.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <string>
#include <vector>
#include <optional>
#include <variant>

#include "TypedExpression.hpp"

typedef std::variant<
    TypedExpression<ComplexExpr>,
    TypedExpression<OperationExpr>,
    TypedExpression<FunctionExpr>,
    TypedExpression<SpaceExpr>,
    TypedExpression<MenuExpr>,
    TypedExpression<ErrorExpr>
>
Token;

struct Tokenizer {
    std::vector<Token> tokenize(std::string& input);
};

#endif /* Tokenizer_hpp */
