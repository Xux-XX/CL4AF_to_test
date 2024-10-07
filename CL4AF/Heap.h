//
// Created by xux on 2024/9/24.
//

#ifndef CL4AF_HEAP_H
#define CL4AF_HEAP_H


#include <algorithm>

class Heap {
private:
    double*     scores;
    int         size,
                capacity,
                *args,
                *arg2id;

    void shiftup(int index);
    void shiftdown(int index);
    void swap(int index1, int index2);
public:
    Heap(int capacity);
    ~Heap();
    int top();
    int pop();
    void insert(double score, int value);
    void remove(int arg);
    bool empty();
};


#endif //CL4AF_HEAP_H
