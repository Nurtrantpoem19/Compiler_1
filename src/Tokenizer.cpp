#include "Tokenizer.hpp"
#include <sstream>

Tokenizer::Tokenizer(const std::filesystem::path inputPath)
    : currentLine(""), token(0)
{
    input.open(inputPath);
    if (!input.is_open())
    {
        std::cerr << "Error: Could not open file " << inputPath << std::endl;
    }
}

// Checks if there are more tokens left to process
bool Tokenizer::hasMoreTokens()
{

    input >> std::ws;

    int nextChar = input.peek();
    if (nextChar == EOF)
    {
        return false;
    }
    else
    {
        if (nextChar == '/')
        {
            input.get();
            int secondChar = input.peek();

            if (secondChar == EOF)
            {
                return false;
            }
            else if (secondChar == '/')
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
    }
    return true;
}

// Advances to the next token, making it the current token
void Tokenizer::advance()
{

    std::string buffer = "";
    char c = ' ';
    int peek = 0;
    while (input.get(c))
    {
        peek = input.peek();
        if (delimiters.find(static_cast<char>(peek)) != delimiters.end())
        {
            if (!buffer.empty())
            {
                token.push_back(buffer);
                buffer.clear();
            }
        }
        else
        {
            buffer += (c);
        }
    }

    if (!buffer.empty())
    {
        token.push_back(buffer);
    }
}

// Returns the type of the current token
Tokenizer::TokenType Tokenizer::tokenType() const
{
    // TODO: Determine and return the actual type of the current token.
    return TokenType::KEYWORD;
}

// Returns the keyword of the current token
Tokenizer::KeyWord Tokenizer::keyword() const
{
    // TODO: Return the correct KeyWord enum value.
    // Should only be called if tokenType() is KEYWORD.
    return KeyWord::CLASS;
}

// Returns the single character of the current token
char Tokenizer::symbol() const
{
    // TODO: Return the current symbol character.
    // Should only be called if tokenType() is SYMBOL.
    return ' ';
}

// Returns the identifier string of the current token
std::string Tokenizer::identifier() const
{
    // TODO: Return the current identifier string.
    // Should only be called if tokenType() is IDENTIFIER.
    return "";
}

// Returns the integer value of the current token
int Tokenizer::intVal() const
{
    // TODO: Convert the current token string to an integer and return it.
    // Should only be called if tokenType() is INT_CONST.
    return 0;
}

// Returns the string constant value without the surrounding double quotes
std::string Tokenizer::stringVal() const
{
    // TODO: Return the string literal text, stripping away the starting and
    // ending " characters. Should only be called if tokenType() is
    // STRING_CONST.
    return "";
}
