#include "Fraction.h"

void Fraction::simplify() {
    if (den < 0) {
        num = -num;
        den = -den;
    }

    long long common = std::gcd(std::abs(num), std::abs(den));
    if (common != 0) {
        num /= common;
        den /= common;
    }
}

Fraction::Fraction(long long n, long long d) : num(n), den(d) {
    if (den == 0) {
        den = 1;
    }

    simplify();
}

Fraction Fraction::operator+(const Fraction& other) const {
    return Fraction(num * other.den + other.num * den, den * other.den);
}

Fraction Fraction::operator-(const Fraction& other) const {
    return Fraction(num * other.den - other.num * den, den * other.den);
}

Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(num * other.num, den * other.den);
}

Fraction Fraction::operator/(const Fraction& other) const {
    return Fraction(num * other.den, den * other.num);
}

bool Fraction::operator==(const Fraction& other) const {
    return (num == other.num && den == other.den);
}

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}

bool Fraction::operator>(const Fraction& other) const {
    return num * other.den > other.num * den;
}

bool Fraction::operator<(const Fraction& other) const {
    return num * other.den < other.num * den;
}

bool Fraction::operator==(long long value) const {
    return num == value && den == 1;
}

bool Fraction::operator!=(long long value) const {
    return !(*this == value);
}

Fraction Fraction::abs() const {
    return Fraction(std::abs(num), den);
}

bool Fraction::isZero() const {
    return num == 0;
}

long long Fraction::getNum() const {
    return num;
}

long long Fraction::getDen() const {
    return den;
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    if (f.den == 1) {
        os << f.num;
    }
    else {
        os << f.num << "/" << f.den;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Fraction& f) {
    long long n;
    is >> n;
    f = Fraction(n);

    return is;
}