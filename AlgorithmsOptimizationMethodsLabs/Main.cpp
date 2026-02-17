#include "LinearSystem.h"

int main() {
    LinearSystem sys;

    sys.loadFromFile("input.txt");

    sys.findAllBasisSolutions();

    return 0;
}