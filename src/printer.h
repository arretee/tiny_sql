#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <iomanip>


#include "table.h"




#define COLUMN_MIN_WIDTH 1







namespace ts_printer {


    /*
        Function that prints all tables names.

        input: reference to unordored map of tables
        output: none
    */
    void tables_print_meta_command(std::unordered_map<std::string, Table>& ref);


    /*
        Function that prints table with specific rows

        input: reference to table, vector if indexes of rows to print
        output: none
    */
    void print_table_with_rows(Table& table, std::vector<int> col_indexes, std::vector<int> row_indexes);

}









#endif