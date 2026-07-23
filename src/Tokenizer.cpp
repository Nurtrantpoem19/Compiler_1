#include "Tokenizer.hpp"
#include <cctype>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

Tokenizer::Tokenizer(const std::filesystem::path inputPath)
    : currentLine(""), token("")
{
    input.open(inputPath);
    if (!input.is_open())
    {
        std::cerr << "Error: Could not open file " << inputPath << std::endl;
    }
}

bool Tokenizer::hasMoreTokens()
{

    input >> std::ws;

    int nextChar = input.peek();
    if (nextChar == EOF)
    {
        return false;
    }

    if (nextChar == '/')
    {
        input.get();
        int secondChar = input.peek();

        if (secondChar == '/')
        {
            std::string temp;
            std::getline(input, temp);
            return hasMoreTokens();
        }
        else if (secondChar == '*')
        {
            input.get();
            char c1 = ' ', c2 = ' ';
            while (input.get(c2) && !((c1 == '*') && (c2 == '/')))
            {
                c1 = c2;
            }
            return hasMoreTokens();
        }
        else
        {
            input.unget();
            return true;
        }
    }
    return true;
}

void Tokenizer::advance()
{

    std::string buffer = "";
    char c = ' ';
    int peek = 0;

    if (!input.get(c))
    {
        return;
    }

    if (c == '"')
    {
        buffer += c;
        while (input.get(c))
        {
            buffer += c;
            if (c == '"')
            {
                token = buffer;
                buffer.clear();
                return;
            }
        }
    }

    if (delimiters.find(c) != delimiters.end())
    {
        buffer.push_back(c);
        token = buffer;
        return;
    }

    buffer += c;
    while (input.get(c))
    {
        if (delimiters.find(c) != delimiters.end())
        {
            input.unget();
            break;
        }

        buffer += c;
    }
    token = buffer;
    buffer.clear();
    return;
}

Tokenizer::TokenType Tokenizer::tokenType() const
{

    if (token.empty())
    {
        throw std::runtime_error("no token");
    }

    if (token.front() == '"' && token.back() == '"')
    {
        return TokenType::STRING_CONST;
    }

    if (token.length() == 1 && symbols.find(token[0]) != symbols.end())
    {
        return TokenType::SYMBOL;
    }

    if (keyword.find(token) != keyword.end())
    {
        return TokenType::KEYWORD;
    }

    bool isInt = true;
    for (const char &c : token)
    {
        if (!std::isdigit(c))
        {
            isInt = false;
        }
    }
    if (isInt)
    {
        return TokenType::INT_CONST;
    }

    return TokenType::IDENTIFIER;
}

Tokenizer::KeyWord Tokenizer::getKeyword() const
{

    static const std::unordered_map<std::string, Tokenizer::KeyWord> keyMap = {
        {"class", KeyWord::CLASS},
        {"method", KeyWord::METHOD},
        {"function", KeyWord::FUNCTION},
        {"constructor", KeyWord::CONSTRUCTOR},
        {"int", KeyWord::INT},
        {"boolean", KeyWord::BOOLEAN},
        {"char", KeyWord::CHAR},
        {"void", KeyWord::VOID},
        {"var", KeyWord::VAR},
        {"static", KeyWord::STATIC},
        {"field", KeyWord::FIELD},
        {"let", KeyWord::LET},
        {"do", KeyWord::DO},
        {"if", KeyWord::IF},
        {"else", KeyWord::ELSE},
        {"while", KeyWord::WHILE},
        {"return", KeyWord::RETURN},
        {"true", KeyWord::TRUE},
        {"false", KeyWord::FALSE},
        {"null", KeyWord::NULL_VAL},
        {"this", KeyWord::THIS}

    };

    auto it = keyMap.find(token);
    if (it != keyMap.end())
    {
        return it->second;
    }

    throw std::runtime_error("ERROR: not a keyword");
}

char Tokenizer::symbol() const
{
    char c = token[0];
    auto it = symbols.find(c);

    if ((token.length() == 1))
    {
        return c;
    }
    else
        throw std::runtime_error("ERROR: called symbol when not a symbol");
}

std::string Tokenizer::identifier() const
{

    if (tokenType() != Tokenizer::TokenType::IDENTIFIER)
    {
        throw std::runtime_error("ERROR: TokenType != identifier");
    }

    return token;
}

int Tokenizer::intVal() const
{
    if (tokenType() != Tokenizer::TokenType::INT_CONST)
    {
        throw std::runtime_error("ERROR: not an integer constant");
    }
    int value = std::stoi(token);

    return value;
}

std::string Tokenizer::stringVal() const
{

    if (tokenType() != Tokenizer::TokenType::STRING_CONST)
    {
        throw std::runtime_error("ERROR: Not a string constant");
    }

    if (token.length() >= 2 && token.front() == '"' && token.back() == '"')
    {
        return token.substr(1, token.length() - 2);
    }

    return token;
}
