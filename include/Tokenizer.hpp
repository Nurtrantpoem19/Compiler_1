#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Tokenizer {
private:
  std::ifstream input;
  std::vector<std::string> token;
  enum class TokenType {
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST

  };

  enum class KeyWord {
    CLASS,
    METHOD,
    FUNCTION,
    CONSTRUCTOR,
    INT,
    BOOLEAN,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    NULL_VAL,
    THIS
  };
};

#endif TOKENIZER_HPP
