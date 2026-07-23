#include "Tokenizer.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

class TokenizerTest : public ::testing::Test
{
  protected:
    std::filesystem::path tempFilePath;

    void CreateTestFile(const std::string &contents)
    {
        tempFilePath =
            std::filesystem::temp_directory_path() / "temp_test_file.jack";
        std::ofstream outFile(tempFilePath);
        outFile << contents;
        outFile.close();
    }

    void TearDown() override
    {
        if (std::filesystem::exists(tempFilePath))
        {
            std::filesystem::remove(tempFilePath);
        }
    }
};

TEST_F(TokenizerTest, EmptyFileReturnsFalse)
{
    CreateTestFile("");

    Tokenizer tokenizer(tempFilePath);
    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, OnlyWhitespaceReturnsFalse)
{
    CreateTestFile("   \n\t  \n\n   ");

    Tokenizer tokenizer(tempFilePath);
    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, SingleTokenReturnsTrue)
{
    CreateTestFile("class");

    Tokenizer tokenizer(tempFilePath);
    EXPECT_TRUE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, LineCommentReturnsFalse)
{
    CreateTestFile("// This is a comment containing class and code keywords");

    Tokenizer tokenizer(tempFilePath);
    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, BlockCommentReturnsFalse)
{
    CreateTestFile(
        "/**\n * Multi-line API doc comment\n * * containing asterisks\n */");

    Tokenizer tokenizer(tempFilePath);
    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, MixedCodeAndCommentsReturnsTrue)
{
    std::string jackCode = "// Initialization line\n"
                           "   \t   \n"
                           "/** Block comment decoration */\n"
                           "class Main {\n"
                           "   field int x;\n"
                           "}";
    CreateTestFile(jackCode);

    Tokenizer tokenizer(tempFilePath);

    EXPECT_TRUE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesSpaceSeparatedTokens)
{
    CreateTestFile("class Main function");
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::string> expected = {"class", "Main", "function"};

    int i = 0;
    for (const auto &expectedToken : expected)
    {
        SCOPED_TRACE("after " + std::to_string(i++) +
                     " advance: " + expectedToken + "\n");
        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();
        EXPECT_EQ(tokenizer.getToken(), expectedToken);
    }

    // std::cout << tokenizer.getToken() << std::endl;
    // EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesMultipleLines_Advance)
{
    std::string testString = "class Main\n"
                             "{\n"
                             "    function void test()\n"
                             "}";
    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::string> expected = {
        "class", "Main", "{", "function", "void", "test", "(", ")", "}"};

    int i = 0;
    for (const auto &word : expected)
    {

        SCOPED_TRACE("Token index: " + std::to_string(i++) +
                     "and expecting: " + word + "\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();
        ASSERT_EQ(tokenizer.getToken(), word);
    }

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesStrings)
{
    std::string testString = "let s = \"Hello World!\";\n"
                             "let empty = \"\";";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::string> expected = {
        "let", "s",    "=", "\"Hello World!\"", ";", "let", "empty",
        "=",   "\"\"", ";"};

    int i = 0;
    for (const auto &word : expected)
    {
        SCOPED_TRACE("Token index: " + std::to_string(i++) +
                     " expecting: " + word + "\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();
        EXPECT_EQ(tokenizer.getToken(), word);
    }

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, TokenTypeChecks)
{
    std::string testString = "class 123 \"hello\" ; myVar";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::pair<std::string, Tokenizer::TokenType>> expected = {
        {"class", Tokenizer::TokenType::KEYWORD},
        {"123", Tokenizer::TokenType::INT_CONST},
        {"\"hello\"", Tokenizer::TokenType::STRING_CONST},
        {";", Tokenizer::TokenType::SYMBOL},
        {"myVar", Tokenizer::TokenType::IDENTIFIER}};

    int i = 0;
    for (const auto &[expectedToken, expectedType] : expected)
    {
        SCOPED_TRACE("token index: " + std::to_string(i++) +
                     " expecing: " + expectedToken + "\n");
        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();

        EXPECT_EQ(tokenizer.getToken(), expectedToken);
        EXPECT_EQ(tokenizer.tokenType(), expectedType);
    }
    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, KeyWordTypeChecks)
{
    std::string testString = "class constructor function method field static "
                             "var int char boolean void true false null this "
                             "let do if else while return";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::pair<std::string, Tokenizer::KeyWord>> expected = {
        {"class", Tokenizer::KeyWord::CLASS},
        {"constructor", Tokenizer::KeyWord::CONSTRUCTOR},
        {"function", Tokenizer::KeyWord::FUNCTION},
        {"method", Tokenizer::KeyWord::METHOD},
        {"field", Tokenizer::KeyWord::FIELD},
        {"static", Tokenizer::KeyWord::STATIC},
        {"var", Tokenizer::KeyWord::VAR},
        {"int", Tokenizer::KeyWord::INT},
        {"char", Tokenizer::KeyWord::CHAR},
        {"boolean", Tokenizer::KeyWord::BOOLEAN},
        {"void", Tokenizer::KeyWord::VOID},
        {"true", Tokenizer::KeyWord::TRUE},
        {"false", Tokenizer::KeyWord::FALSE},
        {"null", Tokenizer::KeyWord::NULL_VAL},
        {"this", Tokenizer::KeyWord::THIS},
        {"let", Tokenizer::KeyWord::LET},
        {"do", Tokenizer::KeyWord::DO},
        {"if", Tokenizer::KeyWord::IF},
        {"else", Tokenizer::KeyWord::ELSE},
        {"while", Tokenizer::KeyWord::WHILE},
        {"return", Tokenizer::KeyWord::RETURN}};

    int i = 0;
    for (const auto &[expectedToken, expectedKw] : expected)
    {
        SCOPED_TRACE("Token index " + std::to_string(i++) +
                     ": expecting keyword '" + expectedToken + "'\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();

        EXPECT_EQ(tokenizer.getToken(), expectedToken);
        EXPECT_EQ(tokenizer.tokenType(), Tokenizer::TokenType::KEYWORD);
        EXPECT_EQ(tokenizer.getKeyword(), expectedKw);
    }

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesSymbols)
{
    // Includes separated symbols, back-to-back symbols, and operators
    std::string testString = "{ } ( ) [ ] . , ; + - * / & | < > = ~ "
                             "{};()";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::string> expected = {
        "{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*",
        "/", "&", "|", "<", ">", "=", "~", "{", "}", ";", "(", ")"};

    int i = 0;
    for (const auto &expectedSym : expected)
    {
        SCOPED_TRACE("Token index: " + std::to_string(i++) +
                     " expected symbol is: " + expectedSym + "\n");
        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();
        EXPECT_EQ(tokenizer.tokenType(), Tokenizer::TokenType::SYMBOL);
        EXPECT_EQ(tokenizer.symbol(), expectedSym[0]);
    }
}

TEST_F(TokenizerTest, HandlesSymbolsInExpressions)
{
    std::string testString = "arr[i] = (x + 5) * (y - 3);";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    std::vector<std::string> expected = {"arr", "[", "i", "]", "=", "(",
                                         "x",   "+", "5", ")", "*", "(",
                                         "y",   "-", "3", ")", ";"};

    int i = 0;
    for (const auto expectedToken : expected)
    {
        SCOPED_TRACE("Token index: " + std::to_string(i++) +
                     " expecting token: '" + expectedToken + "'\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();

        if (tokenizer.tokenType() == Tokenizer::TokenType::SYMBOL &&
            (expectedToken.length() == 1))
        {
            EXPECT_EQ(tokenizer.symbol(), expectedToken[0]);
        }
        else
        {
            EXPECT_EQ(tokenizer.getToken(), expectedToken);
        }
    }

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, IdentifiersCheck)
{
    std::string teststring = "let square = Square.new(0,0,30);";

    CreateTestFile(teststring);

    Tokenizer tokenizer(tempFilePath);

    std::vector<std::pair<std::string, Tokenizer::TokenType>> expectedTokens = {
        {"let", Tokenizer::TokenType::KEYWORD},
        {"square", Tokenizer::TokenType::IDENTIFIER}, // Variable
        {"=", Tokenizer::TokenType::SYMBOL},
        {"Square", Tokenizer::TokenType::IDENTIFIER}, // Class name
        {".", Tokenizer::TokenType::SYMBOL},
        {"new", Tokenizer::TokenType::IDENTIFIER}, // Method/constructor name
        {"(", Tokenizer::TokenType::SYMBOL},
        {"0", Tokenizer::TokenType::INT_CONST},
        {",", Tokenizer::TokenType::SYMBOL},
        {"0", Tokenizer::TokenType::INT_CONST},
        {",", Tokenizer::TokenType::SYMBOL},
        {"30", Tokenizer::TokenType::INT_CONST},
        {")", Tokenizer::TokenType::SYMBOL},
        {";", Tokenizer::TokenType::SYMBOL}};

    int i = 0;
    for (const auto &[expectedString, expectedType] : expectedTokens)
    {
        ASSERT_TRUE(tokenizer.hasMoreTokens());

        tokenizer.advance();

        EXPECT_EQ(tokenizer.tokenType(), expectedType);
        if (expectedType == Tokenizer::TokenType::IDENTIFIER)
        {
            i++;
            EXPECT_EQ(tokenizer.identifier(), expectedString);
        }
    }
    EXPECT_EQ(i, 3);

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesIntConstants)
{
    std::string testString = "let numbers[0] = 32767;\n"
                             "let total = 100 + 42 - 0;";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    struct ExpectedToken
    {
        std::string tokenStr;
        Tokenizer::TokenType type;
        int expectedIntVal; // Only checked when type == INT_CONST
    };

    std::vector<ExpectedToken> expected = {
        {"let", Tokenizer::TokenType::KEYWORD, 0},
        {"numbers", Tokenizer::TokenType::IDENTIFIER, 0},
        {"[", Tokenizer::TokenType::SYMBOL, 0},
        {"0", Tokenizer::TokenType::INT_CONST, 0},
        {"]", Tokenizer::TokenType::SYMBOL, 0},
        {"=", Tokenizer::TokenType::SYMBOL, 0},
        {"32767", Tokenizer::TokenType::INT_CONST, 32767},
        {";", Tokenizer::TokenType::SYMBOL, 0},
        {"let", Tokenizer::TokenType::KEYWORD, 0},
        {"total", Tokenizer::TokenType::IDENTIFIER, 0},
        {"=", Tokenizer::TokenType::SYMBOL, 0},
        {"100", Tokenizer::TokenType::INT_CONST, 100},
        {"+", Tokenizer::TokenType::SYMBOL, 0},
        {"42", Tokenizer::TokenType::INT_CONST, 42},
        {"-", Tokenizer::TokenType::SYMBOL, 0},
        {"0", Tokenizer::TokenType::INT_CONST, 0},
        {";", Tokenizer::TokenType::SYMBOL, 0}};

    int i = 0;
    for (const auto &item : expected)
    {
        SCOPED_TRACE("Token index " + std::to_string(i++) + ": expecting '" +
                     item.tokenStr + "'\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();

        EXPECT_EQ(tokenizer.getToken(), item.tokenStr);
        EXPECT_EQ(tokenizer.tokenType(), item.type);

        if (item.type == Tokenizer::TokenType::INT_CONST)
        {
            EXPECT_EQ(tokenizer.intVal(), item.expectedIntVal);
        }
    }

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}

TEST_F(TokenizerTest, HandlesStringConstants)
{
    std::string testString = "do Output.printString(\"Hello World!\");\n"
                             "let msg = \"Jack Compiler 2026: Success!\";";

    CreateTestFile(testString);
    Tokenizer tokenizer(tempFilePath);

    struct ExpectedToken
    {
        std::string tokenStr;
        Tokenizer::TokenType type;
        std::string expectedStringVal;
    };

    std::vector<ExpectedToken> expected = {
        {"do", Tokenizer::TokenType::KEYWORD, ""},
        {"Output", Tokenizer::TokenType::IDENTIFIER, ""},
        {".", Tokenizer::TokenType::SYMBOL, ""},
        {"printString", Tokenizer::TokenType::IDENTIFIER, ""},
        {"(", Tokenizer::TokenType::SYMBOL, ""},
        {"\"Hello World!\"", Tokenizer::TokenType::STRING_CONST,
         "Hello World!"},
        {")", Tokenizer::TokenType::SYMBOL, ""},
        {";", Tokenizer::TokenType::SYMBOL, ""},
        {"let", Tokenizer::TokenType::KEYWORD, ""},
        {"msg", Tokenizer::TokenType::IDENTIFIER, ""},
        {"=", Tokenizer::TokenType::SYMBOL, ""},
        {"\"Jack Compiler 2026: Success!\"", Tokenizer::TokenType::STRING_CONST,
         "Jack Compiler 2026: Success!"},
        {";", Tokenizer::TokenType::SYMBOL, ""}};

    int j = 0;
    int i = 0;
    for (const auto &item : expected)
    {
        SCOPED_TRACE("Token index " + std::to_string(i++) + ": expecting '" +
                     item.tokenStr + "'\n");

        ASSERT_TRUE(tokenizer.hasMoreTokens());
        tokenizer.advance();

        EXPECT_EQ(tokenizer.getToken(), item.tokenStr);
        EXPECT_EQ(tokenizer.tokenType(), item.type);

        if (item.type == Tokenizer::TokenType::STRING_CONST)
        {
            j++;
            EXPECT_EQ(tokenizer.stringVal(), item.expectedStringVal);
        }
    }

    EXPECT_EQ(j, 2);

    EXPECT_FALSE(tokenizer.hasMoreTokens());
}
