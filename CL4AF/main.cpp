#include "Solver.h"

int main(int argc, char**argv) {
//    std::ofstream logFile("test.log");
//    std::streambuf *oldCoutBuffer = std::cout.rdbuf(logFile.rdbuf());

    Solver solver;
    solver.read_file(argv[1]);
    solver.solve();

    return 0;
}
