//
// Created by xux on 2024/10/24.
//

#include "Bitset.h"

// 构造函数
Bitset::Bitset(unsigned size) : arr_size((size + WORD - 1) / WORD), _size(size) {
    bits = new WordT[arr_size];
    memset(bits, 0, arr_size * sizeof(WordT)); // 初始化为 0
}

// 析构函数
Bitset::~Bitset() {
    delete[] bits;
}

// 位与操作
Bitset Bitset::operator&(const Bitset& other) {
    if (_size != other._size) {
        throw std::invalid_argument("Bitsets must be of the same size");
    }
    Bitset result(_size);
    for (unsigned i = 0; i < arr_size; ++i) {
        result.bits[i] = bits[i] & other.bits[i];
    }
    return result;
}

// 计数操作
int Bitset::count() {
    int total = 0;
    for (unsigned i = 0; i < arr_size; ++i) {
        total += __builtin_popcountll(bits[i]); // 统计 1 的个数
    }
    return total;
}

// 设置位
void Bitset::set(int offset) {
    if (offset < 0 || offset >= _size) throw std::out_of_range("Index out of range");
    bits[offset / WORD] |= (1ULL << (offset % WORD));
}

// 取消设置位
void Bitset::unset(int offset) {
    if (offset < 0 || offset >= _size) throw std::out_of_range("Index out of range");
    bits[offset / WORD] &= ~(1ULL << (offset % WORD));
}

bool Bitset::get(int offset) {
    return (bool )(bits[offset / WORD] & (1ULL << (offset % WORD)));
}

// 赋值运算符重载
Bitset& Bitset::operator=(const Bitset& other) {
    if (this != &other) { // 防止自赋值
        delete[] bits; // 释放旧内存
        arr_size = other.arr_size;
        _size = other._size;
        bits = new WordT[arr_size];
        std::memcpy(bits, other.bits, arr_size * sizeof(WordT)); // 深拷贝
    }
    return *this;
}

Bitset Bitset::operator^(const Bitset &other) {
    if (_size != other._size) {
        throw std::invalid_argument("Bitsets must be of the same size");
    }
    Bitset result(_size);
    for (unsigned i = 0; i < arr_size; ++i) {
        result.bits[i] = bits[i] ^ other.bits[i];
    }
    return result;
}

Bitset Bitset::operator|(const Bitset &other) {
    if (_size != other._size) {
        throw std::invalid_argument("Bitsets must be of the same size");
    }
    Bitset result(_size);
    for (unsigned i = 0; i < arr_size; ++i) {
        result.bits[i] = bits[i] | other.bits[i];
    }
    return result;
}

int Bitset::lowbit() {
    int res = 0;
    for(int i=0;i<arr_size;++i){
        if (bits[i]){
            return res + __builtin_ctz(bits[i] & -bits[i]);
        }
        res += WORD;
    }
    return -1;
}
