/*
 * test_scanner.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "catch.hpp"

// To test private methods
#define private public
#include "../parser.h"
#undef private

#define REQUIRE_STRCMP(x, y) REQUIRE(strcmp(x, y) == 0)

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

TEST_CASE( "Parser::stmt_sequence correctness", "[Parser]" ) {
    Parser parser;
    std::string input_data;
    input_data = "a := 1024 + 42; b := 9 * a;\nc := b - 23";
    TreeNode *tree = nullptr;
    parser.init_scanner(input_data.c_str(), input_data.size());
    REQUIRE(parser.token_ == TokenType::ID);
    REQUIRE(parser.token_str_ == "a");
    tree = parser.stmt_sequence();
    REQUIRE(tree != nullptr);
    REQUIRE(tree->node_type == NodeStmt);
    REQUIRE(tree->stmt == StmtAssign);
    REQUIRE_STRCMP(tree->attr.name, "a");

    // node: 1024 + 42
    TreeNode *node = tree->children[0];
    REQUIRE(node != nullptr);
    REQUIRE(node->node_type == NodeExpr);
    REQUIRE(node->expr == ExprOp);
    REQUIRE(node->attr.op == TokenType::PLUS);
    REQUIRE(node->children[0]->expr == ExprConst);
    REQUIRE(node->children[0]->attr.val == 1024);
    REQUIRE(node->children[1]->expr == ExprConst);
    REQUIRE(node->children[1]->attr.val == 42);

    // node: b assignment
    node = tree->neighbor;
    REQUIRE(node != nullptr);
    REQUIRE(node->node_type == NodeStmt);
    REQUIRE(node->stmt == StmtAssign);
    REQUIRE_STRCMP(node->attr.name, "b");

    // node: 9 * a
    node = node->children[0];
    REQUIRE(node->expr == ExprOp);
    REQUIRE(node->attr.op == TokenType::TIMES);
    REQUIRE(node->children[0]->expr == ExprConst);
    REQUIRE(node->children[0]->attr.val == 9);
    REQUIRE(node->children[1]->expr == ExprIdentifier);
    REQUIRE_STRCMP(node->children[1]->attr.name, "a");

    // node: c assignment
    node = tree->neighbor->neighbor;
    REQUIRE(node != nullptr);
    REQUIRE(node->node_type == NodeStmt);
    REQUIRE(node->stmt == StmtAssign);
    REQUIRE_STRCMP(node->attr.name, "c");

    // node: b - 23
    node = node->children[0];
    REQUIRE(node->expr == ExprOp);
    REQUIRE(node->attr.op == TokenType::MINUS);
    REQUIRE(node->children[0]->expr == ExprIdentifier);
    REQUIRE_STRCMP(node->children[0]->attr.name, "b");
    REQUIRE(node->children[1]->expr == ExprConst);
    REQUIRE(node->children[1]->attr.val == 23);

    REQUIRE(node->neighbor == nullptr);
    destroyTreeNode(tree);
}

TEST_CASE( "Parser::parse correctness", "[Parser]" ) {
    Parser parser;
    std::string input_data;
    input_data = "if (a < 0) then a := a + 1 end";
    TreeNode *tree = parser.parse(input_data.c_str(), input_data.size());
    REQUIRE(tree != nullptr);
}
