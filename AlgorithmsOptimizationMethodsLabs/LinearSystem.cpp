#include "LinearSystem.h"
#include <algorithm>
#include <iostream>
#include <sstream>

LinearSystem::LinearSystem() : m(0), n(0) {}

void LinearSystem::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        file >> m >> n;
        matrix.resize(m, std::vector<Fraction>(n + 1));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j <= n; j++) {
                file >> matrix[i][j];
            }
        }

        file.close();
        std::cout << "System loaded: " << m << " equations, " << n << " variables" << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void LinearSystem::printMatrix() const {
    std::vector<int> colWidths(n + 1, 0);

    for (int j = 0; j <= n; j++) {
        for (int i = 0; i < m; i++) {
            std::stringstream ss;
            ss << matrix[i][j];
            int width = ss.str().length();

            if (width > colWidths[j]) {
                colWidths[j] = width;
            }
        }
    }

    for (int i = 0; i < m; i++) {
        std::cout << "| ";

        for (int j = 0; j <= n; j++) {
            if (j == n) std::cout << "| ";

            std::stringstream ss;
            ss << matrix[i][j];
            std::string str = ss.str();

            int padding = colWidths[j] - str.length();

            for (int p = 0; p < padding; p++) {
                std::cout << " ";
            }

            std::cout << str << " ";
        }

        std::cout << "|" << std::endl;
    }

    std::cout << std::endl;
}

void LinearSystem::printMatrix(const std::vector<std::vector<Fraction>>& mat) const {
    if (mat.empty()) return;

    int rows = mat.size();
    int cols = mat[0].size();

    std::vector<int> colWidths(cols, 0);

    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            std::stringstream ss;
            ss << mat[i][j];
            int width = ss.str().length();

            if (width > colWidths[j]) {
                colWidths[j] = width;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        std::cout << "| ";

        for (int j = 0; j < cols; j++) {
            if (j == cols - 1) std::cout << "| ";

            std::stringstream ss;
            ss << mat[i][j];
            std::string str = ss.str();

            int padding = colWidths[j] - str.length();

            for (int p = 0; p < padding; p++) {
                std::cout << " ";
            }

            std::cout << str << " ";
        }

        std::cout << "|" << std::endl;
    }

    std::cout << std::endl;
}

bool LinearSystem::isConsistent(const std::vector<std::vector<Fraction>>& mat) {
    for (size_t i = 0; i < mat.size(); i++) {
        bool allZeros = true;

        for (size_t j = 0; j < mat[i].size() - 1; j++) {
            if (mat[i][j] != Fraction(0)) {
                allZeros = false;
                break;
            }
        }

        if (allZeros && mat[i].back() != Fraction(0)) {
            return false;
        }
    }

    return true;
}

bool LinearSystem::gaussElimination(std::vector<std::vector<Fraction>>& mat, std::vector<int>& basis, std::vector<int>& free) {
    int rows = mat.size();
    int cols = mat[0].size() - 1;

    basis.clear();
    free.clear();

    std::cout << "Starting Gaussian elimination:" << std::endl;

    std::vector<bool> colUsed(cols, false);
    int pivotRow = 0;

    for (int col = 0; col < cols && pivotRow < rows; col++) {
        int pivot = -1;

        for (int i = pivotRow; i < rows; i++) {
            if (mat[i][col] != Fraction(0)) {
                pivot = i;
                break;
            }
        }

        if (pivot != -1) {
            if (pivot != pivotRow) {
                std::swap(mat[pivot], mat[pivotRow]);
                std::cout << "Swap row " << pivot + 1 << " with row " << pivotRow + 1 << std::endl;
            }

            Fraction divisor = mat[pivotRow][col];

            for (int j = col; j <= cols; j++) {
                mat[pivotRow][j] = mat[pivotRow][j] / divisor;
            }

            std::cout << "Normalize row " << pivotRow + 1 << " using pivot in column " << col + 1 << std::endl;
            printMatrix(mat);

            for (int i = 0; i < rows; i++) {
                if (i != pivotRow && mat[i][col] != Fraction(0)) {
                    Fraction factor = mat[i][col];

                    for (int j = col; j <= cols; j++) {
                        mat[i][j] = mat[i][j] - (factor * mat[pivotRow][j]);
                    }

                    std::cout << "Eliminate column " << col + 1 << " from row " << i + 1 << std::endl;
                    printMatrix(mat);
                }
            }

            basis.push_back(col);
            colUsed[col] = true;
            pivotRow++;
        }
    }

    for (int col = 0; col < cols; col++) {
        if (!colUsed[col]) {
            free.push_back(col);
        }
    }

    return isConsistent(mat);
}

void LinearSystem::generateBasisCombinations(std::vector<std::vector<int>>& combinations, std::vector<int>& current, int start, int k) {
    if (k == 0) {
        combinations.push_back(current);

        return;
    }

    for (int i = start; i <= n - k; i++) {
        current.push_back(i);
        generateBasisCombinations(combinations, current, i + 1, k - 1);
        current.pop_back();
    }
}

bool LinearSystem::isSolutionValid(const std::vector<Fraction>& solution) {
    for (int i = 0; i < m; i++) {
        Fraction sum(0);

        for (int j = 0; j < n; j++) {
            sum = sum + matrix[i][j] * solution[j];
        }

        if (sum != matrix[i][n]) {
            return false;
        }
    }

    return true;
}

