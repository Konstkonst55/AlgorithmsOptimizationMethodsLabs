#include "LinearSystem.h"

int main() {
    LinearSystem sys;

    sys.loadFromFile("input.txt");
    sys.solveJordanGauss();

    return 0;
}