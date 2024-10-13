//
// Created by xux on 2024/9/11.
//

#include <cassert>
#include "Clause.h"

Clause Clause::of(int arg, int label) {
    assert(label != LAB_BLANK);
    Clause res;
    auto& v = res.lit;
    v.assign(1, arg<<1|(label==LAB_IN || label == LAB_MUST_IN));
    return res;
}

Clause Clause::of(std::vector<int>&& a) {
    Clause res;
    auto& v = res.lit;
    v = a;
    return res;
}

void Clause::add(int arg, int label) {
    assert(label != LAB_BLANK);
    lit.push_back(arg<<1|(label==LAB_IN || label == LAB_MUST_IN));
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

