//
// Created by xux on 2024/9/11.
//

#pragma once
#ifndef CL4AF_CLAUSE_H
#define CL4AF_CLAUSE_H

#include <vector>
#include <list>
#include "label_lib.h"
#include "Bitset.h"

class Clause {
private:
    std::vector<int> lit;
    Bitset *in = nullptr;
    Bitset *out = nullptr;
public:
    static Clause of(int arg, int label);
    Clause exclude(int arg);
    int get_arg(int index);
    int get_sign(int index);
    void add(int arg, int label);
    Bitset& get_in(int arg_number);
    Bitset& get_out(int arg_number);
    std::list<int> collect_arg_as_list();

    int size(){return (int)lit.size();}
    auto begin(){return lit.begin();}
    auto end(){return lit.end();}
};


#endif //CL4AF_CLAUSE_H
