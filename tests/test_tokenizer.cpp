#include <gtest/gtest.h>

#include "../src/tokenizer.h"

// -------------- Token Struct TEST --------------
TEST(TokenStructTest, CheckConstructors) {
    Token tok1 = Token();
    Token tok2 = Token("CREATE", Token::IDENTIFIER);

    ASSERT_EQ(tok1.value, "");
    ASSERT_EQ(tok1.type, Token::LITERAL);

    ASSERT_EQ(tok2.value, "CREATE");
    ASSERT_EQ(tok2.type, Token::IDENTIFIER);
}

// -------------- ts_tokenizer Functions Test --------------
// -------------- tokenize Functions Test --------------
TEST(tokenizeFunctionTest, BasicInput) { 
    // Basic check of function
    std::string line = "SELECT * FROM users WHERE id = 5";
    std::vector<Token> tokens_expect = {
        Token("SELECT", Token::KEYWORD), 
        Token("*", Token::SYMBOL), 
        Token("FROM", Token::KEYWORD), 
        Token("users", Token::IDENTIFIER), 
        Token("WHERE", Token::KEYWORD), 
        Token("id", Token::IDENTIFIER), 
        Token("=", Token::SYMBOL), 
        Token("5", Token::LITERAL), 
    };
    std::vector<Token> tokens_get = ts_tokenizer::tokenize(line);

    EXPECT_EQ(tokens_expect.size(), tokens_get.size());
    for(int i = 0; i < tokens_expect.size() && i < tokens_get.size(); i++)
    {
        EXPECT_EQ(tokens_get[i], tokens_expect[i]);
    }
}

TEST(tokenizeFunctionTest, BasicInputWithParentheses) { 
    // Check with parentheses
    std::string line = "CREATE TABLE users (col1 INT, col2 TEXT, col3 INT)";
    std::vector<Token> tokens_expect = {
        Token("CREATE", Token::KEYWORD), 
        Token("TABLE", Token::KEYWORD), 
        Token("users", Token::IDENTIFIER), 
        Token("col1", Token::IDENTIFIER), 
        Token("INT", Token::KEYWORD), 
        Token("col2", Token::IDENTIFIER), 
        Token("TEXT", Token::KEYWORD), 
        Token("col3", Token::IDENTIFIER), 
        Token("INT", Token::KEYWORD), 
    };
    std::vector<Token> tokens_get = ts_tokenizer::tokenize(line);

    EXPECT_EQ(tokens_expect.size(), tokens_get.size());
    for(int i = 0; i < tokens_expect.size() && i < tokens_get.size(); i++)
    {
        EXPECT_EQ(tokens_get[i], tokens_expect[i]);
    }
}

TEST(tokenizeFunctionTest, EscapingAndQuotes) { 

    // Check for Escaping and quotes
    std::string line = "INSERT INTO users VALUES ('user1', 123441, 'Jack\\'')";
    std::vector<Token> tokens_expect = {
        Token("INSERT", Token::KEYWORD), 
        Token("INTO", Token::KEYWORD), 
        Token("users", Token::IDENTIFIER), 
        Token("VALUES", Token::KEYWORD), 
        Token("'user1'", Token::LITERAL), 
        Token("123441", Token::LITERAL), 
        Token("'Jack''", Token::LITERAL), 
    };
    std::vector<Token> tokens_get = ts_tokenizer::tokenize(line);

    EXPECT_EQ(tokens_expect.size(), tokens_get.size());
    for(int i = 0; i < tokens_expect.size() && i < tokens_get.size(); i++)
    {
        EXPECT_EQ(tokens_get[i], tokens_expect[i]);
    }
}

