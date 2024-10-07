//
// Created by xux on 2024/9/11.
//

#pragma once
#ifndef CL4AF_CLAUSE_H
#define CL4AF_CLAUSE_H

#include <vector>
#include "label_lib.h"

class Clause {
private:
    std::vector<int> lit;
public:
    static Clause of(int arg, int label);
    static Clause of(std::vector<int>&& a);
    Clause exclude(int arg);
    int get_arg(int index);
    int get_sign(int index);
    void add(int arg, int label);

    int size(){return (int)lit.size();}
    auto begin(){return lit.begin();}
    auto end(){return lit.end();}
};


#endif //CL4AF_CLAUSE_H
