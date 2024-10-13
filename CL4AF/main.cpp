#include "Solver.h"

int main(int argc, char**argv) {
    Solver solver;
    solver.read_TGF(argv[1]);
    solver.solve();
    return 0;
}
