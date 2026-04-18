#include "parser.h"

// -------------- Structs --------------
Command::Command(std::string command, std::string table_name, std::vector<std::string> arguments): command(command), table_name(table_name), arguments(arguments), whereClause(WhereClause()){ }
Command::Command(std::string command, std::string table_name, std::vector<std::string> arguments, WhereClause whereClause): command(command), table_name(table_name), arguments(arguments), whereClause(whereClause){ }



Condition::Condition(): column_name(""), op(Operator::EQ), value(0) {}
Condition::Condition(std::string column_name, Operator op, Value value) : column_name(column_name), op(op), value(value) {}


WhereClause::WhereClause(): condition(Condition()){}
WhereClause::WhereClause(Condition condition): condition(condition){}









// -------------- ts_parser function --------------

Command *ts_parser::parse_tokens(std::vector<Token> &tokens)
{
    // Check if the first word is an KEYWORD
    if (tokens.at(0).type != Token::KEYWORD){
        std::cout << "TinySQL Parser error: First token has to be KEYWORD" << std::endl;
        return nullptr;
    }

    // Move to needed function by first and second token
    if(tokens.at(0).value == KEYWORD_CREATE)
    {
        if (tokens.at(1).value == KEYWORD_TABLE)
            return ts_parser::parse_create_table(tokens);
    }
    else if (tokens.at(0).value == KEYWORD_INSERT)
    {
        if(tokens.at(1).value == KEYWORD_INTO)
            return ts_parser::parse_insert_into_values(tokens);
    }      
    else if (tokens.at(0).value == KEYWORD_SELECT)
    {
        return ts_parser::parse_select(tokens);
    }
    else if (tokens.at(0).value == KEYWORD_DELETE)
    {
        if(tokens.at(1).value == KEYWORD_FROM)
            return ts_parser::parse_delete_from(tokens);
    }
    else if (tokens.at(0).value == KEYWORD_DROP)
    {
        if (tokens.at(1).value == KEYWORD_TABLE)
            return ts_parser::parse_drop_table(tokens);
    }       
    

    std::cout << "TinySQL Parser error: Command not found by Keyword, use .help to find usefull instuctions." << std::endl;
    return nullptr;
}

Command *ts_parser::parse_create_table(std::vector<Token>& tokens) {
    // Commnads:
    // CREATE TABLE <name> (<col> <type>, ...)

    std::string table_name;
    std::vector<std::string> arguments;
    size_t tokens_size = tokens.size();
    int i = 0;


    // Check for minimum size of command
    if(tokens_size < 5)
    {
        std::cout << "TinySQL Parser error: Create table command need at least 5 words" << std::endl;
        std::cout << "CREATE TABLE <table_name> (<col_name> <type>, ....)" << std::endl;
        return nullptr;
    }

    // Check for correct name for table
    if (tokens.at(2).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: Table Name not Identifier" << std::endl;
        std::cout << "Identifier: name of somthing, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return nullptr;
    }

    table_name = tokens.at(2).value;

    // 3 -> Start index of cols and types arguments
    for(i = 3; i < tokens_size; i+=2)
    {
        // Check for column name valid
        if(i < tokens_size)
            if(tokens.at(i).type == Token::IDENTIFIER)
            {
                arguments.push_back(tokens.at(i).value);
            }
            else
            {
                std::cout << "TinySQL Parser error: Column Name not Identifier" << std::endl;
                std::cout << "The incorrect name -> " << tokens.at(i).value << std::endl; 
                return nullptr;
            }


        // Check if exists type for column
        if(i + 1 < tokens_size){
            // Check if type is valid
            if (tokens.at(i + 1).type == Token::KEYWORD || (tokens.at(i + 1).value == KEYWROD_DATATYPE_INTEGER || tokens.at(i + 1).value == KEYWROD_DATATYPE_TEXT))
            {
                arguments.push_back(tokens.at(i + 1).value);
            }
            else
            {
                std::cout << "TinySQL Parser error: type is not correct" << std::endl;
                std::cout << "type: INTEGER for integer, TEXT for text" << std::endl;
                return nullptr;
            }
        }

        else{
            std::cout << "TinySQL Parser error: Every column need type" << std::endl;
            std::cout << "type: INT for integer, TEXT for text" << std::endl;
            return nullptr;
        }
    }

    return new Command(COMMAND_CREATE_TABLE, table_name, arguments);
}

