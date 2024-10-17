//
// Created by xux on 2024/9/11.
//


#include "Solver.h"

void Solver::solve() {
    init_label();
    int decide_res = 0;
    while (decide_res != NO_BLANK_ARG){
        propagate:
#ifdef DEBUG
        std::string msg = "now: ";
        for(auto arg:args){
            msg += id2argument[arg] + ":" + label2char(get_label(arg)) + "(" + std::to_string(depth[arg]) + ")"+ ",";
        }
        LOG(msg + "\n");
#endif
        int status = propagate();
        if (status != NO_CONFLICT){
            auto [learnt_clause, backtrack_level, conflict_status, need_modify_arg] = analyze();
            if (conflict_status == CONFLICT_ON_BASE) break;
            auto arg = need_modify_arg >> 1;
            auto lab = need_modify_arg&1 ? LAB_OUT : LAB_IN;
#ifdef DEBUG
            msg = "";
            for(int i=0;i<learnt_clause.size();i++){
                auto id = learnt_clause.get_arg(i);
                auto sign = learnt_clause.get_sign(i);
                msg += id2argument[id] + ":" + label2char(sign) + "(" + std::to_string(depth[id]) + ")" + ",";
            }
            LOG("learnt_clause: " + msg + "\n");
#endif
            backtrack(backtrack_level);
#ifdef DEBUG
            LOG("modify_arg: " + id2argument[arg] + "\t" + label2char(get_label(arg)) + "->" + label2char(lab) + "\n");
#endif
            set_label(arg, lab, learnt_clause.exclude(arg));
            goto propagate;
        }
        decide_res = decide();
    }
    if (decide_res == NO_BLANK_ARG){
        print_ans();
    }
    else{
        std::cout<<"no ans"<<std::endl;
    }
}

int Solver::decide() {
    if (arguments_to_label->empty()) return NO_BLANK_ARG;
    current_level++;
    auto arg = arguments_to_label->top();
    set_label(arg, LAB_IN, DECISION);
#ifdef DEBUG
    LOG("decide: " + id2argument[arg]);
#endif
    return SUCCESS_DECISION;
}

int Solver::propagate() {
    int status = NO_CONFLICT;
    while (propagated < trail.size()){
        auto arg = std::get<0>(trail[propagated++]);
        auto lab = get_label(arg);
        switch (lab) {
            case LAB_IN:
                status = IN_propagate(arg);
                break;
            case LAB_OUT:
                status = OUT_propagate(arg);
                break;
            default:
                assert(false);
        }
        if (status == CONFLICT) return CONFLICT;
    }
    return NO_CONFLICT;
}

void Solver::set_label(int arg, int lab, Clause&& reason) {
#ifdef DEBUG
    for (int i = 0; i < reason.size(); ++i) {
        auto a = reason.get_arg(i);
        assert(depth[a] <= current_level);
    }
#endif
    trail.emplace_back(arg, get_label(arg),depth[arg], reasons[arg]);
    if (get_label(arg) == LAB_BLANK) arguments_to_label->remove(arg);
    label[arg] = lab;
    reasons[arg] = reason;
    depth[arg] = current_level;
}

int Solver::get_label(int arg) {
    return label[arg];
}

int Solver::IN_propagate(int arg) {
    for(auto child:attack[arg]){
        auto lab = get_label(child);
        if (lab == LAB_BLANK) {
            set_label(child, LAB_OUT, Clause::of(arg, LAB_IN));
        }
        else if( lab == LAB_IN){
            conflict = {child, arg};
            return CONFLICT;
        }
    }

    for (auto parent:attack_by[arg]) {
        auto lab = get_label(parent);
        if (lab == LAB_BLANK){
            set_label(parent, LAB_OUT, Clause::of(arg, LAB_IN));
        }
        else if(lab == LAB_IN){
            conflict = {parent, arg};
            return CONFLICT;
        }
    }
    // TODO: 通过学习到的子句进行传播

    return NO_CONFLICT;
}

int Solver::OUT_propagate(int arg) {
    if(!is_legal_must_out(arg)){
        conflict.clear();
        conflict.insert(conflict.end(), attack_by[arg].begin(), attack_by[arg].end());
        conflict.push_back(arg);
        return CONFLICT;
    }

    for (auto child: attack[arg]){
        if(get_label(child) == LAB_OUT && !is_legal_must_out(child)){
            conflict.clear();
            conflict.insert(conflict.end(), attack_by[child].begin(), attack_by[child].end());
            conflict.push_back(child);
            return CONFLICT;
        }
    }
    return NO_CONFLICT;
}

