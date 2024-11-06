//
// Created by xux on 2024/9/11.
//
#pragma once
#ifndef CL4AF_SOLVER_H
#define CL4AF_SOLVER_H


#define NO_CONFLICT (-1)
#define CONFLICT 0
#define CONFLICT_ON_BASE (-1)

#define SUCCESS_DECISION 0
#define DECISION Clause()

#define NO_BLANK_ARG (-1)

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>

#include "Heap.h"
#include "Clause.h"
#include "log.h"
#include "Bitset.h"

class Solver {
private:
    constexpr static int    label_map[] = {LAB_BLANK, LAB_IN, LAB_OUT};


    int                 propagated,             // 已传播争议的位置
                        arg_number,             // 争议个数,
                        current_level;          // 记录当前层次
    std::vector<int>    args,                   // 争议集
                        depth;                  // 争议所在层次
    Bitset              *in,
                        *out;                   // 争议的标签
    std::list<int>      conflict;               // 冲突争议集
    std::vector<Clause> clause_DB;              // 存储学习子句
    std::vector<Clause> reasons;                 // 争议被赋值的原因
    std::vector<std::vector<int>>           attack,                // 攻击关系
                                            attack_by;             // 被攻击争议集
    std::shared_ptr<Heap>                   arguments_to_label;     // 标签为blank的争议
    std::vector<std::string>                id2argument;    // 将int转为string类型
    std::unordered_map<std::string, int>    argument2id;    // 将string转为int类型
    std::vector<std::tuple<int,int,int,Clause>> trail;          // 记录用于回溯的信息{arg, label, depth, reason}


    void add_clause(Clause &&clause);
    bool is_legal_must_out(int arg);
    static double hval(int arg);
    void set_infer(int arg, int lab, Clause&& reason);
    void set_label(int arg, int lab);
    int get_label(int arg);
    void alloc_memory(size_t size);
    int decide();
    std::tuple<Clause, int, int, int> analyze();
    void backtrack(int backtrack_level);
    int propagate();
    int IN_propagate(int arg);
    int OUT_propagate(int arg);
    int clause_propagate();
    int MUST_OUT_propagate(int arg);
    int MUST_IN_propagate(int arg);
    void init_label();
    static char label2char(int lab);

    void print_ans();

    void read_TGF(const std::string& filename);
    void read_AF(const std::string& filename);
public:
    void read_file(const std::string& filename);
    void solve();
    bool self_check();
};


#endif //CL4AF_SOLVER_H
