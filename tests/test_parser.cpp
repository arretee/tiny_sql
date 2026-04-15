#include <gtest/gtest.h>

#include "../src/parser.h"

// -------------- Command Struct Test --------------
TEST(CommnadStructTest, CheckConstructors) {
    std::string table_name = "users";
    std::vector<std::string> args = {"arg1", "arg2"};
    std::unordered_map<std::string, std::vector<std::string>> special_args;
    special_args["WHERE"] = {"arg1", "arg2"};


    Command c1(COMMAND_CREATE_TABLE, table_name, args);
    Command c2(COMMAND_CREATE_TABLE, table_name, args, special_args);

    // C1 Assertion
    EXPECT_EQ(c1.command, COMMAND_CREATE_TABLE);
    EXPECT_EQ(c1.table_name, table_name);
    EXPECT_EQ(c1.arguments, args);


    // C2 Assertion
    EXPECT_EQ(c2.command, COMMAND_CREATE_TABLE);
    EXPECT_EQ(c2.table_name, table_name);
    EXPECT_EQ(c2.arguments, args);
    EXPECT_EQ(c2.special_args, special_args);
    EXPECT_EQ(c2.special_args["WHERE"], special_args["WHERE"]);
}


// ---------------------------- ts_parser Functions Test ----------------------------

// -------------- parse_tokens Function Test --------------
// No need to test basic if else function.
// If function will grow to something bigger -> Write test. 

// -------------- parse_create_table Function Test --------------
TEST(parse_create_tableFunctionTest, FunctionWorks) { 
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;

    std::string expected_command;
    std::string expected_table_name;
    std::vector<std::string> expected_args;

    std::unordered_map<std::string, std::vector<std::string>> expected_special_args;
    std::vector<std::string> where_args;

    Command* c = nullptr;


    // -------------- Test correct input with all symbol --------------
    tokens = {
        Token(KEYWORD_CREATE, Token::KEYWORD),
        Token(KEYWORD_TABLE, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token("col1", Token:: IDENTIFIER),
        Token(KEYWROD_DATATYPE_INT, Token::KEYWORD),
        Token("col2", Token:: IDENTIFIER),
        Token(KEYWROD_DATATYPE_TEXT, Token::KEYWORD),
    };

    expected_command = COMMAND_CREATE_TABLE;
    expected_table_name = "users";
    expected_args = {"col1", KEYWROD_DATATYPE_INT, "col2", KEYWROD_DATATYPE_TEXT};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_create_table(tokens);  // Call function 
    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input";
    }
}

// -------------- parse_insert_into_values Function Test --------------
TEST(parse_insert_into_valuesFunctionTest, FunctionWorks) { 
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;

    std::string expected_command;
    std::string expected_table_name;
    std::vector<std::string> expected_args;

    std::unordered_map<std::string, std::vector<std::string>> expected_special_args;
    std::vector<std::string> where_args;

    Command* c = nullptr;


    // -------------- Test correct input with all symbol --------------
    tokens = {
        Token(KEYWORD_SELECT, Token::KEYWORD),
        Token(KEYWORD_INTO, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_VALUES, Token::KEYWORD),
        Token("'hello world'", Token::LITERAL),
        Token("'how are you ?'", Token::LITERAL),
        Token("5", Token::LITERAL),

    };

    expected_command = COMMAND_INSERT_INTO_VALUES;
    expected_table_name = "users";
    expected_args = {"'hello world'", "'how are you ?'", "5"};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_insert_into_values(tokens);  // Call function 
    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input";
    }
}

// -------------- parse_select Function Test --------------
TEST(parse_selectFunctionTest, FunctionWorks) {
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;

    std::string expected_command;
    std::string expected_table_name;
    std::vector<std::string> expected_args;

    std::unordered_map<std::string, std::vector<std::string>> expected_special_args;
    std::vector<std::string> where_args;

    Command* c = nullptr;

    // -------------- Test correct input with all symbol --------------
    tokens = {
        Token(KEYWORD_SELECT, Token::KEYWORD),
        Token(SYMBOL_ALL, Token::SYMBOL),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
    };

    expected_command = COMMAND_SELECT_FROM;
    expected_table_name = "users";
    expected_args = {SYMBOL_ALL};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_select(tokens);  // Call function 

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input with all symbol test without spec args";
    }

    // -------------- Test correct input with cols names --------------
    tokens = {
        Token(KEYWORD_SELECT, Token::KEYWORD),
        Token("col1", Token::IDENTIFIER),
        Token("col2", Token::IDENTIFIER),
        Token("col3", Token::IDENTIFIER),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
    };

    expected_command = COMMAND_SELECT_FROM;
    expected_table_name = "users";
    expected_args = {"col1", "col2", "col3"};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_select(tokens);  // Call function 

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input with cols names without spec args";
    }


    // -------------- Test correct input with cols names and where special arg --------------
    tokens = {
        Token(KEYWORD_SELECT, Token::KEYWORD),
        Token("col1", Token::IDENTIFIER),
        Token("col2", Token::IDENTIFIER),
        Token("col3", Token::IDENTIFIER),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token("id", Token::IDENTIFIER),
        Token(SYMBOL_EQUATION, Token::SYMBOL),
        Token("5", Token::LITERAL),
    };

    expected_command = COMMAND_SELECT_FROM;
    expected_table_name = "users";
    expected_args = {"col1", "col2", "col3"};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 
    where_args = {"id", SYMBOL_EQUATION, "5"};
    expected_special_args[KEYWORD_WHERE] = where_args;

    c = ts_parser::parse_select(tokens);  // Call function 

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);

        // special args
        try{
            EXPECT_EQ(c->special_args[KEYWORD_WHERE], where_args);
        } catch(...) { }

        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input with all test with spec args";
    }

}

