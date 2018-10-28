/*
 * analyser.h
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef ANALYSER_H
#define ANALYSER_H

#include "parser.h"
#include "symtable.h"

namespace tinylang {

/**
 * @brief Class for semantic analysis.
 */
class Analyser {
public:
    /**
     * @brief Do semantic analysis on the given syntax tree.
     *
     * @return 0 for no error.
     */
    int analyse(TreeNode * tree);

private:
    int build_symbol_table(TreeNode * tree);

    int check_type(TreeNode * tree);

private:
    SymTable symtable_;
};

} /* namespace tinylang */

#endif /* !ANALYSER_H */
