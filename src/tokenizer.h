#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>



// -------------- Keywords of TinySQL --------------
#define KEYWORD_CREATE "CREATE"
#define KEYWORD_TABLE "TABLE"
#define KEYWORD_INSERT "INSERT"
#define KEYWORD_INTO "INTO"
#define KEYWORD_VALUES "VALUES"
#define KEYWORD_SELECT "SELECT"
#define KEYWORD_WHERE "WHERE"
#define KEYWORD_DELETE "DELETE"
#define KEYWORD_FROM "FROM"
#define KEYWORD_DROP "DROP"
#define KEYWROD_DATATYPE_INTEGER "INTEGER"
#define KEYWROD_DATATYPE_TEXT "TEXT"

#define ALL_KEYWORDS_LIST {KEYWORD_CREATE, KEYWORD_TABLE, KEYWORD_INSERT, KEYWORD_INTO, KEYWORD_VALUES, KEYWORD_SELECT, KEYWORD_WHERE, KEYWORD_DELETE, KEYWORD_FROM, KEYWORD_DROP, KEYWROD_DATATYPE_TEXT, KEYWROD_DATATYPE_INTEGER}


// -------------- Symbols of TinySQL --------------
#define SYMBOL_EQ "="
#define SYMBOL_NEQ "!="
#define SYMBOL_GT ">"
#define SYMBOL_LT "<"
#define SYMBOL_GTE ">="
#define SYMBOL_LTE "<="

#define SYMBOL_ALL "*"



#define ALL_SYMBOLS_LIST {SYMBOL_EQ, SYMBOL_NEQ, SYMBOL_ALL, SYMBOL_GT, SYMBOL_LT, SYMBOL_GTE, SYMBOL_LTE}

// -------------- Escaping char for TEXT --------------
#define ESCAPING_CHAR '\\'




// -------------- Token struct --------------
struct Token{
    // Variables Decloration
    std::string value;
    enum Type { KEYWORD, IDENTIFIER, LITERAL, SYMBOL} type;

    // Constructors
    Token();
    Token(std::string value, Token::Type type);


    // Operators
    bool operator==(const Token& token) const;
};




// -------------- Tokenizer functions --------------
namespace ts_tokenizer {
    // Create tokenizer namespace
    /*
        Creates vector of tokens from string line
        
        input: string line of tinySQL input
        output: vector of tokens.
    */
    std::vector<Token> tokenize(std::string line);

    /*
        Splits a command line into a vector of strings. 
        The splitters: whitespace, parentheses, commas, and quotes
        (if there is an whitespace, parentheses, commas inside quotes -> it is not count).
        Words or sentences that were in quotation marks will also be in quotation marks in the vector.
        escaping char \ for -> quote inside the quotes.
        example - input:'hell\'o', final token value:'hell'o'

        input: command line to split.
        output: vector of splitted line to word/symbols.
    */
    std::vector<std::string> split_string(std::string line);

    /*
        Checks if the word is a keyword of tinySQL

        input: string with word to check
        output: bool with true or false
    */
    bool word_is_keyword(std::string word);

    /*
        Checks if the word is a symbol of tinySQL

        input: string with word to check
        output: bool with true or false
    */
    bool word_is_symbol(std::string word);


    /*
        Chekcs if the word is a literal

        input: word
        output: bool with true of false
    */
    bool word_is_literal(std::string word);


    /*
        Function checks if string as a number

        input: string to check
        output: true if string is a number, false otherwise.
    */
    bool is_number(std::string s);
}



#endif