//
//  Calculator.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#ifndef Calculator_hpp
#define Calculator_hpp

#include "FlowProcessor.hpp"
#include "ComplexNumber.hpp"

struct Calculator {
    static double calculate(ComplexNumber& operand, Function& method) {
        switch (method) {
            case Function::MODULUS:
                return operand.modulus();
            case Function::ARGUMENT:
                return operand.argument();
        }
    };

    static double calculate(double& operand, Function& method) {
        ComplexNumber complex (operand, 0);
        return Calculator::calculate(complex, method);
    };

    static ComplexNumber calculate(std::pair<ComplexNumber, ComplexNumber> operands, BinaryComplexOperation& operation) {
        return operation(operands.first, operands.second);
    };

    static ComplexNumber calculate(std::pair<ComplexNumber, double> operands, BinaryComplexDoubleOperation& operation) {
        return operation(operands.first, operands.second);
    };
};

#endif /* Calculator_hpp */