int Solver::MUST_OUT_propagate(int arg) {
    if(!is_legal_must_out(arg)){
        conflict.clear();
        conflict.insert(conflict.end(), attack_by[arg].begin(), attack_by[arg].end());
        conflict.push_back(arg);
#ifdef DEBUG
        LOG("MUST_OUT_PROPAGATE CONFLICT");
#endif
        return CONFLICT;
    }
    return NO_CONFLICT;
}

int Solver::MUST_IN_propagate(int arg) {
    label[arg] = LAB_IN;
    return IN_propagate(arg);
}

std::tuple<Clause, int, int, int> Solver::analyze() {
#ifdef DEBUG
    std::string msg("conflict:");
    for(auto arg:conflict){
        msg += id2argument[arg] + ":" + label2char(get_label(arg)) + "(" + std::to_string(depth[arg]) +")" + ",";
    }
    msg += " current_level: " + std::to_string(current_level);
    LOG(msg + "\n");
#endif
    assert(!conflict.empty());
    std::vector<bool> vis(arg_number, false);
    Clause learnt_clause;
    int backtrack_level = 0;

    // 删除链表中不属于当前层的争议，只保留当前层方便后面进行bfs
    auto it = conflict.begin();
    while (it != conflict.end()){
        auto arg = *it;
        vis[arg] = true;
        if (depth[arg] != current_level){
            learnt_clause.add(arg, get_label(arg));
            conflict.erase(it++);
            backtrack_level = std::max(backtrack_level, depth[arg]);
        }
        else{
            it++;
        }
    }
    if (conflict.empty() || current_level == 0) return {Clause(), -1, CONFLICT_ON_BASE, -1};

    int last_arg = -1;
    while (!conflict.empty()){
        auto arg = conflict.front();
        conflict.pop_front();
        if (reasons[arg].size() == 0){
            if(last_arg == -1 || depth[last_arg] < depth[arg]){
                last_arg = arg;
            }
        }
        for (int i = 0; i < reasons[arg].size(); ++i) {
            auto pre = reasons[arg].get_arg(i);
            if (vis[pre]) continue;
            vis[pre] = true;
            if (depth[pre] != current_level){
                learnt_clause.add(pre, get_label(pre));
                backtrack_level = std::max(backtrack_level, depth[pre]);
            }
            else conflict.push_back(pre);
        }
    }
    auto arg = last_arg;
    assert(arg != -1);
    learnt_clause.add(arg, get_label(arg));

    return {learnt_clause, backtrack_level, CONFLICT, arg<<1|(get_label(arg)==LAB_IN)};
}

void Solver::read_TGF(const std::string &filename) {
    std::ifstream fin(filename);
    if(!fin){
        std::cout<<"无法打开文件: "<<filename<<std::endl;
        exit(1);
    }
    std::string arg1, arg2;
    int arg_id = 0;
    while (fin>>arg1, arg1!="#"){
        args.push_back(arg_id);
        id2argument.push_back(arg1);
        argument2id[arg1] = arg_id++;
    }
    alloc_memory(args.size());
    while (fin>>arg1>>arg2){
        if(argument2id.find(arg1) == argument2id.end()
        || argument2id.find(arg2) == argument2id.end()){
            std::cout<<filename<<"文件格式错误"<<std::endl;
            exit(1);
        }
        int id1 = argument2id[arg1],
            id2 = argument2id[arg2];
        attack[id1].push_back(id2);
        attack_by[id2].push_back(id1);
        if (id1 == id2) {
            label[id1] = LAB_OUT;
            trail.emplace_back(id1, label[id1], 0, Clause());
        }
    }
}

void Solver::backtrack(int backtrack_level) {
#if DEBUG
    std::cout<<"backtrack_level:"<<backtrack_level<<" current_level:"<<current_level<<std::endl;
    std::string msg("backtrack:\n");
#endif
    assert(backtrack_level < current_level);
    while (!trail.empty()){
        auto [arg, lab, dep, reason] = trail.back();
        if (depth[arg] == backtrack_level) break;
#ifdef DEBUG
        msg += "\t" + id2argument[arg] + ":" + label2char(get_label(arg)) + " -> " + label2char(lab) + "\n";
#endif
        trail.pop_back();
        label[arg] = lab;
        depth[arg] = dep;
        reasons[arg] = std::move(reason);
        if (lab == LAB_BLANK) arguments_to_label->insert(hval(arg), arg);
    }
    current_level = backtrack_level;
    propagated = (int)trail.size();
#ifdef DEBUG
    LOG(msg);
#endif
}

