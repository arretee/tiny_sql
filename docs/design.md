# TinySQL Design

## main.py 
program start.
User input is collected here, main loop and cover of meta commands also here.
### Steps of program
    User input collected
    User input is sent to tokenizer that return tokens 
    tokens are sent to parser that parse it to a command struct
    command sent to engine to execute the command
    engine calls the printer functions if needed to print somethin



## tokenizer
tokenize the user input -> split the string by the deviders, and create Tokens vector by each word
tokenizer only collects all the tokens from user input, do not check if it fit needed sequence or something, only create tokens by rules.

## parser
gets the token vector and parse the tokens vector to a command.
parser is responsible for the sequence of tokens -> it has to check if tokens are in needed sequence for command. 
if the command is CREATE TABLE -> parser have to check that tokens sequence is -> CREATE TABLE IDENTYFIER LITERAL KEYWORD LITERAL KEYWORD .....
parser returns struct Command, this command can be executed by engine. 

Command.command -> One of defined strings in parser.h for engine commands
Command.table_name -> name of table to work with
Command.arguments -> default arguments of the command
Command.WhereClause -> extructions to Where command 

## engine
engine gets the command that parser created, and execute it. 


## printer 
prints needed data on the screen. 