Command *ts_parser::parse_insert_into_values(std::vector<Token>& tokens) { 
    // Commnads:
    // INSERT INTO <name> VALUES (...)

    std::string table_name;
    std::vector<std::string> arguments;
    size_t tokens_size = tokens.size();
    int i = 0;

    // Check for minimum size of command
    if(tokens_size < 5)
    {
        std::cout << "TinySQL Parser error: INSERT INTO command need at least 5 words" << std::endl;
        std::cout << "INSERT INTO <name> VALUES (...)" << std::endl;
        return nullptr;
    }

    // Check for correct name of table
    if (tokens.at(2).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: Table Name not Identifier" << std::endl;
        std::cout << "Identifier: name of somthing, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return nullptr;
    }
    table_name = tokens.at(2).value;

    // Check fourth word == VALUES keyword
    if (tokens.at(3).value != KEYWORD_VALUES)
    {
        std::cout << "TinySQL Parser error: word on 4 place must be VALUES" << std::endl;
        std::cout << "INSERT INTO <name> VALUES (...)" << std::endl;
        return nullptr;
    }

    // start index of all values is on index 4
    for (i = 4; i < tokens_size; i++)
    {
        if(tokens.at(i).type != Token::LITERAL)
        {
            std::cout << "TinySQL Parser error: Value is not literal" << std::endl;
            std::cout << "Invalid value: " << tokens.at(i).value << std::endl; 
            std::cout << "literal: number or text inside singles quotes" << std::endl;
            return nullptr;
        }
        else
            arguments.push_back(tokens.at(i).value);
    }

    return new Command(COMMAND_INSERT_INTO_VALUES, table_name, arguments);
}

Command *ts_parser::parse_select(std::vector<Token>& tokens) { 
    // Commnads:
    // SELECT <cols|*> FROM <name>
    // SELECT <cols|*> FROM <name> WHERE <col> = <value>

    std::string table_name;
    std::vector<std::string> arguments;
    WhereClause whereClause;

    size_t tokens_size = tokens.size();
    int i = 0;

    // Check for minimum size of command
    if(tokens_size < 4)
    {
        std::cout << "TinySQL Parser error: SELECT command need at least 4 words" << std::endl;
        std::cout << "SELECT <cols|*> FROM <name>" << std::endl;
        return nullptr;
    }

    // Check if used symbol *
    if (tokens.at(1).value == SYMBOL_ALL)
    {
        arguments.push_back(SYMBOL_ALL);
    }

    // If not * get all columns names 
    else {
        // columns names starts at index 1 until index of token of FROM
        for(i = 1; i < tokens_size - 1 && tokens.at(i).value != KEYWORD_FROM; i++)
        {
            if(tokens.at(i).type == Token::IDENTIFIER)
            {
                arguments.push_back(tokens.at(i).value);
            }

            else
            {
                std::cout << "TinySQL Parser error: column name is not Identifier" << std::endl;
                std::cout << "Invalid column name: " << tokens.at(i).value;
                std::cout << "Identifier: name of something, do not use scrath numbers or single quotes for Identifier" << std::endl;
                return nullptr;
            }
        }
    }

    // move i to FROM token or to the end of vector
    for(i; i < tokens_size - 1 && tokens.at(i).value != KEYWORD_FROM; i++) {}

    // FROM keyword not found 
    if(tokens.at(i).value != KEYWORD_FROM)
    {
        std::cout << "TinySQL Parser error: SELECT command FROM keyword not found" << std::endl;
        std::cout << "SELECT <cols|*> FROM <name>" << std::endl;
        return nullptr;
    }

    // check that i index is not at the end of tokens vector, if yes -> table name not inputed
    if(i == tokens_size - 1)
    {
        std::cout << "TinySQL Parser error: table name is not found" << std::endl;
        std::cout << "SELECT <cols|*> FROM <name>" << std::endl;
        return nullptr;
    }

    // Check for correct name of table
    if (tokens.at(i + 1).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: Table Name not Identifier" << std::endl;
        std::cout << "Identifier: name of somthing, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return nullptr;
    }

    table_name = tokens.at(i + 1).value;

    // Check if WHERE used
    i += 2; // move to next token (after table name)
    if(i < tokens_size)
    {
        if(tokens.at(i).type == Token::KEYWORD && tokens.at(i).value == KEYWORD_WHERE)
        {
            whereClause = parse_where_special_arg(tokens, i); // Parse the special arguments of where

            // Check if WHERE parse is completed without errors
            if (whereClause.condition.column_name == "")
                return nullptr;
        }

        else
        {
            std::cout << "TinySQL Parser error: Unknown arguments after table name." << std::endl;
            std::cout << "Unknown argument: " << tokens.at(i).value << std::endl;
            return nullptr;
        }
    }



    return new Command(COMMAND_SELECT, table_name, arguments, whereClause);
}

