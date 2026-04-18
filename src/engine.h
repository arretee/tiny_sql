#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <variant>



#include "parser.h" // Get COMMANDS constants strings, and Command struct
#include "table.h"  // Get table struct
#include "tokenizer.h"
#include "printer.h"




// -------------- Engine Fucntion --------------
namespace ts_engine {
    // -------------- Execute Functions --------------
    /*
        Function that call needed function from engine for command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors 

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_command(Command* com, std::unordered_map<std::string, Table>& tables);


    /*
        Function exequte create table command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_create_table(Command* com, std::unordered_map<std::string, Table>& tables);

    /*
        Function exequte insert into values command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_insert_into_values(Command* com, std::unordered_map<std::string, Table>& tables);

    /*
        Function exequte select command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_select(Command* com, std::unordered_map<std::string, Table>& tables);
    
    /*
        Function exequte delete from command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_delete_from(Command* com, std::unordered_map<std::string, Table>& tables);
    
    /*
        Function exequte drop table command.
        If error occured, function prints error message for user and returns null;
        *Recommended to use commands that ts_parser created, otherwise you will get errors

        input: pointer to Command to exequte, reference to tables unordered map
        output: null
    */
    void execute_drop_table(Command* com, std::unordered_map<std::string, Table>& tables);
    
    


    // -------------- Check Functions --------------
    /*
        Funtion gets an string with Literal, and get the type of the literal.
        *Expected that ts_tokenizer is specified a word like LITERAL! on tokenize step. 

        input: string with literal
        retunrn: Column::Type 
    */
    Column::Type get_literal_type(std::string literal);
}



#endif
