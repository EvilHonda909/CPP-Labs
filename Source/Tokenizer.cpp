//
//  Tokenizer.cpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 03.09.2021.
//

#include "Tokenizer.hpp"
#include "TypedExpression.hpp"

#include <regex>
#include <numeric>
#include <iostream>

// MARK: - Basic building blocks

std::string signRe = "[-+]?";
std::string integerRe = "[1-9]\\d*";
std::string iRe = "i-i+i";
std::string spacesRe = " +";
std::string orRe = "|";
std::string numberRe = "[-+]?i|[-+]?i?((0|[1-9]\\d*)|(0|[1-9]\\d*)\\.\\d*)";
std::string signedNumberRe = signRe + numberRe;
std::string menuSignsRe = "A|B";


// MARK: - Posible lexemes

std::regex complexRe = std::regex(numberRe, std::regex::icase);
std::regex functionRe = std::regex("modulus" + orRe + "arg", std::regex::icase);
std::regex operationRe = std::regex("[\\+\\-*\\/]", std::regex::icase);
std::regex spaceRe = std::regex(spacesRe, std::regex::icase);
std::regex menuRe = std::regex(menuSignsRe, std::regex::icase);

bool isComplex(std::string& expression) {
    return std::regex_match(expression, complexRe);
}

bool isOperator(std::string& expression) {
    return std::regex_match(expression, operationRe);
}

bool isFunction(std::string& expression) {
    return std::regex_match(expression, functionRe);
}

bool isSpace(std::string& expression) {
    return std::regex_match(expression, spaceRe);
}

bool isMenu(std::string& expression) {
    return std::regex_match(expression, menuRe);
}

std::optional<Token> getNextToken(std::string::iterator& begin, std::string::iterator& end) {
    std::string expression (begin, end);

    if (isSpace(expression)) {
        return std::make_optional(TypedExpression<SpaceExpr> (expression));
    } else if (isComplex(expression)) {
        return std::make_optional(TypedExpression<ComplexExpr> (expression));
    } else if (isFunction(expression)) {
        return std::make_optional(TypedExpression<FunctionExpr> (expression));
    } else if (isOperator(expression)) {
        return std::make_optional(TypedExpression<OperationExpr> (expression));
    } else if (isMenu(expression)) {
        return std::make_optional(TypedExpression<MenuExpr> (expression));
    } else {
        return std::nullopt;
    }
}

bool shouldPushBack(std::optional<Token> token, std::optional<Token> nextToken) {
    return token.has_value() && !nextToken.has_value();
}

std::vector<Token> Tokenizer::tokenize(std::string& input) {
    std::string::iterator bufferBegin = input.begin();
    std::string::iterator bufferEnd = bufferBegin + 1;
    std::optional<Token> token = std::nullopt;
    std::vector<Token> tokens {};

    while (bufferEnd <= input.end()) {
        auto nextToken = getNextToken(bufferBegin, bufferEnd);
        if (shouldPushBack(token, nextToken)) {
            bufferBegin = bufferEnd - 1;
            tokens.push_back(token.value());
        } else {
            bufferEnd += 1;
        }
        token = nextToken;
    }

    if (!token.has_value()) {
        auto expr = std::string(bufferBegin, input.end());
        token = TypedExpression<ErrorExpr>(expr);
    }

    tokens.push_back(token.value());

    return tokens;
}