Command *ts_parser::parse_delete_from(std::vector<Token>& tokens) { 
    // Commnads:
    // DELETE FROM <name>
    // DELETE FROM <name> WHERE <col> = <value>

    std::string table_name;
    std::vector<std::string> arguments;
    WhereClause whereClause;

    size_t tokens_size = tokens.size();
    int i = 0;

    // Check for minimum size of command
    if(tokens_size < 3)
    {
        std::cout << "TinySQL Parser error: DELETE FROM command need at least 3 words" << std::endl;
        std::cout << "SELECT <cols|*> FROM <name>" << std::endl;
        return nullptr;
    }


    // Check third token: table name
    if (tokens.at(2).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: Table Name not Identifier" << std::endl;
        std::cout << "Identifier: name of somthing, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return nullptr;
    }
    table_name = tokens.at(2).value;

    
    // Check if there is a spec args
    if (tokens_size > 3){
        // Check if there is a WHERE SPEC arg
        if(tokens.at(3).type == Token::KEYWORD && tokens.at(3).value == KEYWORD_WHERE)
        {
            whereClause = parse_where_special_arg(tokens, 3); // Parse the special arguments of where

            // Check if WHERE parse is completed without errors
            if (whereClause.condition.column_name == "")
                return nullptr;
        }

        else
        {
            std::cout << "TinySQL Parser error: Unknown arguments after table name." << std::endl;
            std::cout << "Unknown argument: " << tokens.at(3).value << std::endl;
            return nullptr;
        }
    }



    return new Command(COMMAND_DELETE_FROM, table_name, arguments, whereClause);
}

Command *ts_parser::parse_drop_table(std::vector<Token>& tokens) { 
    // Commnads:
    // DROP TABLE <name>

    std::string table_name;
    std::vector<std::string> arguments;
    size_t tokens_size = tokens.size();
    int i = 0;

    // Check for minimum size of command
    if(tokens_size != 3)
    {
        std::cout << "TinySQL Parser error: DROP command need exactly 3 words" << std::endl;
        std::cout << "DROP TABLE <name>" << std::endl;
        return nullptr;
    }

    // Check for correct name of table
    if (tokens.at(2).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: Table Name not Identifier" << std::endl;
        std::cout << "Identifier: name of somthing, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return nullptr;
    }
    table_name = tokens.at(2).value;
    

    return new Command(COMMAND_DROP_TABLE, table_name, arguments);
}

WhereClause ts_parser::parse_where_special_arg(std::vector<Token> &tokens, int where_token_index)
{
    size_t tokens_size = tokens.size();
    WhereClause clause;

    if(tokens_size - where_token_index < 4)
    {
        std::cout << "TinySQL Parser error: WHERE keyword requires 3 arguments" << std::endl; 
        std::cout << "... WHERE <col_name> = <value>" << std::endl; 
        return WhereClause();
    }

    // Chekc for col name is IDENTIFIER
    if (tokens.at(where_token_index + 1).type != Token::IDENTIFIER)
    {
        std::cout << "TinySQL Parser error: column name is not Identifier" << std::endl;
        std::cout << "Invalid column name: " << tokens.at(where_token_index + 1).value << std::endl;
        std::cout << "Identifier: name of something, do not use scrath numbers or single quotes for Identifier" << std::endl;
        return WhereClause();
    }
    clause.condition.column_name = tokens.at(where_token_index + 1).value;


    // Check for correct symbol
    if (tokens.at(where_token_index + 2).type != Token::SYMBOL || tokens.at(where_token_index + 2).value  == SYMBOL_ALL )
    {
        std::cout << "TinySQL Parser error: not used logical symbol: =, !=, >, >=, <, <=" << std::endl;
        std::cout << "... WHERE <col_name> <symbol> <value>" << std::endl;
        return WhereClause();
    }
    if (tokens.at(where_token_index + 2).value == SYMBOL_EQ) clause.condition.op = Operator::EQ;
    else if (tokens.at(where_token_index + 2).value == SYMBOL_NEQ) clause.condition.op = Operator::NEQ;
    else if (tokens.at(where_token_index + 2).value == SYMBOL_GT) clause.condition.op = Operator::GT;
    else if (tokens.at(where_token_index + 2).value == SYMBOL_GTE) clause.condition.op = Operator::GTE;
    else if (tokens.at(where_token_index + 2).value == SYMBOL_LT) clause.condition.op = Operator::LT;
    else if (tokens.at(where_token_index + 2).value == SYMBOL_LTE) clause.condition.op = Operator::LTE;


    // Check for correct value
    if (tokens.at(where_token_index + 3).type != Token::LITERAL)
    {
        std::cout << "TinySQL Parser error: Value is not literal" << std::endl;
        std::cout << "Invalid value: " << tokens.at(where_token_index + 3).value << std::endl; 
        std::cout << "literal: number or text inside singles quotes" << std::endl;
        return WhereClause();
    }
    clause.condition.value = convert_to_value(tokens.at(where_token_index + 3).value);


    return clause;
}

Value ts_parser::convert_to_value(std::string literal)
{
    Value ret;

    if (ts_tokenizer::is_number(literal))
    {
        ret = stoi(literal);
    }
    else
    {
        ret = literal.substr(1, literal.length() - 2); // remove quotes in value
    }


    return ret;
}
