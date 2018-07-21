/*
 * scanner.h
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <cstddef>

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

    TokenType getToken();

    void setInput(const char *input_data, size_t input_len);

private:
    char getNextChar();
    void putNextChar();

private:
    const char *input_data_ = nullptr;
    const char *current_ptr_ = nullptr;
    const char *input_end_ = nullptr;
}; /* class Scanner */

} /* namespace tinylang */


#endif /* !SCANNER_H */
