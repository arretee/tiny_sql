#include "printer.h"

void ts_printer::tables_print_meta_command(std::unordered_map<std::string, Table> &ref)
{
    std::cout << "Tables names list:" << std::endl;

    for (const auto& pair : ref){
        std::cout << pair.first << std::endl;
    }

    std::cout << std::endl;
}

void ts_printer::print_table_with_rows(Table &table, std::vector<int> col_indexes, std::vector<int> row_indexes)
{
    size_t columns_to_print = col_indexes.size();
    size_t rows_to_print = row_indexes.size();
    int col_index;
    int row_index;

    

    // calculate width for every col
    std::vector<size_t> widths;
    Value value_to_check;
    std::string str_value;

    size_t max_width = COLUMN_MIN_WIDTH;
    size_t str_width = 0;


    for (int col_index: col_indexes)
    {
        max_width = COLUMN_MIN_WIDTH;

        // Check with title of col 
        str_width = table.columns[col_index].name.length();
        max_width = std::max(str_width, max_width);

        // Check with every row
        for (int row_index: row_indexes)
        {
            value_to_check = table.rows.at(row_index).at(col_index);    // Get value from row

            // Get data from std::variant
            if (std::holds_alternative<std::string>(value_to_check)) {
                str_value = std::get<std::string>(value_to_check);
            }
            if (std::holds_alternative<int>(value_to_check)) {
                str_value = std::to_string(std::get<int>(value_to_check));
            }

            str_width = str_value.length();
                    max_width = std::max(str_width, max_width);

        }


        // Add max width to vector 
        widths.push_back(max_width);
    }



    // -------- Print the table --------

    // Print table label and titles
    // Print decorative 
    for (int col = 0; col < columns_to_print; col ++)
    {
        std::cout << "+-";
        for (int i = 0; i < widths[col]; i++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+" << std::endl;


    // Print table cols names
    for (int col_num = 0; col_num < columns_to_print; col_num++)
    {
        col_index = col_indexes[col_num];
        std::cout << "|"; 
        std::cout << std::setw(widths[col_num] + 2) << std::left << table.columns[col_index].name;
    }
    std::cout << "|" << std::endl;



    // Print rows 
    for (int row_index: row_indexes)
    {
        // Print decorative 
        for (int col_num = 0; col_num < columns_to_print; col_num++)
        {
            std::cout << "+-";
            for (int i = 0; i < widths[col_num]; i++)
                std::cout << "-";
            std::cout << "-";
        }
        std::cout << "+" << std::endl;

        // Print row 
        for (int col_num = 0; col_num < columns_to_print; col_num++)
        {
            col_index = col_indexes[col_num];


            std::cout << "|"; 
            // Get data from variant
            value_to_check = table.rows.at(row_index).at(col_index);    // Get value from row
            if (std::holds_alternative<std::string>(value_to_check)) {
                str_value = std::get<std::string>(value_to_check);
            }
            if (std::holds_alternative<int>(value_to_check)) {
                str_value = std::to_string(std::get<int>(value_to_check));
            }
            std::cout << std::setw(widths[col_num] + 2) << std::left << str_value;  // Print te value 
        }
        std::cout << "|" << std::endl;

    }

    // End decorative 
    for (int col_num = 0; col_num < columns_to_print; col_num++)
    {
        std::cout << "+-";
        for (int i = 0; i < widths[col_num]; i++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
}

