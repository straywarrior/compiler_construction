/*
 * test_scanner.cpp
 * Copyright (C) 2018 StrayWarrior <i@straywarrior.com>
 *
 * Distributed under terms of the MIT license.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../scanner.h"

using namespace tinylang;

TEST_CASE( "Scanner::getToken correctness", "[Scanner]" ) {
    Scanner scanner;
    std::string input_data;
    input_data = "if (a < 0) then a := a + 1; end";
    scanner.setInput(input_data.c_str(), input_data.size());
    REQUIRE(scanner.getToken() == TokenType::IF);
    REQUIRE(scanner.getToken() == TokenType::LPAREN);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::LT);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::RPAREN);
    REQUIRE(scanner.getToken() == TokenType::THEN);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::ASSIGN);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::PLUS);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::SEMI);
    REQUIRE(scanner.getToken() == TokenType::END);
    REQUIRE(scanner.getToken() == TokenType::ENDFILE);

    input_data = "a1 := (186 - 23) / 2;";
    scanner.setInput(input_data.c_str(), input_data.size());
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::ASSIGN);
    REQUIRE(scanner.getToken() == TokenType::LPAREN);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::MINUS);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::RPAREN);
    REQUIRE(scanner.getToken() == TokenType::OVER);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::SEMI);
    REQUIRE(scanner.getToken() == TokenType::ENDFILE);

    input_data = "a2 := a1 * 5 / (a + a1); { this is a comment }";
    scanner.setInput(input_data.c_str(), input_data.size());
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::ASSIGN);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::TIMES);
    REQUIRE(scanner.getToken() == TokenType::NUM);
    REQUIRE(scanner.getToken() == TokenType::OVER);
    REQUIRE(scanner.getToken() == TokenType::LPAREN);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::PLUS);
    REQUIRE(scanner.getToken() == TokenType::ID);
    REQUIRE(scanner.getToken() == TokenType::RPAREN);
    REQUIRE(scanner.getToken() == TokenType::SEMI);
    REQUIRE(scanner.getToken() == TokenType::ENDFILE);
}
