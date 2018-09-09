/*
 * scanner.h
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <cstddef>
#include <string>

namespace tinylang {
/*
 * Three typical categories of tokens: reserved words, special symbols, "other"
 * Reserved words: if, then, else, ...
 * Special symbols: +, -, *, /, =, :=, ;, ...
 * Other: identifier, numbers
 */
enum class TokenType {
    /* book-keeping tokens */
    ENDFILE,ERROR,
    /* reserved words */
    IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    ASSIGN,EQ,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI
};

class Scanner {
public:
    Scanner();

    /**
     * @brief Try to get next token
     *
     * @param token_str The token string will be set if token_str != nullptr
     */
    TokenType getToken(std::string *token_str = nullptr);

    void setInput(const char *input_data, size_t input_len);

    size_t current_line_no() const { return line_number_; }

private:
    char getNextChar();
    void putNextChar();
    void setNextLine();

private:
    const char *input_data_ = nullptr;
    const char *current_ptr_ = nullptr;
    const char *line_end_ = nullptr;
    const char *input_end_ = nullptr;
    size_t line_number_ = 0;
}; /* class Scanner */

} /* namespace tinylang */


#endif /* !SCANNER_H */
