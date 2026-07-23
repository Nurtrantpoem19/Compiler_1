#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

class Tokenizer
{
  private:
    std::ifstream input;
    std::string token;
    std::string currentLine;
    std::unordered_set<char> delimiters{' ', '\t', '\n', '\r', '(', ')', '[',
                                        ']', '{',  '}',  '.',  ',', ';', ':'};
    inline static const std::unordered_set<std::string> keyword = {
        "class", "constructor", "function", "method",  "field", "static",
        "var",   "int",         "char",     "boolean", "void",  "true",
        "false", "null",        "this",     "let",     "do",    "if",
        "else",  "while",       "return"};

    inline static const std::unordered_set<char> symbols = {
        '{', '}', '(', ')', '[', ']', '.', ',', ';', '+',
        '-', '*', '/', '&', '|', '<', '>', '=', '~'};

  public:
    enum class TokenType
    {
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST

    };

    enum class KeyWord
    {
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

    //@brief constructor for tokenizer, opens .jack file and gets ready to
    // tokenize
    explicit Tokenizer(const std::filesystem::path inputPath);

    //@brief checks if there are more tokens in the input
    bool hasMoreTokens();

    /**
     * @brief Gets the next token from the input, and makes it the current
     * token. This method should be called only if hasMoreTokens() is true.
     * Initially there is no current token.
     */
    void advance();

    /**
     * @brief Returns the type of the current token, as a constant.
     */
    TokenType tokenType() const;

    /**
     * @brief Returns the keyword which is the current token, as a constant.
     * This method should be called only if tokenType() is KEYWORD.
     */
    KeyWord getKeyword() const;

    /**
     * @brief Returns the character which is the current token.
     * Should becalled only if tokenType() is SYMBOL.
     */
    char symbol() const;

    /**
     * @brief Returns the string which is the current token.
     * Should be called only if tokenType() is IDENTIFIER.
     */
    std::string identifier() const;

    /**
     * @brief Returns the integer value of the current token.
     * Should be called only if tokenType() is INT_CONST.
     */
    int intVal() const;

    /**
     * @brief Returns the string value of the current token, without the opening
     * and closing double quotes. Should be called only if tokenType() is
     * STRING_CONST.
     */
    std::string stringVal() const;

    /*
     * @brief returns current token value
     */
    std::string getToken() { return token; }
};

#endif
