//
//  ComplexNumber.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 21.04.2021.
//

#ifndef ComplexNumber_hpp
#define ComplexNumber_hpp

#include <stdio.h>
#include <iostream>

enum Operations {
    MODULUS,
    ARGUMENT,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUALITY,
    INCREMENT,
};

class ComplexNumber {
private:
    double real;
    double imaginary;
public:
    double getReal();
    double getImaginary();

    ComplexNumber(const double real, const double imaginary);
    ComplexNumber(const double real);
    ComplexNumber(const ComplexNumber& complex);

    double modulus();
    double argument();

    friend ComplexNumber operator+(const ComplexNumber complex, const double real);
    friend ComplexNumber operator-(const ComplexNumber complex, const double real);
    friend ComplexNumber operator*(const ComplexNumber complex, const double real);
    friend ComplexNumber operator/(const ComplexNumber complex, const double real);

    void operator+=(const ComplexNumber complex);
    void operator-=(ComplexNumber complex);
    ComplexNumber operator+(const ComplexNumber secondTerm);
    ComplexNumber operator-(const ComplexNumber secondTerm);

    bool operator==(const ComplexNumber secondOperand);

    ComplexNumber operator++();
    ComplexNumber operator++(int);

    friend std::ostream & operator << (std::ostream &out, const ComplexNumber &complex);
    friend std::istream & operator >> (std::istream &in,  ComplexNumber &complex);

    ComplexNumber& operator=(const int realInteger);
    ComplexNumber& operator=(const double realFloating);

    operator double();

    std::string to_string() {
        auto imaginaryString = (imaginary < 0) ? std::to_string(imaginary).substr(1) : std::to_string(imaginary);
        auto imaginaryPart = (imaginary < 0) ? "-i" : "+i";

        return std::to_string(real) + imaginaryPart + imaginaryString;
    }
};

#endif /* ComplexNumber_hpp */
