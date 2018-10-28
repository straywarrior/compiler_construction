/*
 * analyser.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "analyser.h"
#include <functional>

namespace tinylang {

using traverse_proc_t = std::function<void(TreeNode *)>;

//! @brief Just do nothing
static void empty_proc(TreeNode * t) {
    return;
}

static void insert_node_to_symtable(SymTable * st, TreeNode * t) {
    switch (t->node_type) {
        case NodeType::NodeStmt:
            switch (t->stmt) {
                // only assign / read statement create new symbols
                case StmtProp::StmtAssign:
                case StmtProp::StmtRead:
                    st->insert(t->attr.name, t->line_no);
                    break;
                default:
                    break;
            }
            break;
        case NodeType::NodeExpr:
            switch (t->expr) {
                case ExprProp::ExprIdentifier:
                    st->insert(t->attr.name, t->line_no);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

static void check_node_type(SymTable * st, TreeNode * t) {
}

static void traverse(TreeNode * node,
                     traverse_proc_t pre_proc, traverse_proc_t post_proc) {
    if (node != nullptr) {
        //node->print();
        pre_proc(node);
        for (int i = 0; i < TreeNode::MAX_CHILDREN; ++i) {
            // recursively call this function
            traverse(node->children[i], pre_proc, post_proc);
        }
        post_proc(node);
        // TODO: do not make recursive calls for neighbor nodes
        traverse(node->neighbor, pre_proc, post_proc);
    }
}

int Analyser::build_symbol_table(TreeNode * tree) {
    using std::placeholders::_1;
    traverse_proc_t proc = std::bind(insert_node_to_symtable, &symtable_, _1);
    traverse(tree, proc, empty_proc);
    return 0;
}

int Analyser::check_type(TreeNode * tree) {
    using std::placeholders::_1;
    traverse_proc_t proc = std::bind(check_node_type, &symtable_, _1);
    traverse(tree, empty_proc, proc);
    return 0;
}

int Analyser::analyse(TreeNode * tree) {
    if (this->build_symbol_table(tree) != 0) {
        return -1;
    }
    symtable_.print();
    if (this->check_type(tree) != 0) {
        return -1;
    }
    return 0;
}

} /* namespace tinylang */
