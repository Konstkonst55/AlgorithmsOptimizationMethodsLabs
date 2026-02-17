#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <numeric>
#include <cmath>

class Fraction {
private:
    long long num;
    long long den;
    void simplify();

public:
    Fraction(long long n = 0, long long d = 1);

    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator==(long long value) const;
    bool operator!=(long long value) const;

    Fraction abs() const;
    bool isZero() const;
    long long getNum() const;
    long long getDen() const;

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::istream& operator>>(std::istream& is, Fraction& f);
};

#endif