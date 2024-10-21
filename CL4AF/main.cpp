#include "Solver.h"

int main(int argc, char**argv) {
    Solver solver;
    solver.read_file(argv[1]);
    solver.solve();
    
    std::string s(argv[1]);
    if (s.ends_with(".af")){
        std::cout<<(solver.self_check() ? "true" : "false")<<std::endl;
    }
    return 0;
}