void Solver::alloc_memory(size_t size) {
    propagated = current_level = 0;
    arg_number = (int)size;
    label.assign(arg_number, LAB_BLANK);
    depth.assign(arg_number, 0);
    reasons.assign(arg_number, {});
    attack.assign(arg_number, {});
    attack_by.assign(arg_number, {});
    arguments_to_label = std::make_shared<Heap>(arg_number);
}

void Solver::print_ans() {
    std::cout<<"[";
    bool is_first = true;
    for(auto arg: args){
        if (get_label(arg) == LAB_IN){
            if (!is_first)std::cout<<",";
            std::cout<<id2argument[arg];
            is_first = false;
        }
    }
    std::cout<<"]"<<std::endl;
}

// TODO
double Solver::hval(int arg) {
    return 0;
}

bool Solver::is_legal_must_out(int arg) {
    return std::any_of(attack_by[arg].begin(), attack_by[arg].end(),  [&](int parent){
        int lab = get_label(parent);
        return lab == LAB_BLANK || lab == LAB_MUST_IN || lab == LAB_IN;
    });
}

void Solver::init_label() {
//    std::vector<int>blank_attack_count(arg_number, 0);
//    std::queue<int> q;
//    for (int i = 0; i < arg_number; ++i) {
//        blank_attack_count[i] = (int)attack_by[i].size();
//        if (blank_attack_count[i] == 0) q.push(i);
//    }
//    while(!q.empty()){
//        auto arg = q.front();
//        q.pop();
//        label[arg] = LAB_IN;
//        for(auto child: attack[arg]){
//            label[arg] = LAB_OUT;
//            for(auto child1:attack[child]){
//                if (--blank_attack_count[child1] == 0){
//                    q.push(child1);
//                }
//            }
//        }
//        for(auto parent:attack_by[arg]){
//            auto lab = get_label(parent);
//            label[arg] = LAB_MUST_OUT;
//            for(auto pchild:attack[parent]){
//                if (--blank_attack_count[pchild] == 0){
//                    q.push(pchild);
//                }
//            }
//        }
//    }
    for(auto arg: args){
        if (get_label(arg) == LAB_BLANK) arguments_to_label->insert(hval(arg), arg);
    }
}

char Solver::label2char(int lab) {
    switch (lab) {
        case LAB_IN: return 'I';
        case LAB_MUST_OUT: return 'M';
        case LAB_OUT: return 'O';
        case LAB_BLANK: return 'X';
        case LAB_MUST_IN: return 'W';
        default: return '?';
    }
}

void Solver::read_file(const std::string &filename) {
    if (filename.ends_with(".af")) {
        read_AF(filename);
    }
    else if(filename.ends_with(".tgf")) {
        read_TGF(filename);
    }
    else {
        std::cout<<"illegal file format"<<std::endl;
        exit(1);
    }
}

void Solver::read_AF(const std::string &filename) {
    std::ifstream fin(filename);
    if (!fin){
        std::cout<<"can not open "<<filename<<std::endl;
        exit(1);
    }
    std::string arg1, arg2;
    std::vector<int>self_attack;
    int arg_id = 0;
    while (fin>>arg1){
        fin>>arg2;
        if (arg1 == "#"){
            args.push_back(arg_id);
            id2argument.push_back(arg2);
            argument2id[arg2] = arg_id++;
        }
        else{
            assert(argument2id.find(arg1) != argument2id.end() && argument2id.find(arg2) != argument2id.end());
            int id1 = argument2id[arg1],
                id2 = argument2id[arg2];
            if (id1 == id2){
                self_attack.push_back(id1);
                continue;
            }
            attack[id1].push_back(id2);
            attack_by[id2].push_back(id1);
        }
    }
    alloc_memory(args.size());
    for(auto arg:self_attack){
        label[arg] = LAB_OUT;
        trail.emplace_back(arg, label[arg], 0, Clause());
    }
}
