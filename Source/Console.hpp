//
//  Console.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#ifndef Console_hpp
#define Console_hpp

#include <vector>
#include <variant>
#include <optional>

#include "ComplexNumber.hpp"
#include "Tokenizer.hpp"
#include "Flow.hpp"
#include "FlowProcessor.hpp"
#include "Calculator.hpp"

struct StateInfo {
    virtual std::string getInfo() = 0;
};

const std::string greeting = "Hey, this is binary calculator working with complex numbers";
const std::string realGrammarInfo = "The real part of complex number is an integer value";
const std::string imaginaryGrammarInfo = "The imaginary part of complex number is an \"i\" letter following by integer value";
const std::string examplesInfo = "Examples: \"2+i2\", \"-i2 -2.222\", \"2.222\", \"-i1.2\"";
const std::string binaryOperationInfo = "You can type these operations: \"+\", \"-\", \"*\", \"/\"";
const std::string methodInfo = "You can type these methods: \"modulus\", \"arg\"";
const std::string resultInfo = "Your result: ";

std::string makeOperandInfo() {
    return realGrammarInfo + "\n" + imaginaryGrammarInfo + "\n" + examplesInfo + "\n" + "Type complex number: ";
}

std::string getMenu(std::string target) {
    return "Menu:\n A - Exit\n B - " + target + "\n Type: ";
}

struct Idle: StateInfo {
    std::string getInfo() override {
        return greeting + "\n" + getMenu("First operand") + "\n";
    }
};

struct FirstOperand: StateInfo {
    std::string getInfo() override {
        return makeOperandInfo();
    }
};
struct Operator: StateInfo {
    ComplexNumber firstOperand;
    Operator(ComplexNumber firstOperand): firstOperand(firstOperand) {};

    std::string getInfo() override {
        return binaryOperationInfo + "\n" + methodInfo + "\n" + "Type in order to go on the flow: ";
    }
};

struct SecondDoubleOperand: StateInfo {
    ComplexNumber firstOperand;
    BinaryComplexDoubleOperation operation;

    SecondDoubleOperand(ComplexNumber firstOperand, BinaryComplexDoubleOperation operation): firstOperand(firstOperand), operation(operation) {};

    std::string getInfo() override {
        return "You chose operation which second operand should be double.\nPlease, type double number: ";
    }
};

struct SecondOperand: StateInfo {
    ComplexNumber firstOperand;
    BinaryComplexOperation operation;

    SecondOperand(ComplexNumber firstOperand, BinaryComplexOperation operation): firstOperand(firstOperand), operation(operation) {};

    std::string getInfo() override {
        return makeOperandInfo();
    }
};

struct MethodResult: StateInfo {
    ComplexNumber operand;
    Function method;
    MethodResult(ComplexNumber operand, Function method): operand(operand), method(method) {};

    std::string getInfo() override {
        auto result = Calculator::calculate(operand, method);
        return resultInfo + std::to_string(result) + "\n" + getMenu("First operand") + "\n";
    }
};

typedef std::pair<ComplexNumber, ComplexNumber> ComplexOperands;

struct BinaryComplexResult: StateInfo {
    ComplexOperands operands;
    BinaryComplexOperation operation;

    BinaryComplexResult(ComplexOperands operands, BinaryComplexOperation operation): operands(operands), operation(operation) {};

    std::string getInfo() override {
        auto result = Calculator::calculate(operands, operation).to_string();
        return resultInfo + result + "\n" + getMenu("First operand") + "\n";
    }
};

typedef std::pair<ComplexNumber, double> MixedOperands;

struct BinaryComplexDoubleResult: StateInfo {
    MixedOperands operands;
    BinaryComplexDoubleOperation operation;

    BinaryComplexDoubleResult(MixedOperands operands, BinaryComplexDoubleOperation operation): operands(operands), operation(operation) {};

    std::string getInfo() override {
        auto result = Calculator::calculate(operands, operation).to_string();
        return resultInfo + result + "\n" + getMenu("First operand") + "\n";
    }
};

struct ErrorResult: StateInfo {
    std::string description;
    ErrorResult(std::string description): description(description) {};

    std::string getInfo() override {
        return "Error:" + description + "\n" + getMenu("First operand") + "\n";
    }
};
struct End: StateInfo {
    std::string getInfo() override {
        return "Bye\n";
    }
};

