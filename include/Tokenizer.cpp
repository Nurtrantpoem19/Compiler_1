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
};
