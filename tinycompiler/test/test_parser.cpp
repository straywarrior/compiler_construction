/*
 * test_scanner.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// To test private methods
#define private public
#include "../parser.h"
#undef private

using namespace tinylang;

TEST_CASE( "Parser::term correctness", "[Parser]" ) {
    Parser parser;
    std::string input_data;
    input_data = "1 * 1";
    TreeNode *tree = nullptr;
    parser.init_scanner(input_data.c_str(), input_data.size());
    REQUIRE(parser.token_ == TokenType::NUM);
    REQUIRE(parser.token_str_ == "1");
    tree = parser.term();
    REQUIRE(tree != nullptr);
    REQUIRE(tree->node_type == NodeExpr);
    REQUIRE(tree->attr.op == TokenType::TIMES);
    destroyTreeNode(tree);

    input_data = "(1 + 2) * rhs";
    parser.init_scanner(input_data.c_str(), input_data.size());
    REQUIRE(parser.token_ == TokenType::LPAREN);
    REQUIRE(parser.token_str_ == "(");
    tree = parser.term();
    REQUIRE(tree != nullptr);
    REQUIRE(tree->node_type == NodeExpr);
    REQUIRE(tree->attr.op == TokenType::TIMES);
    TreeNode * node = nullptr;
    node = tree->children[0];
    REQUIRE(node->attr.op == TokenType::PLUS);
    node = tree->children[1];
    REQUIRE(node->expr == ExprIdentifier);
    REQUIRE(strcmp(node->attr.name, "rhs") == 0);
    destroyTreeNode(tree);
}

TEST_CASE( "Parser::parse correctness", "[Parser]" ) {
    Parser parser;
    std::string input_data;
    input_data = "if (a < 0) then a := a + 1; end";
    TreeNode *tree = parser.parse(input_data.c_str(), input_data.size());
    REQUIRE(tree != nullptr);
}
