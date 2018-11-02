/*
 * parser.h
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

namespace tinylang {

enum NodeType {
    NodeStmt,
    NodeExpr
};

enum StmtProp {
    StmtIf,
    StmtRepeat,
    StmtAssign,
    StmtRead,
    StmtWrite
};

enum ExprProp {
    ExprOp,
    ExprConst,
    ExprIdentifier
};

/**
 * @brief enum for type checking
 */
enum ExprType {
    ExpVoid,
    ExpInteger,
    ExpBool
};

struct TreeNode {
    static constexpr int MAX_CHILDREN = 8;
    TreeNode *children[MAX_CHILDREN];
    TreeNode *neighbor = nullptr;
    int line_no;
    NodeType node_type;
    union {
        StmtProp stmt;
        ExprProp expr;
    }; // anonymous union for corresponding node_type
    union {
        TokenType op; // for Op expression
        int val;      // for Const expression
        char *name;   // for Identifier expression
    } attr; // un-named union for expression
    ExprType expr_type;

    void print();
};

/**
 * @brief Destroy the TreeNode recursively
 */
void destroyTreeNode(TreeNode *tree);

class Parser {
public:
    /** @brief
     * Initialize the parser
     */
    Parser();

    /** @brief Parse and return the syntax tree if success.
     *  The TreeNode pointer should be released by the caller.
     */
    TreeNode *parse(const char *input_data, size_t input_len);

    ~Parser();
private:
    /**
     * @brief Initialize the scanner and try to get the first token
     */
    void init_scanner(const char *input_data, size_t input_len) {
        scanner_->setInput(input_data, input_len);
        token_ = scanner_->getToken(&token_str_);
    }

    /**
     * @brief Compare the given token with the current lookahead token.
     *  Get next lookahead token if they matches or it throws error.
     *
     * @param token The token to compare with the lookahead.
     */
    void match_token(TokenType token);

    void syntax_error(const char *msg);


    TreeNode *make_stmt_node(StmtProp stmt_prop);
    TreeNode *make_expr_node(ExprProp expr_prop);

    // stmt_sequence -> statement {; statement}
    TreeNode *stmt_sequence();
    // statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
    TreeNode *statement();
    // if-stmt -> if exp then stmt-sequence [ else stmt-sequence ] end
    TreeNode *if_stmt();
    TreeNode *repeat_stmt();
    TreeNode *assign_stmt();
    TreeNode *read_stmt();
    TreeNode *write_stmt();
    // expression -> simple-expression [ comparison-op simple-expression ]
    TreeNode *expr();
    // simple-expression -> term { add-op term }
    TreeNode *simple_expr();
    // add-op -> + | -
    TreeNode *add_op();
    // term -> factor { mul-op factor }
    TreeNode *term();
    // mul-op -> * | /
    TreeNode *mul_op();
    // factor -> (exp) | number | identifier
    TreeNode *factor();

private:
    Scanner *scanner_ = nullptr;
    TokenType token_; // token for lookahead
    std::string token_str_;
};

} /* namespace tinylang */

#endif /* !PARSER_H */
