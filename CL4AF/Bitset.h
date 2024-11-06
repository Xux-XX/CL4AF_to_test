//
// Created by xux on 2024/10/24.
//

#ifndef CL4AF_BITSET_H
#define CL4AF_BITSET_H

#include <cstring>
#include <stdexcept>

class Bitset {
private:
    using WordT = unsigned long long;
    const int WORD = sizeof(WordT);

    unsigned    arr_size;
    unsigned    _size;
    WordT       *bits;

public:
    Bitset(unsigned size);
    ~Bitset();

    Bitset operator & (const Bitset& other);
    Bitset operator ^ (const Bitset& other);
    Bitset operator | (const Bitset& other);
    Bitset& operator = (const Bitset& other);

    int count();
    int lowbit();
    bool get(int offset);
    void set(int offset);
    void unset(int offset);

};


#endif //CL4AF_BITSET_H
