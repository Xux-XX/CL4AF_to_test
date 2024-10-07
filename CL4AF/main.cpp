#include "Solver.h"

int main(int argc, char**argv) {
//    std::cout<<"========== start =========="<<std::endl;
    Solver solver;
    solver.read_TGF(argv[1]);
    solver.solve();
//    std::cout<<"==========  end  =========="<<std::endl;
    return 0;
}
