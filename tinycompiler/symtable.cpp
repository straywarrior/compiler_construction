/*
 * symtable.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "symtable.h"
#include <cstdio>
#include <string>

namespace tinylang {

void SymTable::insert(const std::string & name, int line_no) {
    if (records_.find(name) == records_.end()) {
        auto & record = records_[name];
        record.name = name;
        record.lines.push_back(line_no);
    } else {
        auto & record = records_[name];
        record.lines.push_back(line_no);
    }
}

int SymTable::find(const std::string & name) {
    auto iter = records_.find(name);
    if (iter == records_.end()) {
        return -1;
    }
    return 0;
}

void SymTable::print() {
    printf("SymbolName\tLines\n");
    std::string buf;
    for (const auto & p : records_) {
        for (int i : p.second.lines) {
            buf += std::to_string(i) + " ";
        }
        printf("%-10s\t%s\n", p.second.name.c_str(), buf.c_str());
        buf.clear();
    }
}

} /* namespace tinylang */
