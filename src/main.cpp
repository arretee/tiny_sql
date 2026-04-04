#include <iostream>
#include <unordered_map>
#include <string>

#include "table.h"
#include "tokenizer.h"
#include "parser.h"

#define EXIT_COMMAND ".quit"
#define TABLES_NAMES_PRINT_COMMAND ".tables"
#define TINY_SQL_HELP_COMMAND ".help"


// Functions declaration 
void print_welcome_msg();
void print_exit_message();
std::string get_user_input(std::string message);
void tables_print_meta_command(std::unordered_map<std::string, Table>& ref);

int main()
{
    // Variables Declaration 
    int quit_statemen = 0;
    std::string user_input = "";
    std::unordered_map<std::string, Table> tables_map;  // Hash map with all tables is memory
    std::vector<Token> tokens; 
    
    // Start the program logic
    print_welcome_msg();

    while (!quit_statemen){
        std::cout << "\n\n";
        user_input = get_user_input("TinySQL>>");
        
        // Check if input is more than 0 chars
        if (user_input.length() == 0)
            std::cout << "TinySQL input ERROR: inputed string length is 0." << std::endl;
        
        // Check for meta command
        else if (user_input[0] == '.')
        {
            if (user_input == EXIT_COMMAND)
            {
                quit_statemen++; // Switch quit statement for True
            }
            else if (user_input == TABLES_NAMES_PRINT_COMMAND)
            {
                tables_print_meta_command(tables_map);
            }
        }

        // Default SQL command 
        else 
        {  
            tokens = ts_tokenizer::tokenize(user_input);
            Command* com = ts_parser::parse_tokens(tokens);

            // temp
            for(int i = 0; i < tokens.size(); i++)
            {
                std::cout << tokens[i].value << " ----> Type: " << tokens[i].type << std::endl;
            }
            std::cout << "\n\n\n";

            if (com != nullptr)
            {   
                // Command and table name
                std::cout << com -> command << std::endl;
                std::cout << com -> table_name << std::endl;

                // Default args
                for(int i = 0; i < com->arguments.size(); i++) {
                    std::cout << com->arguments[i] << "\t";
                }
                
                std::cout << std::endl;


                // if there is a special args
                for (const auto& pair : com -> special_args) {
                    std::cout << pair.first << ": ";
                    for (int i = 0; i < com -> special_args[pair.first].size(); i++)
                    {
                        std::cout << com -> special_args[pair.first][i] << ", ";
                    }
                }


                std::cout << std::endl;

                delete com;
            }
            else std::cout<< "\n\nnullptr\n\n";
            //temp

        }
    }
    
    // Print exit message for user
    print_exit_message();
    

    return 0;       
}



/*
    Function prints welcome message to user for TinySQL 

    input: none
    output none
*/
void print_welcome_msg() {
    std::cout << "Welcome to TinySQL" << std::endl;
    std::cout << "It is a minimal SQL database engine" << std::endl;
    std::cout << "That runs entirely in memory" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Let's start ? You can use basic SQL statements like:" << std::endl;
    std::cout << "CREATE TABLE, INSERT INTO, SELECT, DELETE FROM, DROP TABLE" << std::endl;
    std::cout << "Algorithm uses SQL Escaping with char "<< ESCAPING_CHAR << " to escape chars inside TEXT data type" << std::endl;
    std::cout << "Also there is 3 commands not from SQL:" << std::endl;
    std::cout << TABLES_NAMES_PRINT_COMMAND << " -> Prints all tables names" << std::endl;
    std::cout << TINY_SQL_HELP_COMMAND << " -> Prints help info for TinySQL REPL" << std::endl;
    std::cout << EXIT_COMMAND <<" -> Program exit" << std::endl;
}

/*
    Function prints help message to user for TinySQL 

    input: none
    output: none
*/
void print_help_message() {
    std::cout << std::endl;
    std::cout << "CREATE TABLE <name> (<col> <type>, ...) -> To create a table" << std::endl;
    std::cout << "INSERT INTO <name> VALUES (...) -> to insert values into a specific table(number of values and types, have to fit number of columns and their types)" << std::endl;
    std::cout << "CREATE TABLE <name> (<col> <type>, ...) -> To create a table" << std::endl;
    std::cout << "SELECT <cols|*> FROM <name> -> prints matching columns for all rows. * symbol for all columns" << std::endl;
    std::cout << "DELETE FROM <name> WHERE <col> = <value> -> Removes matching rows from table" << std::endl;
    std::cout << "DROP TABLE <name> -> To delete a table" << std::endl;
    
    std::cout << "\nGood Luck" << std::endl;
}


/*
    Function prints exit message to user for TinySQL 

    input: none
    output: none
*/
void print_exit_message() {
    std::cout << std::endl;
    std::cout << "Quited successfuly" << std::endl;
    std::cout << "Thanks for using TinySQL" << std::endl;
    std::cout << "Have a nice day!" << std::endl;
}



/*
    Function prints message for user and gets the user input. 

    input: message to print before asking for input.
    output: user input string.

*/
std::string get_user_input(std::string message) {
    std::string input; 

    std:: cout << message; 
    std::getline(std::cin, input);

    return input;
}


/*
    Function that prints all tables names.

    input: reference to unordored map of tables
    output: none
*/
void tables_print_meta_command(std::unordered_map<std::string, Table>& ref) {
    std::cout << "Tables names list:" << std::endl;

    for (const auto& pair : ref){
        std::cout << pair.first << std::endl;
    }

    std::cout << std::endl;
}