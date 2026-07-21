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
