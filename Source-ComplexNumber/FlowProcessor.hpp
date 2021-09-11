//
//  FlowProcessor.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#ifndef FlowProcessor_hpp
#define FlowProcessor_hpp

#include <variant>
#include <functional>

#include "Flow.hpp"
#include "Result.hpp"
#include "ComplexNumber.hpp"
#include "Tokenizer.hpp"

enum class Function {
    MODULUS,
    ARGUMENT
};

enum class MenuItems {
    EXIT,
    TARGET
};


typedef std::function<ComplexNumber(ComplexNumber, double)> BinaryComplexDoubleOperation;
typedef std::function<ComplexNumber(ComplexNumber, ComplexNumber)> BinaryComplexOperation;
typedef std::variant<Function, BinaryComplexOperation, BinaryComplexDoubleOperation> OperationType;

struct FlowProcessor {
    FlowProcessor() {};
    Result<ComplexNumber> process(Flow<ComplexOperand> flow) const;
    Result<double> process(Flow<DoubleOperand> flow) const;
    Result<OperationType> process(Flow<Operation> flow) const;
    Result<MenuItems> process(Flow<Menu> flow) const;
};

#endif /* FlowProcessor_hpp */
