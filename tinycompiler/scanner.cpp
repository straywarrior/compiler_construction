/*
 * scanner.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "scanner.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace tinylang {

enum StateType {
    START = 0,
    IN_COMMENT = 1,
    IN_NUM = 2,
    IN_IDENTIFIER = 3,
    IN_ASSIGN = 4,
    DONE = 5,
    SCAN_STATE_END_FLAG = 6,
};

enum SymbolType {
    WHITE_SPACE = 0,
    LETTER = 1,
    DIGIT = 2,
    COLON = 3,
    EQUAL = 4,
    LEFT_BRACE = 5,
    RIGHT_BRACE = 6,
    OTHER = 7,
    SYMBOL_TYPE_END_FLAG = 8,
};

static std::vector<std::vector<StateType>> transition_table;
static std::unordered_map<std::string, TokenType> reserved_words;

static void initTransitionTable();

bool is_letter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t';
}

static SymbolType getSymbolType(char c) {
    if (is_letter(c)) {
        return SymbolType::LETTER;
    } else if (is_digit(c)) {
        return SymbolType::DIGIT;
    } else if (is_whitespace(c)) {
        return SymbolType::WHITE_SPACE;
    } else if (c == ':') {
        return SymbolType::COLON;
    } else if (c == '=') {
        return SymbolType::EQUAL;
    } else if (c == '{') {
        return SymbolType::LEFT_BRACE;
    } else if (c == '}') {
        return SymbolType::RIGHT_BRACE;
    } else {
        return SymbolType::OTHER;
    }
}

Scanner::Scanner() {
    initTransitionTable();
}

TokenType Scanner::getToken() {
    StateType state = StateType::START;
    StateType last_state;
    TokenType token = TokenType::ERROR;
    std::string token_string;
    while (state != StateType::DONE) {
        char c = this->getNextChar();
        SymbolType symbol_type = getSymbolType(c);
        last_state = state;
        if (state == StateType::IN_IDENTIFIER &&
            !(symbol_type == LETTER || symbol_type == DIGIT)) {
            this->putNextChar();
        } else if (state == StateType::IN_NUM && symbol_type != DIGIT) {
            this->putNextChar();
        } else {
            token_string.push_back(c);
        }
        state = transition_table[state][symbol_type];
        if (state == StateType::START){
            token_string.clear();
        }
    }
    switch (last_state) {
        case StateType::START:
            if (reserved_words.find(token_string) != reserved_words.end()) {
                token = reserved_words[token_string];
            }
            break;
        case StateType::IN_NUM:
            token = TokenType::NUM;
            break;
        case StateType::IN_IDENTIFIER:
            if (reserved_words.find(token_string) != reserved_words.end()) {
                token = reserved_words[token_string];
            } else {
                token = TokenType::ID;
            }
            break;
        case StateType::IN_ASSIGN:
            token = TokenType::ASSIGN;
            break;
        default:
            break;
    }
    return token;
}

char Scanner::getNextChar() {
    if (input_data_ != nullptr && current_ptr_ != input_end_) {
        return *(current_ptr_++);
    } else {
        return '\0';
    }
}

void Scanner::putNextChar() {
    --current_ptr_;
}

void Scanner::setInput(const char *input_data, size_t input_len) {
    input_data_ = input_data;
    current_ptr_ = input_data_;
    if (input_data_ != nullptr) {
        input_end_ = input_data_ + input_len;
    }
}

static void initTransitionTable() {
    if (transition_table.size())
        return;
    transition_table.resize(StateType::SCAN_STATE_END_FLAG);
    for (auto &row : transition_table) {
        row.resize(SymbolType::SYMBOL_TYPE_END_FLAG, StateType::DONE);
    }

    auto init_vector = [](std::vector<StateType> & v, StateType t) {
        std::fill(v.begin(), v.end(), t);
    };
    transition_table[StateType::START][SymbolType::WHITE_SPACE] = StateType::START;
    transition_table[StateType::START][SymbolType::LEFT_BRACE] = StateType::IN_COMMENT;
    transition_table[StateType::START][SymbolType::DIGIT] = StateType::IN_NUM;
    transition_table[StateType::START][SymbolType::LETTER] = StateType::IN_IDENTIFIER;
    transition_table[StateType::START][SymbolType::COLON] = StateType::IN_ASSIGN;
    transition_table[StateType::START][SymbolType::OTHER] = StateType::DONE;
    init_vector(transition_table[StateType::IN_COMMENT], StateType::IN_COMMENT);
    transition_table[StateType::IN_COMMENT][SymbolType::RIGHT_BRACE] = StateType::START;
    transition_table[StateType::IN_NUM][SymbolType::DIGIT] = StateType::IN_NUM;
    transition_table[StateType::IN_NUM][SymbolType::OTHER] = StateType::DONE;
    transition_table[StateType::IN_IDENTIFIER][SymbolType::DIGIT] = StateType::IN_IDENTIFIER;
    transition_table[StateType::IN_IDENTIFIER][SymbolType::LETTER] = StateType::IN_IDENTIFIER;
    transition_table[StateType::IN_IDENTIFIER][SymbolType::OTHER] = StateType::DONE;
    transition_table[StateType::IN_ASSIGN][SymbolType::EQUAL] = StateType::DONE;
    transition_table[StateType::IN_ASSIGN][SymbolType::OTHER] = StateType::DONE;

    reserved_words["if"] = TokenType::IF;
    reserved_words["then"] = TokenType::THEN;
    reserved_words["else"] = TokenType::ELSE;
    reserved_words["end"] = TokenType::END;
    reserved_words["repeat"] = TokenType::REPEAT;
    reserved_words["until"] = TokenType::UNTIL;
    reserved_words["read"] = TokenType::READ;
    reserved_words["write"] = TokenType::WRITE;
    reserved_words["="] = TokenType::EQ;
    reserved_words["<"] = TokenType::LT;
    reserved_words["+"] = TokenType::PLUS;
    reserved_words["-"] = TokenType::MINUS;
    reserved_words["*"] = TokenType::TIMES;
    reserved_words["/"] = TokenType::OVER;
    reserved_words["("] = TokenType::LPAREN;
    reserved_words[")"] = TokenType::RPAREN;
    reserved_words[";"] = TokenType::SEMI;
}


} /* namespace tinylang */
