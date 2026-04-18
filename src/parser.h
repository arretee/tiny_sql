#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <vector>

#include "tokenizer.h" // Including the Keywords and Symbols defines and Token struct
#include "table.h" // Include table and column struct

// Parser Commads
#define COMMAND_CREATE_TABLE "CREATE TABLE"
#define COMMAND_INSERT_INTO_VALUES "INSERT INTO VALUES"
#define COMMAND_SELECT "SELECT"
#define COMMAND_DELETE_FROM "DELETE FROM"
#define COMMAND_DROP_TABLE "DROP TABLE"







// -------------- Condition Struct --------------
/*
    Used for Condition struct
*/
enum class Operator {
    EQ,
    NEQ,
    GT,
    LT,
    GTE,
    LTE
};
/*
    Logical condition expretion for some column
*/
struct Condition{
    // Variables
    std::string column_name;
    Operator op;
    Value value;

    Condition();
    Condition(std::string column_name, Operator op, Value value);

};

// -------------- Condition Struct --------------
struct WhereClause{
    Condition condition;


    WhereClause();
    WhereClause(Condition condition);
};


// -------------- Command struct  --------------

/*
    Struct with instructions for ts_engine (engine.h)

    // Variables
    std::string command;                        // One of defined strings in parser.h for engine commands
    
    std::string table_name;                     // Table name to work with
    
    std::vector<std::string> arguments;         // Default args like col names, values and so on. used for basic command without extintions of WHERE, ORDER BY
    
    WhereClause whereClause;                    // Where command instructions
*/
    struct Command{
    // Variables
    std::string command;                        // One of defined strings in parser.h for engine commands
    std::string table_name;                     // Table name to work with
    std::vector<std::string> arguments;         // Default args like col names, values and so on. used for basic command without extintions of WHERE, ORDER BY
    WhereClause whereClause;                    // Where command instructions
    Command(std::string command, std::string table_name, std::vector<std::string> arguments);
    Command(std::string command, std::string table_name, std::vector<std::string> arguments,  WhereClause whereClause);

};


/*
    Parser main purpose: Create from tokens that ts_tokenizer created, command for ts_engine.
    parser is responsible for get needed command, table name.
    parser is responsible for check that arguments got in needed order and count.
    parser is responsible to get the special args if needed for command.
    special args -> WHERE, ORDER BY .... 
*/
namespace ts_parser{
    // -------------- Parser Commands --------------
    /*
        Function that parses the tokens into a command object for engine.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_tokens(std::vector<Token>& tokens);

    
    // -------------- Function to parse every command --------------

    /*
        Function that parses the create table command.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_create_table(std::vector<Token>& tokens);

    /*
        Function that parses the insert into values command.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_insert_into_values(std::vector<Token>& tokens);
    
    /*
        Function that parses the select command.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_select(std::vector<Token>& tokens);
    
    /*
        Function that parses the delete from command.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_delete_from(std::vector<Token>& tokens);

    /*
        Function that parses the drop table command.
        If occured error during parse, error will be printed and returned nullptr.

        input: tokens from tokenizer.
        output: pointer to command or nullptr if error occured during parse. 
    */
    Command *parse_drop_table(std::vector<Token>& tokens);



    // -------------- Parse Special Arguments --------------
    /*
        Function parses WHERE keyword.

        input: tokens list, index where a WHERE keyword is used.
        output: vector of strings with 3 values -> {col name, EQUATION_SYMBOL, value}. if returned a empty vector -> error. 
    */
    WhereClause parse_where_special_arg(std::vector<Token>& tokens, int where_token_index);




    // -------------- Help Functions --------------

    Value convert_to_value(std::string literal);





}



#endif