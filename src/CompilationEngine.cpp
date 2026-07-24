#include "CompilationEngine.hpp"
#include <iostream>
#include <stdexcept>

CompilationEngine::CompilationEngine(Tokenizer *inputTokenizer,
                                     const std::string &outputName)
    : tokenizer(inputTokenizer)
{

    output.open(outputName);
    if (!output.is_open())
    {
        std::cerr << "Error: Could not open output file: " << outputName
                  << std::endl;
    }
}

CompilationEngine::~CompilationEngine()
{
    if (output.is_open())
    {
        output.close();
    }
}

void CompilationEngine::compileClass()
{
    output << "<class>\n";
    if (tokenizer->tokenType() == Tokenizer::TokenType::KEYWORD &&
        tokenizer->getToken() == "class")
    {
        output << "<keyword> class </keyword>\n";
    }
    else
    {
        throw std::runtime_error("Compilation error: Expected 'class' keyword");
    }
    //-----------------------

    tokenizer->advance();
    if (tokenizer->tokenType() == Tokenizer::TokenType::IDENTIFIER)
    {
        output << "<identifier> " << tokenizer->getToken()
               << " </identifier>\n";
    }
    else
    {
        throw std::runtime_error("Compilation error: need class identifier");
    }

    //---------------------

    tokenizer->advance();
    if (tokenizer->tokenType() == Tokenizer::TokenType::SYMBOL &&
        (tokenizer->symbol() == '{'))
    {
        output << "<symbol> { </symbol>\n";
    }
    else
    {
        throw std::runtime_error("Compilation Error: missing symbol");
    }
    //--------------

    tokenizer->advance();
    while (tokenizer->tokenType() == Tokenizer::TokenType::KEYWORD &&
           ((tokenizer->getToken() == "static") ||
            (tokenizer->getToken() == "field")))
    {
        compileClassVarDec();
    }
    //------------------------
    //

    while (tokenizer->tokenType() == Tokenizer::TokenType::KEYWORD &&
           ((tokenizer->getToken() == "method") ||
            tokenizer->getToken() == "function" ||
            tokenizer->getToken() == "constructor"))
    {
        compileSubroutine();
    }

    if (tokenizer->tokenType() == Tokenizer::TokenType::SYMBOL &&
        tokenizer->symbol() == '}')
    {
        output << "<symbol> } </symbol>\n";
    }
    else
    {
        throw std::runtime_error("Compilation error: need closing brace");
    }

    tokenizer->advance();

    output << "</class>\n";
}

void CompilationEngine::compileClassVarDec()
{
    // TODO: Parse ('static' | 'field'), type, varName (',' varName)*, ';'
}

void CompilationEngine::compileSubroutine()
{
    // TODO: Parse ('constructor' | 'function' | 'method'),
    // ('void' | type), subroutineName, '(', parameterList, ')', subroutineBody
}

void CompilationEngine::compileParameterList()
{
    // TODO: Parse ((type varName) (',' type varName)*)?
    // Note: Does not handle enclosing parentheses '(' and ')'
}

void CompilationEngine::compileSubroutineBody()
{
    // TODO: Parse '{', varDec*, statements, '}'
}

void CompilationEngine::compileVarDec()
{
    // TODO: Parse 'var', type, varName (',' varName)*, ';'
}

// ============================================================================
// Statement Parsing Routines
// ============================================================================

void CompilationEngine::compileStatements()
{
    // TODO: Parse sequence of statements (let | if | while | do | return)*
    // Note: Does not handle enclosing curly brackets '{' and '}'
}

void CompilationEngine::compileLet()
{
    // TODO: Parse 'let', varName, ('[' expression ']')?, '=', expression, ';'
}

void CompilationEngine::compileIf()
{
    // TODO: Parse 'if', '(', expression, ')', '{', statements, '}',
    // ('else', '{', statements, '}')?
}

void CompilationEngine::compileWhile()
{
    // TODO: Parse 'while', '(', expression, ')', '{', statements, '}'
}

void CompilationEngine::compileDo()
{
    // TODO: Parse 'do', subroutineCall, ';'
}

void CompilationEngine::compileReturn()
{
    // TODO: Parse 'return', expression?, ';'
}

// ============================================================================
// Expression Parsing Routines
// ============================================================================

void CompilationEngine::compileExpression()
{
    // TODO: Parse term, (op term)*
}

void CompilationEngine::compileTerm()
{
    // TODO: Parse integerConstant | stringConstant | keywordConstant |
    // varName | varName'[' expression ']' | subroutineCall |
    // '(' expression ')' | unaryOp term
    //
    // Note: Use lookahead token ('[', '(', or '.') to distinguish identifier
    // usages.
}

int CompilationEngine::compileExpressionList()
{
    int expressionCount = 0;

    // TODO: Parse (expression (',' expression)*)?
    // Increment expressionCount for each expression found.

    return expressionCount;
}
