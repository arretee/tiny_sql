#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <vector>

#include "tokenizer.h" // Including the Keywords and Symbols defines and Token struct
#include "table.h" // Include table and column struct

// Parser Commads
#define COMMAND_CREATE_TABLE "CREATE TABLE"
#define COMMAND_INSERT_INTO_VALUES "INSERT INTO VALUES"
#define COMMAND_SELECT_FROM "SELECT FROM"
#define COMMAND_DELETE_FROM "DELETE FROM"
#define COMMAND_DROP_TABLE "DROP TABLE"






// Command struct 
struct Command{
    // Variables
    std::string command;                        // One of defined strings in parser.h for engine commands
    std::string table_name;                     // Table name to work with
    std::vector<std::string> arguments;         // Default args like col names, values and so on. used for basic command without extintions of WHERE, ORDER BY
    std::unordered_map<std::string, std::vector<std::string>> special_args; // Special args for WHERE, ORDER BY and similars. 

    Command(std::string command, std::string table_name, std::vector<std::string> arguments);
    Command(std::string command, std::string table_name, std::vector<std::string> arguments,  std::unordered_map<std::string, std::vector<std::string>> special_args);

};


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



    /*
        Function parses WHERE keyword.

        input: tokens list, index where a WHERE keyword is used.
        output: vector of strings with 3 values -> {col name, EQUATION_SYMBOL, value}. if returned a empty vector -> error. 
    */
    std::vector<std::string> parse_where_special_arg(std::vector<Token>& tokens, int where_token_index);
}



#endif