typedef std::variant<
    Idle,
    FirstOperand,
    Operator,
    SecondOperand,
    SecondDoubleOperand,
    MethodResult,
    BinaryComplexResult,
    BinaryComplexDoubleResult,
    ErrorResult,
    End
> ConsoleState;

struct NewTokensEvent {
    std::vector<Token> tokens;
    FlowProcessor processor = FlowProcessor();

    NewTokensEvent(std::vector<Token>& tokens): tokens(tokens) {};

    ConsoleState operator()(const Idle&) {
        Flow<Menu> flowItem (tokens);
        std::optional<MenuItems> menuItem = processor.process(flowItem).success();

        if (menuItem.has_value()) {
            switch (menuItem.value()) {
                case MenuItems::EXIT:
                    return End();
                case MenuItems::TARGET:
                    return FirstOperand();
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const FirstOperand&) {
        Flow<ComplexOperand> flowItem (tokens);
        auto number = processor.process(flowItem).success();

        if (number.has_value()) {
            return Operator(number.value());
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const Operator& state) {
        Flow<Operation> flowItem (tokens);
        std::optional<OperationType> operation = processor.process(flowItem).success();

        if (operation.has_value()) {
            OperationType operationVariant = operation.value();
            if (std::holds_alternative<Function>(operationVariant)) {
                return MethodResult(state.firstOperand, std::get<Function>(operationVariant));
            } else if (std::holds_alternative<BinaryComplexOperation>(operationVariant)) {
                return SecondOperand(state.firstOperand, std::get<BinaryComplexOperation>(operationVariant));
            } else {
                return SecondDoubleOperand(state.firstOperand, std::get<BinaryComplexDoubleOperation>(operationVariant));
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const SecondOperand& state) {
        Flow<ComplexOperand> flowItem (tokens);
        auto number = processor.process(flowItem).success();

        if (number.has_value()) {
            return BinaryComplexResult(ComplexOperands (state.firstOperand, number.value()), state.operation);
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const SecondDoubleOperand& state) {
        Flow<DoubleOperand> flowItem (tokens);
        auto doubleValue = processor.process(flowItem).success();

        if (doubleValue.has_value()) {
            return BinaryComplexDoubleResult(MixedOperands (state.firstOperand, doubleValue.value()), state.operation);
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const BinaryComplexResult&) {
        Flow<Menu> flowItem (tokens);
        std::optional<MenuItems> menuItem = processor.process(flowItem).success();

        if (menuItem.has_value()) {
            switch (menuItem.value()) {
                case MenuItems::EXIT:
                    return End();
                case MenuItems::TARGET:
                    return FirstOperand();
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const BinaryComplexDoubleResult&) {
        Flow<Menu> flowItem (tokens);
        std::optional<MenuItems> menuItem = processor.process(flowItem).success();

        if (menuItem.has_value()) {
            switch (menuItem.value()) {
                case MenuItems::EXIT:
                    return End();
                case MenuItems::TARGET:
                    return FirstOperand();
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const MethodResult&) {
        Flow<Menu> flowItem (tokens);
        std::optional<MenuItems> menuItem = processor.process(flowItem).success();

        if (menuItem.has_value()) {
            switch (menuItem.value()) {
                case MenuItems::EXIT:
                    return End();
                case MenuItems::TARGET:
                    return FirstOperand();
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const ErrorResult&) {
        Flow<Menu> flowItem (tokens);
        std::optional<MenuItems> menuItem = processor.process(flowItem).success();

        if (menuItem.has_value()) {
            switch (menuItem.value()) {
                case MenuItems::EXIT:
                    return End();
                case MenuItems::TARGET:
                    return FirstOperand();
            }
        } else {
            return ErrorResult("Invalid Input");
        }
    }

    ConsoleState operator()(const End&) {
        return End();
    }

};

class Console {
private:
    Tokenizer tokenizer = Tokenizer();
    ConsoleState state = Idle();
    std::string buffer;
public:
    void start() {
        while (true) {
            std::visit([](StateInfo& informator) { std::cout << informator.getInfo(); }, state);
            if (std::holds_alternative<End>(state)) {
                return;
            }
            getline(std::cin, buffer);
            auto tokens = tokenizer.tokenize(buffer);
            auto event = NewTokensEvent(tokens);
            state = std::visit(event, state);
        }
    }
};

#endif /* Console_hpp */
