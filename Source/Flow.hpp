//
//  Flow.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#ifndef Flow_hpp
#define Flow_hpp

#include <vector>

#include "Tokenizer.hpp"

template <typename Step>
struct Flow {
    std::vector<Token> tokens;

    Flow(std::vector<Token>& tokens) {
        this->tokens = tokens;
    }
};

struct ComplexOperand {};
struct DoubleOperand {};
struct Operation {};
struct Menu {};

#endif /* Flow_hpp */
