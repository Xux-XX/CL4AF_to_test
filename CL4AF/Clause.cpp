//
// Created by xux on 2024/9/11.
//

#include <cassert>
#include "Clause.h"

Clause Clause::of(int arg, int label) {
    assert(label != LAB_BLANK);
    Clause res;
    auto& v = res.lit;
    v.assign(1, arg<<1|(label==LAB_IN));
    return res;
}

void Clause::add(int arg, int label) {
    assert(label != LAB_BLANK);
    lit.push_back(arg<<1|(label==LAB_IN));
}

Clause Clause::exclude(int arg) {
    Clause res;
    for (int i = 0; i < lit.size(); ++i) {
        if (get_arg(i) != arg) res.lit.push_back(lit[i]);
    }
    return res;
}

int Clause::get_arg(int index) {
    return lit[index]>>1;
}

int Clause::get_sign(int index) {
    return lit[index]&1 ? LAB_IN : LAB_OUT;
}

Bitset Clause::get_out(int arg_number) {
    Bitset res(arg_number);
    for(int i=0;i<size();i++){
        auto arg = get_arg(i),
             lab = get_sign(i);
        if (lab == LAB_OUT) res.set(arg);
    }
    return res;
}

Bitset Clause::get_in(int arg_number) {
    Bitset res(arg_number);
    for(int i=0;i<size();i++){
        auto arg = get_arg(i),
             lab = get_sign(i);
        if (lab == LAB_IN) res.set(arg);
    }
    return res;
}

std::list<int> Clause::collect_arg_as_list() {
    std::list<int> res;
    for(int i=0;i<size();i++){
        res.push_back(get_arg(i));
    }
    return res;
}


