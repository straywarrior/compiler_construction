/*
 * symtable.h
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string>
#include <list>
#include <unordered_map>

namespace tinylang {

//! @brief Struct that represents a record in symbol table.
struct SymRecord {
    std::string name;
    std::list<int> lines; // record the positions at which the name occur
};

class SymTable {
public:
    void insert(const std::string & name, int line_no);

    void insert(const char * name, int line_no) {
        return this->insert(std::string(name), line_no);
    }

    int find(const std::string & name);

    int find(const char * name) {
        return this->find(std::string(name));
    }

    void print();

private:
    std::unordered_map<std::string, SymRecord> records_;
};

} /* namespace tinylang */

#endif /* !SYMTABLE_H */
