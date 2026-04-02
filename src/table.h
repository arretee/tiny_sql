#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <variant>
#include <vector>

// Structs for table logic
using Value = std::variant<int, std::string>;

struct Column {
    // Variables Decloration 
    std::string name;
    enum Type {INTEGER, TEXT} type;


    // Constructors
    Column(std::string name, Column::Type type);
    
};

struct Table { 
    // Variables Decloration 
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Value>> rows;

    // Constructors
    Table(std::string name, std::vector <Column> columns);
    Table(std::string name, std::vector <Column> columns, std::vector<std::vector<Value>> rows);
    
} ;


#endif