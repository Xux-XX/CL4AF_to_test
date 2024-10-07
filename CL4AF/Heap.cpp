//
// Created by xux on 2024/9/24.
//
#include "Heap.h"

void Heap::shiftdown(int index) {
    int left = index<<1|1;
    int fa = index;
    while (left < size) {
        if (left+1 < size && scores[left+1] < scores[left]){
            swap(left, left+1);
        }
        if (scores[fa] < scores[left]) break;
        swap(fa, left);
        fa = left;
        left = fa<<1|1;
    }
}

void Heap::shiftup(int index) {
    int fa = (index-1)>>1;
    int child = index;
    while (child && scores[fa] < scores[child]) {
        swap(child, fa);
        child = fa;
        fa = (fa-1)>>1;
    }
}

bool Heap::empty() {
    return size == 0;
}

void Heap::insert(double score, int value) {
    args[size] = value;
    arg2id[value] = size;
    scores[size++] = score;
    shiftup(size-1);
}

int Heap::pop() {
    auto res = args[0];
    args[0] = args[--size];
    shiftdown(0);
    return res;
}

void Heap::remove(int arg) {
    int id = arg2id[arg];
    swap(--size, id);
    shiftdown(id);
}

Heap::Heap(int capacity):capacity(capacity), size(0) {
    args = new int[capacity];
    arg2id = new int[capacity];
    scores = new double[capacity];
}

Heap::~Heap() {
    delete []args;
    delete []arg2id;
    delete []scores;
}

void Heap::swap(int index1, int index2) {
    arg2id[args[index1]] = index2;
    arg2id[args[index2]] = index1;
    std::swap(args[index1], args[index2]);
    std::swap(scores[index1], scores[index2]);
}

int Heap::top() {
    return args[0];
}
