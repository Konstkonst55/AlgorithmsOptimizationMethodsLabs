#ifndef LINEARSYSTEM_H
#define LINEARSYSTEM_H

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "Fraction.h"

class LinearSystem {
protected:
    std::vector<std::vector<Fraction>> matrix;
    int m;
    int n;

public:
    LinearSystem();
    void loadFromFile(const std::string& filename);
    void printMatrix() const;
    virtual void solveJordanGauss();
    void printGeneralSolution(const std::vector<int>& colToVar, int rank) const;
};

#endif