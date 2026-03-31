#include "tokenizer.h"

// -------------- Help Functions --------------
bool is_number(std::string s);




// -------------- Token Constructors --------------
Token::Token(): value(""), type(Token::LITERAL){}
Token::Token(std::string value, Token::Type type): value(value), type(type) {}





// -------------- Tokenizer functions --------------
std::vector<Token> ts_tokenizer::tokenize(std::string line)
{
    // Variables declaration
    std::vector<Token> tokens = std::vector<Token>();
    Token token;
    int index = 0;


    // split line to word.
    std::vector<std::string> words = split_string(line);


    // Identify type of each word and create token for each word
    for (index = 0; index < words.size(); index++) {
        // Chekc if word is KEYWORD
        if(word_is_keyword(words[index])) {
            token = Token(words[index], Token::KEYWORD);
            tokens.push_back(token);
        }

        // Check if word is Symbol
        else if(word_is_symbol(words[index])) {
            token = Token(words[index], Token::SYMBOL);
            tokens.push_back(token);
        }

        // Check if word is a Literal
        else if(word_is_literal(words[index])){
            token = Token(words[index], Token::LITERAL);
            tokens.push_back(token);
        }

        // The last ones is a Identifier
        else {
            token = Token(words[index], Token::IDENTIFIER);
            tokens.push_back(token);
        }
    }


    return tokens;
}

std::vector<std::string> ts_tokenizer::split_string(std::string line){
    // Variables declaration
    std::vector<std::string> splited_line;
    int index = 0; 
    char c = ' ';

    bool inside_quote = false;
    bool skip_spliter = false; // flag for ESCAPING_CHAR

    std::string temp_string = "";

    // Split string on whitespace, parentheses, commas, and quotes and save to vector
    for (index = 0; index < line.length(); index ++)
    {
        c = line[index];

        // get word isnide quotes
        if (c == '\'' || inside_quote) {
            if (c == '\'' && inside_quote == false) {
                inside_quote = true;
            }
            else if (c == ESCAPING_CHAR && skip_spliter == false) {
                skip_spliter = true;
            }
            else if (c != '\'' || skip_spliter == true) {
                temp_string += c;
                skip_spliter = false;
            }
            else if (c == '\'' && inside_quote == true && skip_spliter == false) {
                splited_line.push_back("'" + temp_string + "'");
                temp_string = "";
                inside_quote = false;
            }
        }
        
        // Check if char is a splitter
        else if (c == ' ' || c == '(' || c == ')' || c == ',') {
            if (temp_string != "") {// if there is a word or string inside temp_string, add it to vector
                splited_line.push_back(temp_string);
                temp_string = "";
            }
        }

        else {
            temp_string += c;
        }
    }

    // Add the last word
    if(temp_string != ""){
        splited_line.push_back(temp_string);
    }

    return splited_line;
}


bool ts_tokenizer::word_is_keyword(std::string word)
{

    for(auto keyword: ALL_KEYWORDS_LIST)
    {
        if (word == keyword)
            return true;
    }

    return false;
}


bool ts_tokenizer::word_is_symbol(std::string word)
{
    for(auto symbol: ALL_SYMBOLS_LIST)
    {
        if (word.length() == 1)
            if (word[0] == symbol)
                return true;
    }

    return false;
}


bool ts_tokenizer::word_is_literal(std::string word)
{
    if (word.length() > 0)
        // if last and first char is a quote or the string is a number
        if((word[0] == '\'' && word.at(word.length() - 1) == '\'') || is_number(word))
        {
            return true;
        }

    return false;
}

// ----------------------- Help Functions -----------------------
bool is_number(std::string s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        std::stod(s, &pos); 
        return pos == s.length(); 
    } catch (...) {
        return false;
    }
}