void LinearSystem::printSolution(const std::vector<Fraction>& solution, const std::vector<int>& basis, const std::vector<int>& free) {
    std::cout << "Basis solution:" << std::endl;
    std::cout << "Basis variables: ";

    for (int b : basis) {
        std::cout << "x" << b + 1 << " ";
    }

    std::cout << std::endl << "Free variables: ";
    
    for (int f : free) {
        std::cout << "x" << f + 1 << " ";
    }

    std::cout << std::endl << std::endl << "Solution:" << std::endl;

    for (int i = 0; i < n; i++) {
        std::cout << "x" << i + 1 << " = " << solution[i] << std::endl;
    }

    std::cout << std::endl;
}

void LinearSystem::findAllBasisSolutions() {
    std::cout << "Original matrix:" << std::endl;
    printMatrix();

    std::vector<std::vector<Fraction>> tempMatrix = matrix;
    std::vector<int> tempBasis, tempFree;

    if (!gaussElimination(tempMatrix, tempBasis, tempFree)) {
        std::cout << "System is inconsistent! No solutions." << std::endl;
        
        return;
    }

    int rank = tempBasis.size();
    std::cout << "System rank: " << rank << std::endl;

    if (rank < n) {
        std::cout << "System has " << n - rank << " degrees of freedom" << std::endl;
    }

    if (rank == n && m >= n) {
        std::cout << "\nSystem has a UNIQUE solution:" << std::endl;
        std::vector<Fraction> solution(n);
        
        for (int i = 0; i < n; i++) {
            solution[i] = tempMatrix[i][n];
        }
        
        printSolution(solution, tempBasis, tempFree);
        
        return;
    }

    std::vector<std::vector<int>> basisCombinations;
    std::vector<int> current;
    generateBasisCombinations(basisCombinations, current, 0, rank);

    std::cout << "\nTrying " << basisCombinations.size() << " possible basis combinations:" << std::endl;

    std::vector<std::vector<Fraction>> allSolutions;

    for (const auto& basisCombo : basisCombinations) {
        std::cout << "Trying basis: ";
        
        for (int b : basisCombo) {
            std::cout << "x" << b + 1 << " ";
        }
        
        std::cout << std::endl;

        std::vector<std::vector<Fraction>> workMatrix(m, std::vector<Fraction>(n + 1));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j <= n; j++) {
                workMatrix[i][j] = matrix[i][j];
            }
        }

        std::vector<int> freeVars;

        for (int j = 0; j < n; j++) {
            if (std::find(basisCombo.begin(), basisCombo.end(), j) == basisCombo.end()) {
                freeVars.push_back(j);
            }
        }

        std::vector<std::vector<Fraction>> reordered(m, std::vector<Fraction>(n + 1));

        for (size_t idx = 0; idx < basisCombo.size(); idx++) {
            int col = basisCombo[idx];
            
            for (int i = 0; i < m; i++) {
                reordered[i][idx] = matrix[i][col];
            }
        }

        for (size_t idx = 0; idx < freeVars.size(); idx++) {
            int col = freeVars[idx];
        
            for (int i = 0; i < m; i++) {
                reordered[i][basisCombo.size() + idx] = matrix[i][col];
            }
        }

        for (int i = 0; i < m; i++) {
            reordered[i][n] = matrix[i][n];
        }

        std::vector<int> currentBasis, currentFree;

        if (gaussElimination(reordered, currentBasis, currentFree)) {
            bool validBasis = true;
        
            for (size_t i = 0; i < basisCombo.size(); i++) {
                if (i < reordered.size() && reordered[i][i] != Fraction(1)) {
                    validBasis = false;
                    break;
                }
            }

            if (validBasis && currentBasis.size() == rank) {
                std::cout << "Valid basis found!" << std::endl;

                std::vector<Fraction> solution(n, Fraction(0));

                for (int f : freeVars) {
                    solution[f] = Fraction(0);
                }

                for (size_t i = 0; i < basisCombo.size() && i < reordered.size(); i++) {
                    solution[basisCombo[i]] = reordered[i][n];
                }

                if (isSolutionValid(solution)) {
                    bool isNew = true;
            
                    for (const auto& existing : allSolutions) {
                        bool same = true;
                    
                        for (int k = 0; k < n; k++) {
                            if (existing[k] != solution[k]) {
                                same = false;
                                break;
                            }
                        }

                        if (same) {
                            isNew = false;
                            break;
                        }
                    }

                    if (isNew) {
                        allSolutions.push_back(solution);
                        std::cout << std::endl << "Found basis solution #" << allSolutions.size() << std::endl;
                        printSolution(solution, basisCombo, freeVars);
                    }
                }
            }
        }
    }

    std::cout << "Results: Found " << allSolutions.size() << " basis solutions" << std::endl;

    if (allSolutions.empty()) {
        std::cout << "No basis solutions found." << std::endl;
    }
    else {
        for (size_t i = 0; i < allSolutions.size(); i++) {
            std::cout << std::endl << "Solution #" << i + 1 << ":" << std::endl;

            for (int j = 0; j < n; j++) {
                std::cout << "x" << j + 1 << " = " << allSolutions[i][j] << std::endl;
            }
        }
    }
}