#include "table.h"


// -------------- Column Constructors --------------
Column::Column(std::string name, Column::Type type) : name(name), type(type) {}



// -------------- Table Constructors --------------
Table::Table(std::string name, std::vector <Column> columns): name(name), columns(columns), rows(std::vector<std::vector<Value>>()) {}
Table::Table(std::string name, std::vector <Column> columns, std::vector<std::vector<Value>> rows): name(name), columns(columns), rows(rows){}

// -------------- Table Methods --------------
