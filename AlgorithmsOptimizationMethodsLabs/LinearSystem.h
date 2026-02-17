#ifndef LINEARSYSTEM_H
#define LINEARSYSTEM_H

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
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
    void printMatrix(const std::vector<std::vector<Fraction>>& mat) const;
    void findAllBasisSolutions();

private:
    bool gaussElimination(std::vector<std::vector<Fraction>>& mat, std::vector<int>& basis, std::vector<int>& free);
    bool isSolutionValid(const std::vector<Fraction>& solution);
    void printSolution(const std::vector<Fraction>& solution, const std::vector<int>& basis, const std::vector<int>& free);
    bool isConsistent(const std::vector<std::vector<Fraction>>& mat);
    void generateBasisCombinations(std::vector<std::vector<int>>& combinations, std::vector<int>& current, int start, int k);
};

#endif