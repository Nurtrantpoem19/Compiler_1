#include "Tokenizer.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

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
