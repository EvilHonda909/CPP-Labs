//
//  ComplexNumber.cpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 21.04.2021.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

using namespace std;

#include "ComplexNumber.hpp"

// MARK: - Constructors

ComplexNumber::ComplexNumber(double real, double imaginary) {
    this->real = real;
    this->imaginary = imaginary;
}
ComplexNumber::ComplexNumber(double real) {
    this->real = real;
    this->imaginary = 0;
}

ComplexNumber::ComplexNumber(const ComplexNumber& complex) {
    this->real = complex.real;
    this->imaginary = complex.imaginary;
}

// MARK: - Getters

double ComplexNumber::getReal() {
    return real;
}
double ComplexNumber::getImaginary() {
    return imaginary;
}

// MARK: - Special Math Operations For Complex Numbers

double ComplexNumber::modulus() {
    return sqrt(real*real + imaginary*imaginary);
}

double ComplexNumber::argument() {
    const double ratio = (double)imaginary/real;

    if (real > 0) {
        return atan(ratio);
    } else if (real < 0 && imaginary >= 0) {
        return atan(ratio) + M_PI;
    } else if (real < 0 && imaginary < 0) {
        return atan(ratio) - M_PI;
    } else if (real == 0 && imaginary > 0) {
        return M_PI_2;
    } else if (real == 0 && imaginary < 0) {
        return -M_PI_2;
    } else {
        return NAN;
    }
}

// MARK: - Arithmetic Operations

void ComplexNumber::operator+=(const ComplexNumber complex) {
    real += complex.real;
    imaginary += complex.imaginary;
}
void ComplexNumber::operator-=(ComplexNumber complex) {
    real -= complex.real;
    imaginary -= complex.imaginary;
}
ComplexNumber ComplexNumber::operator+(const ComplexNumber secondTerm) {
    return ComplexNumber(real + secondTerm.real, imaginary + secondTerm.imaginary);
}
ComplexNumber ComplexNumber::operator-(const ComplexNumber secondTerm) {
    return ComplexNumber(real - secondTerm.real, imaginary - secondTerm.imaginary);
}

ComplexNumber ComplexNumber::operator++() {
    real += 1;
    imaginary += 1;

    return ComplexNumber(real + 1, imaginary + 1);
}

ComplexNumber ComplexNumber::operator++(int) {
    real += 1;
    imaginary += 1;

    return ComplexNumber(real, imaginary);
}

// MARK: - Logic Operation

bool ComplexNumber::operator==(const ComplexNumber secondOperand) {
    return real == secondOperand.real && imaginary == secondOperand.imaginary;
}

// MARK: Implicit Coercion from Integer And Double

ComplexNumber& ComplexNumber::operator=(const int realInteger) {
    real = realInteger;
    imaginary = 0.0;

    return *this;
}

ComplexNumber& ComplexNumber::operator=(const double realFloating) {
    real = realFloating;
    imaginary = 0.0;

    return *this;
}

// MARK: - Coercion To Double

ComplexNumber::operator double() {
    return real;
}

// MARK: - Additional Arithmetic Operations Supporting Double Type

ComplexNumber operator+(const ComplexNumber complex, const double real) {
    return ComplexNumber(complex.real + real, complex.imaginary);
}

ComplexNumber operator-(const ComplexNumber complex, const double real) {
    return ComplexNumber(complex.real - real, complex.imaginary);
}

ComplexNumber operator*(const ComplexNumber complex, const double real) {
    return ComplexNumber(complex.real*real, complex.imaginary*real);
}

ComplexNumber operator/(const ComplexNumber complex, const double real) {
    return ComplexNumber(complex.real/real, complex.imaginary/real);
}

// MARK: - I/O Operations

istream & operator >> (istream &in,  ComplexNumber &complex) {
    cout << "Enter Real Part ";
    in >> complex.real;
    cout << "Enter Imaginary Part ";
    in >> complex.imaginary;

    return in;
}

ostream & operator << (ostream &out, const ComplexNumber &complex) {
    out << complex.real;
    out << "+i" << complex.imaginary << endl;
    
    return out;
}