// -------------- parse_delete_from Function Test --------------
TEST(parse_delete_fromFunctionTest, FunctionWorks) {
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;

    std::string expected_command;
    std::string expected_table_name;
    std::vector<std::string> expected_args;

    std::unordered_map<std::string, std::vector<std::string>> expected_special_args;
    std::vector<std::string> where_args;

    Command* c = nullptr;

    // Test correct input
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
    };

    expected_command = COMMAND_DELETE_FROM;
    expected_table_name = "users";
    expected_args = {};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_delete_from(tokens);  // Call function 

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input test without spec args";
    }



    // Test correct input with WHERE
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token("id", Token::IDENTIFIER),
        Token(SYMBOL_EQUATION, Token::SYMBOL),
        Token("5", Token::LITERAL),
    };

    expected_command = COMMAND_DELETE_FROM;
    expected_table_name = "users";
    expected_args = {};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 
    where_args = {"id", SYMBOL_EQUATION, "5"};
    expected_special_args[KEYWORD_WHERE] = where_args;

    c = ts_parser::parse_delete_from(tokens); // Call function 

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);

        // special args
        try{
            EXPECT_EQ(c->special_args[KEYWORD_WHERE], where_args);
        } catch(...) { }

        delete(c);

    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in correct input test with WHERE";
    }


}

// -------------- parse_drop_table Function Test --------------
TEST(parse_drop_tableFunctionTest, FunctionWorks) {
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;

    std::string expected_command;
    std::string expected_table_name;
    std::vector<std::string> expected_args;
    std::unordered_map<std::string, std::vector<std::string>> expected_special_args;

    Command* c = nullptr;

    // Test correct input
    tokens = {
        Token(KEYWORD_DROP, Token::KEYWORD),
        Token(KEYWORD_TABLE, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
    };

    expected_command = COMMAND_DROP_TABLE;
    expected_table_name = "users";
    expected_args = {};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_drop_table(tokens);

    if (c != nullptr)
    {
        EXPECT_EQ(c->command, expected_command);
        EXPECT_EQ(c->table_name, expected_table_name);
        EXPECT_EQ(c->arguments, expected_args);
        EXPECT_EQ(c->special_args, expected_special_args);
        delete(c);
    }
    else
    {
        EXPECT_EQ(1, 2) << "returned pointer is nullptr in Valid input test";
    }


    // Test incorrect table name
    tokens = {
        Token(KEYWORD_DROP, Token::KEYWORD),
        Token(KEYWORD_TABLE, Token::KEYWORD),
        Token("'users'", Token::LITERAL),
    };

    expected_command = COMMAND_DROP_TABLE;
    expected_table_name = "users";
    expected_args = {};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_drop_table(tokens);

    EXPECT_EQ(c, nullptr);


    // Test without table name
    tokens = {
        Token(KEYWORD_DROP, Token::KEYWORD),
        Token(KEYWORD_TABLE, Token::KEYWORD),
    };

    expected_command = COMMAND_DROP_TABLE;
    expected_table_name = "users";
    expected_args = {};
    expected_special_args = std::unordered_map<std::string, std::vector<std::string>>(); 

    c = ts_parser::parse_drop_table(tokens);

    EXPECT_EQ(c, nullptr);

    if(c != nullptr) delete(c);
    

}

// -------------- parse_where_special_arg Function Test --------------
TEST(parse_where_special_argFunctionTest, FunctionWorks) {
    std::cout.setstate(std::ios_base::failbit); // Turn off cout in function

    std::vector<Token> tokens;
    std::vector<std::string> expected;

    // Test correct input
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token("id", Token::IDENTIFIER),
        Token(SYMBOL_EQUATION, Token::SYMBOL),
        Token("5", Token::LITERAL),
    };
    expected = {"id", SYMBOL_EQUATION, "5"};
    ASSERT_EQ(ts_parser::parse_where_special_arg(tokens, 3), expected);


    // Test input without value
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token("id", Token::IDENTIFIER),
        Token(SYMBOL_EQUATION, Token::SYMBOL),
    };
    expected = {};
    ASSERT_EQ(ts_parser::parse_where_special_arg(tokens, 3), expected);


    // Test input without col name
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token(SYMBOL_EQUATION, Token::SYMBOL),
        Token("5", Token::LITERAL),
    };
    expected = {};
    ASSERT_EQ(ts_parser::parse_where_special_arg(tokens, 3), expected);

    // Test with another symbol 
    tokens = {
        Token(KEYWORD_DELETE, Token::KEYWORD),
        Token(KEYWORD_FROM, Token::KEYWORD),
        Token("users", Token::IDENTIFIER),
        Token(KEYWORD_WHERE, Token::KEYWORD),
        Token("id", Token::IDENTIFIER),
        Token(SYMBOL_ALL, Token::SYMBOL),
        Token("5", Token::LITERAL),
    };
    expected = {};
    ASSERT_EQ(ts_parser::parse_where_special_arg(tokens, 3), expected);
}