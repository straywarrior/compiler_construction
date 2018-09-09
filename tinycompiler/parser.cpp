/*
 * parser.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "parser.h"
#include <cstdio>
#include <cstdlib>

namespace tinylang {

static TreeNode *make_stmt_node(StmtProp stmt_prop) {
    TreeNode *node = new TreeNode();
    node->node_type = NodeStmt;
    node->stmt = stmt_prop;
    return node;
}

static TreeNode *make_expr_node(ExprProp expr_prop) {
    TreeNode *node = new TreeNode();
    node->node_type = NodeExpr;
    node->expr = expr_prop;
    return node;
}

static char *copy_str(const std::string &src) {
    size_t n = src.size();
    char *dst = new char[n];
    strncpy(dst, src.c_str(), n);
    return dst;
}

void destroyTreeNode(TreeNode *tree) {
    if (tree->node_type == NodeExpr && tree->expr == ExprIdentifier) {
        delete tree->attr.name;
        tree->attr.name = nullptr;
    }
    for (int i = 0; i < TreeNode::MAX_CHILDREN; ++i) {
        if (tree->children[i] != nullptr) {
            destroyTreeNode(tree->children[i]);
        }
    }
    // do not use recursion here
    TreeNode *next_node = nullptr;
    TreeNode *neighbor = tree->neighbor;
    while (neighbor != nullptr) {
        if (neighbor->neighbor != nullptr) {
            next_node = neighbor->neighbor;
            neighbor->neighbor = nullptr;
        }
        destroyTreeNode(neighbor);
        neighbor = next_node;
        next_node = nullptr;
    }
}

Parser::Parser() {
    scanner_ = new Scanner();
}

TreeNode *Parser::parse(const char *input_data, size_t input_len) {
    this->init_scanner(input_data, input_len);
    TreeNode *t = this->stmt_sequence();
    return t;
}

Parser::~Parser() {
    if (scanner_) {
        delete scanner_;
        scanner_ = nullptr;
    }
}

void Parser::match_token(TokenType token) {
    if (token == token_) {
        token_ = this->scanner_->getToken(&token_str_);
    } else {
        this->syntax_error(("Expect: " + getTokenTypeName(token)).c_str());
    }
}

void Parser::syntax_error(const char *msg) {
    printf("Unexpected token: %s at line %lu\n%s\n",
           token_str_.c_str(), scanner_->current_line_no(), msg);
}

TreeNode *Parser::stmt_sequence() {
    TreeNode *node = this->statement();
    TreeNode *t = node;
    while (token_ == TokenType::SEMI) {
        this->match_token(TokenType::SEMI);
        t->neighbor = this->statement();
        t = t->neighbor;
    }
    return node;
}
TreeNode *Parser::statement() {
    TreeNode *node = nullptr;
    switch (token_) {
        case TokenType::IF:
            node = this->if_stmt();
            break;
        case TokenType::REPEAT:
            node = this->repeat_stmt();
            break;
        case TokenType::ID:
            node = this->assign_stmt();
            break;
        case TokenType::READ:
            node = this->read_stmt();
            break;
        case TokenType::WRITE:
            node = this->write_stmt();
            break;
        default:
            this->syntax_error("");
            break;
    }
    return node;
}
TreeNode *Parser::if_stmt() {
    TreeNode *node = make_stmt_node(StmtIf);
    this->match_token(TokenType::IF);
    node->children[0] = this->expr();
    this->match_token(TokenType::THEN);
    node->children[1] = this->stmt_sequence();
    if (token_ == TokenType::ELSE) {
        node->children[2] = this->stmt_sequence();
    }
    this->match_token(TokenType::END);
    return node;
}
TreeNode *Parser::repeat_stmt() {
    TreeNode *node = make_stmt_node(StmtRepeat);
    this->match_token(TokenType::REPEAT);
    node->children[0] = this->stmt_sequence();
    this->match_token(TokenType::UNTIL);
    node->children[1] = this->expr();
    return node;
}
TreeNode *Parser::assign_stmt() {
    TreeNode *node = make_stmt_node(StmtAssign);
    node->attr.name = copy_str(token_str_);
    this->match_token(TokenType::ID);
    this->match_token(TokenType::ASSIGN);
    node->children[0] = this->expr();
    return node;
}
TreeNode *Parser::read_stmt() {
    TreeNode *node = make_stmt_node(StmtRead);
    this->match_token(TokenType::READ);
    node->attr.name = copy_str(token_str_);
    this->match_token(TokenType::ID);
    return node;
}
TreeNode *Parser::write_stmt() {
    TreeNode *node = make_stmt_node(StmtWrite);
    this->match_token(TokenType::WRITE);
    node->children[0] = this->expr();
    return node;
}
TreeNode *Parser::expr() {
    TreeNode *node = this->simple_expr();
    if (token_ == TokenType::LT || token_ == TokenType::EQ) {
        TreeNode *t = make_expr_node(ExprOp);
        t->children[0] = node;
        t->attr.op = token_;
        node = t; // node t is now the parent node
        this->match_token(token_);
        node->children[1] = this->simple_expr();
    }
    return node;
}
TreeNode *Parser::simple_expr() {
    TreeNode *node = this->term();
    if (token_ == TokenType::PLUS || token_ == TokenType::MINUS) {
        TreeNode *t = this->add_op();
        t->children[0] = node;
        node = t;
        node->children[1] = this->term();
    }
    return node;
}
TreeNode *Parser::add_op() {
    TreeNode *node = make_expr_node(ExprOp);
    node->attr.op = token_;
    this->match_token(token_);
    return node;
}
TreeNode *Parser::term() {
    TreeNode *node = this->factor();
    if (token_ == TokenType::TIMES || token_ == TokenType::OVER) {
        TreeNode *t = this->mul_op();
        t->children[0] = node;
        node = t;
        node->children[1] = this->term();
    }
    return node;
}
TreeNode *Parser::mul_op() {
    TreeNode *node = make_expr_node(ExprOp);
    node->attr.op = token_;
    this->match_token(token_);
    return node;
}
TreeNode *Parser::factor() {
    TreeNode *node = nullptr;
    switch (token_) {
        case TokenType::LPAREN:
            this->match_token(TokenType::LPAREN);
            node = this->expr();
            this->match_token(TokenType::RPAREN);
            break;
        case TokenType::NUM:
            node = make_expr_node(ExprConst);
            node->attr.val = strtol(token_str_.c_str(), nullptr, 10);
            this->match_token(TokenType::NUM);
            break;
        case TokenType::ID:
            node = make_expr_node(ExprIdentifier);
            node->attr.name = copy_str(token_str_);
            this->match_token(TokenType::ID);
            break;
        default:
            this->syntax_error("");
            break;
    }
    return node;
}

} /* namespace tinylang */
