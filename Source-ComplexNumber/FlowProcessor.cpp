//
//  FlowProcessor.cpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#include <algorithm>
#include <iterator>
#include <map>
#include <functional>
#include <optional>
#include <numeric>

#include "FlowProcessor.hpp"

template<typename NumberType>
struct Number {
    double value;
    Number<NumberType>(double value): value(value) {};
};

struct Real {};
struct Imaginary {};

std::variant<Number<Real>, Number<Imaginary>> evaluate(std::string expression) {
    auto result = std::find(expression.begin(), expression.end(), 'i');
    if (result == std::end(expression)) {
        return Number<Real>(std::stod(expression));
    } else {
        expression.erase(result);
        if (expression == "-" || expression == "+") {
            expression.append("1");
        }
        return Number<Imaginary>(std::stod(expression));
    }
}

std::optional<ComplexNumber> processArgs(std::vector<TypedExpression<ComplexExpr>> args) {
    auto first = evaluate(args[0].expression);
    auto second = [args, first]() -> std::variant<Number<Real>, Number<Imaginary>> {
        if (args.size() == 1) {
            if (std::holds_alternative<Number<Real>>(first)) {
                return Number<Imaginary>(0.0);
            } else {
                return Number<Real>(0.0);
            }
        } else {
            return evaluate(args[1].expression);
        }
    }();

    if (std::holds_alternative<Number<Real>>(first) && std::holds_alternative<Number<Real>>(second)) {
        return std::nullopt;
    } else if (std::holds_alternative<Number<Imaginary>>(first) && std::holds_alternative<Number<Imaginary>>(second)) {
        return std::nullopt;
    } else {
        auto real = std::holds_alternative<Number<Imaginary>>(first) ? std::get<Number<Real>>(second) : std::get<Number<Real>>(first);
        auto imaginary = std::holds_alternative<Number<Real>>(first) ? std::get<Number<Imaginary>>(second) : std::get<Number<Imaginary>>(first);
        return ComplexNumber(real.value, imaginary.value);
    }
}

typedef std::function<bool(const Token&)> TokenTypeHandler;
typedef std::function<bool(const unsigned long)> VectorSizeHandler;

std::optional<std::vector<Token>> filter(std::vector<Token>& tokens, TokenTypeHandler targetTokenHandler, VectorSizeHandler sizeHandler) {
    if (tokens.empty()) {
        return std::nullopt;
    }

    bool hasError = false;

    auto filteredTokens = std::accumulate(tokens.begin(), tokens.end(), std::vector<Token>(), [targetTokenHandler, &hasError](std::vector<Token>& tokens, const Token& token) mutable {
        if (targetTokenHandler(token)) {
            tokens.push_back(token);
        } else if (!std::holds_alternative<TypedExpression<SpaceExpr>>(token)) {
            hasError = true;
        }

        return tokens;
    });

    if (!sizeHandler(filteredTokens.size()) || hasError) {
        return std::nullopt;
    } else {
        return std::make_optional(filteredTokens);
    }
}

Result<ComplexNumber> FlowProcessor::process(Flow<ComplexOperand> flow) const {
    auto tokenTypeHandler = [](const Token& token) { return std::holds_alternative<TypedExpression<ComplexExpr>>(token); };
    auto tokensSizeHandler = [](const unsigned long size) { return size > 0 && size <= 2; };
    auto filteredTokens = filter(flow.tokens, tokenTypeHandler, tokensSizeHandler);

    if (!filteredTokens.has_value()) {
        return Result<ComplexNumber>(Error("non valid arg"));
    }
    else {
        auto filteredTokensVec = filteredTokens.value();
        std::vector<TypedExpression<ComplexExpr>> args {};
        std::transform(filteredTokensVec.begin(), filteredTokensVec.end(), std::back_inserter(args), [](const Token token) -> TypedExpression<ComplexExpr> {
            return std::get<TypedExpression<ComplexExpr>>(token);
        });
        auto result = processArgs(args);
        return result.has_value() ? Result<ComplexNumber>(result.value()) : Result<ComplexNumber>(Error("non valid arg"));
    }
}

Result<MenuItems> FlowProcessor::process(Flow<Menu> flow) const {
    auto tokenTypeHandler = [](const Token& token) { return std::holds_alternative<TypedExpression<MenuExpr>>(token); };
    auto tokensSizeHandler = [](const unsigned long size) { return size == 1; };
    auto filteredTokens = filter(flow.tokens, tokenTypeHandler, tokensSizeHandler);

    if (!filteredTokens.has_value()) {
        return Result<MenuItems>(Error("non valid arg"));
    } else {
        auto menuItem = std::get<TypedExpression<MenuExpr>>(filteredTokens.value()[0]);
        auto menuItemInt = menuItem.expression == "A" ? 0 : 1;
        return Result<MenuItems>(static_cast<MenuItems>(menuItemInt));
    }
}

Result<OperationType> FlowProcessor::process(Flow<Operation> flow) const {
    std::function<bool(const Token&)> typeHandlerOperation = [](const Token& token) {
        return std::holds_alternative<TypedExpression<OperationExpr>>(token);
    };

    std::function<bool(const Token&)> typeHandlerFunction = [](const Token& token) {
        return std::holds_alternative<TypedExpression<FunctionExpr>>(token);
    };
    auto tokensSizeHandler = [](const unsigned long size) { return size == 1; };

    auto operationToken = filter(flow.tokens, typeHandlerOperation, tokensSizeHandler);
    auto functionToken = filter(flow.tokens, typeHandlerFunction, tokensSizeHandler);

    if (operationToken.has_value()) {
        auto expr = std::get<TypedExpression<OperationExpr>>(operationToken.value()[0]).expression;
        if (expr == "+") {
            return Result<OperationType>(BinaryComplexOperation([](ComplexNumber first, ComplexNumber second) { return first + second; }));
        } else if (expr == "-") {
            return Result<OperationType>(BinaryComplexOperation([](ComplexNumber first, ComplexNumber second) { return first - second; }));
        } else if (expr == "/") {
            return Result<OperationType>(BinaryComplexDoubleOperation([](ComplexNumber first, double second) { return first / second; }));
        } else {
            return Result<OperationType>(BinaryComplexDoubleOperation([](ComplexNumber first, double second) { return first * second; }));
        }
    } else if (functionToken.has_value()) {
        auto expr = std::get<TypedExpression<FunctionExpr>>(functionToken.value()[0]).expression;
        if (expr == "modulus") {
            return Result<OperationType>(Function::MODULUS);
        } else {
            return Result<OperationType>(Function::ARGUMENT);
        }
    } else {
        return Result<OperationType>(Error("non valid arg"));
    }
}

Result<double> FlowProcessor::process(Flow<DoubleOperand> flow) const {
    auto tokenTypeHandler = [](const Token& token) { return std::holds_alternative<TypedExpression<ComplexExpr>>(token); };
    auto tokensSizeHandler = [](const unsigned long size) { return size == 1; };
    auto filteredTokens = filter(flow.tokens, tokenTypeHandler, tokensSizeHandler);

    if (filteredTokens.has_value()) {
        auto expr = std::get<TypedExpression<ComplexExpr>>(filteredTokens.value()[0]).expression;
        auto number = evaluate(expr);
        if (std::holds_alternative<Number<Real>>(number)) {
            auto doubleNumber = std::get<Number<Real>>(number).value;
            return Result<double>(doubleNumber);
        }
    }
    return Result<double>(Error("non-valid arg"));
}





