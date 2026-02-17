#include "LinearSystem.h"

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
    }
}

void LinearSystem::printMatrix() const {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= n; j++) {
            std::cout << std::setw(10) << matrix[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

void LinearSystem::solveJordanGauss() {
    std::cout << "Initial Matrix:" << std::endl;
    printMatrix();

    int pivotRow = 0;
    std::vector<int> colToVar(n, -1);

    for (int j = 0; j < n && pivotRow < m; j++) {
        int maxRow = pivotRow;

        for (int i = pivotRow + 1; i < m; i++) {
            if (matrix[i][j].abs() > matrix[maxRow][j].abs()) {
                maxRow = i;
            }
        }

        if (matrix[maxRow][j] != Fraction(0)) {
            std::swap(matrix[pivotRow], matrix[maxRow]);

            Fraction divisor = matrix[pivotRow][j];

            for (int k = j; k <= n; k++) {
                matrix[pivotRow][k] = matrix[pivotRow][k] / divisor;
            }

            for (int i = 0; i < m; i++) {
                if (i != pivotRow) {
                    Fraction factor = matrix[i][j];

                    for (int k = j; k <= n; k++) {
                        matrix[i][k] = matrix[i][k] - (factor * matrix[pivotRow][k]);
                    }
                }
            }

            colToVar[j] = pivotRow;
            pivotRow++;
            std::cout << "After elimination step for column " << j + 1 << ":" << std::endl;

            printMatrix();
        }
    }

    bool hasSolution = true;

    for (int i = 0; i < m; i++) {
        bool allZeros = true;

        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != Fraction(0)) {
                allZeros = false;
                break;
            }
        }

        if (allZeros && matrix[i][n] != Fraction(0)) {
            hasSolution = false;
            break;
        }
    }

    if (!hasSolution) {
        std::cout << "System has NO solution." << std::endl;
    }
    else if (pivotRow < n) {
        std::cout << "System has INFINITE solutions." << std::endl;

        printGeneralSolution(colToVar, pivotRow);
    }
    else {
        std::cout << "System has a UNIQUE solution:" << std::endl;

        for (int j = 0; j < n; j++) {
            if (colToVar[j] != -1) {
                std::cout << "x" << j + 1 << " = " << matrix[colToVar[j]][n] << std::endl;
            }
        }
    }
}

void LinearSystem::printGeneralSolution(const std::vector<int>& colToVar, int rank) const {
    std::cout << "\nGeneral solution:" << std::endl;

    std::vector<int> basicVars;
    std::vector<int> freeVars;

    for (int j = 0; j < n; j++) {
        if (colToVar[j] != -1) {
            basicVars.push_back(j);
        }
        else {
            freeVars.push_back(j);
        }
    }

    for (int basic : basicVars) {
        int row = colToVar[basic];
        std::cout << "x" << basic + 1 << " = ";

        bool firstTerm = true;

        if (matrix[row][n] != Fraction(0)) {
            std::cout << matrix[row][n];
            firstTerm = false;
        }

        for (int free : freeVars) {
            Fraction coeff = matrix[row][free];

            if (coeff != Fraction(0)) {
                if (!firstTerm && coeff > Fraction(0)) {
                    std::cout << " + ";
                }
                else if (!firstTerm && coeff < Fraction(0)) {
                    std::cout << " - ";
                    coeff = coeff.abs();
                }
                else if (firstTerm && coeff < Fraction(0)) {
                    std::cout << "-";
                    coeff = coeff.abs();
                }

                if (coeff == Fraction(1)) {
                    std::cout << "x" << free + 1;
                }
                else {
                    std::cout << coeff << "*x" << free + 1;
                }

                firstTerm = false;
            }
        }

        if (firstTerm) {
            std::cout << "0";
        }

        std::cout << std::endl;
    }

    if (!freeVars.empty()) {
        std::cout << "\nwhere ";

        for (size_t i = 0; i < freeVars.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << "x" << freeVars[i] + 1;
        }

        std::cout << " are free parameters" << std::endl;
    }
}