// -------------- split_string Function Test --------------
TEST(split_stringFunctionTest, FunctionWorks) {
    // Basic check of function
    std::string line1 = "SELECT * FROM users WHERE id = 5";
    std::vector<std::string> splited1 = {"SELECT", "*", "FROM", "users", "WHERE", "id", "=", "5"};
    
    // Check with parentheses
    std::string line2 = "CREATE TABLE users (col1 INT, col2 TEXT, col3 INT)";
    std::vector<std::string> splited2 = {"CREATE", "TABLE", "users", "col1", "INT", "col2", "TEXT", "col3", "INT"};

    // Check for Escaping and quotes
    std::string line3 = "INSERT INTO users VALUES ('user1', 123441, 'Jack\\'')";
    std::vector<std::string> splited3 = {"INSERT", "INTO", "users", "VALUES", "'user1'", "123441", "'Jack''"};


    EXPECT_EQ(ts_tokenizer::split_string(line1), splited1);
    EXPECT_EQ(ts_tokenizer::split_string(line2), splited2);
    EXPECT_EQ(ts_tokenizer::split_string(line3), splited3);
}

// -------------- word_is_keyword Function Test --------------
TEST(word_is_keywordFunctionTest, FunctionWorks) { 
    // True Output Test
    EXPECT_EQ(ts_tokenizer::word_is_keyword("CREATE"), true);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("INTO"), true);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("TEXT"), true);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("TABLE"), true);

    // Empry Input Test
    EXPECT_EQ(ts_tokenizer::word_is_keyword(""), false);

    // False Output Test
    EXPECT_EQ(ts_tokenizer::word_is_keyword("create"), false);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("sdaasdasdsadasd"), false);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("CREATE "), false);
    EXPECT_EQ(ts_tokenizer::word_is_keyword("CREATE TABLE"), false);
}

// -------------- word_is_symbol Function Test --------------
TEST(word_is_symbolFunctionTest, FunctionWorks) { 
    // True Output Test
    EXPECT_EQ(ts_tokenizer::word_is_symbol("*"), true);
    EXPECT_EQ(ts_tokenizer::word_is_symbol("="), true);

    // Empry Input Test
    EXPECT_EQ(ts_tokenizer::word_is_symbol(""), false);

    // False Output Test
    EXPECT_EQ(ts_tokenizer::word_is_symbol("not symbol"), false);
    EXPECT_EQ(ts_tokenizer::word_is_symbol("******"), false);
    EXPECT_EQ(ts_tokenizer::word_is_symbol("*="), false);
}

// -------------- is_number Function Test --------------
TEST(word_is_literalFunctionTest, FunctionWorks) { 
    // True Output Test
    EXPECT_EQ(ts_tokenizer::word_is_literal("'mother'"), true);
    EXPECT_EQ(ts_tokenizer::word_is_literal("'Hello World'"), true);
    EXPECT_EQ(ts_tokenizer::word_is_literal("''"), true);
    EXPECT_EQ(ts_tokenizer::word_is_literal("12345"), true);
    EXPECT_EQ(ts_tokenizer::word_is_literal("222"), true);
    EXPECT_EQ(ts_tokenizer::word_is_literal("-1234"), true);

    // Empry Input Test
    EXPECT_EQ(ts_tokenizer::word_is_literal("CREATE"), false);
    EXPECT_EQ(ts_tokenizer::word_is_literal("a12345a"), false);
    EXPECT_EQ(ts_tokenizer::word_is_literal("My Name is Gtest"), false);

    // False Output Test
    EXPECT_EQ(ts_tokenizer::word_is_literal(""), false);
}

// -------------- is_number Function Test --------------
TEST(is_numberFunctionTest, FunctionWorks) {
    // True Output Test
    EXPECT_EQ(ts_tokenizer::is_number("-1234"), true);
    EXPECT_EQ(ts_tokenizer::is_number("0"), true);
    EXPECT_EQ(ts_tokenizer::is_number("1234"), true);
    EXPECT_EQ(ts_tokenizer::is_number("0003"), true);

    // Empry Input Test
    EXPECT_EQ(ts_tokenizer::is_number(""), false);

    // False Output Test
    EXPECT_EQ(ts_tokenizer::is_number("12345e"), false);
    EXPECT_EQ(ts_tokenizer::is_number("123s23"), false);
    EXPECT_EQ(ts_tokenizer::is_number("000000e"), false);
    EXPECT_EQ(ts_tokenizer::is_number("a0320"), false);
    EXPECT_EQ(ts_tokenizer::is_number("asmkfdsgfsd5"), false);
 }
 