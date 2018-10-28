/*
 * main.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

int load_file(const char * filepath, std::string & dst) {
    std::ifstream ifs(filepath);
    if (!ifs) {
        std::cerr << "error: cannot open file " << filepath;
        return -1;
    }
    std::stringstream strbuf;
    strbuf << ifs.rdbuf();
    dst = strbuf.str();
    return 0;
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        std::cerr << "error: no input files" << std::endl;
        return -1;
    }
    std::string source_lines;
    if (load_file(argv[1], source_lines) != 0) {
        return -1;
    }
    tinylang::Parser parser = tinylang::Parser();
    tinylang::TreeNode * ast =
        parser.parse(source_lines.c_str(), source_lines.size());
    return 0;
}


