#include "engine.h"

void ts_engine::execute_command(Command *com, std::unordered_map<std::string, Table> &tables)
{
    // check that command is not nullptr
    if (com == nullptr) return;


    // Move the command and tables to necessary function
    if(com->command == COMMAND_CREATE_TABLE)
        execute_create_table(com, tables);

    else if (com->command == COMMAND_INSERT_INTO_VALUES)
        execute_insert_into_values(com, tables);

    else if (com -> command == COMMAND_SELECT)
        execute_select(com, tables);

    else if (com -> command == COMMAND_DELETE_FROM)
        execute_delete_from(com, tables);

    else if (com -> command == COMMAND_DROP_TABLE)
        execute_drop_table(com, tables);

    else 
        std::cout << "TinySQL Engine ERROR: command not found" << std::endl;

}

void ts_engine::execute_create_table(Command *com, std::unordered_map<std::string, Table> &tables)  
{
    // Check if command is correct 
    if (com -> command != COMMAND_CREATE_TABLE)
    {
        std::cout << "TinySQL Engine ERROR: command type is unmatch" << std::endl;
        return;
    }
    
    // Check if table  already exists
    if(tables.find(com -> table_name) != tables.end())
    {
        std::cout << "TinySQL Engine ERROR: Table with name: " << com->table_name  << " - already exists in db"<< std::endl;
        return;
    }


    // Create table

    size_t args_size = com->arguments.size();

    // Create the columns
    std::vector<Column> columns = std::vector<Column>();
    Column::Type type = Column::INTEGER;
    std::unordered_set<std::string> columns_names = std::unordered_set<std::string>();
    
    for(int i = 0; i < args_size; i = i + 2)
    {
        if (i > args_size || i == args_size) {
            std::cout << "TinySQL Engine ERROR: unexpected arguments size" << std::endl;
            return;
        }


        // Column already added to this table
        if(columns_names.find(com->arguments[i]) != columns_names.end()) 
        {
            std::cout << "TinySQL Engine ERROR: Table can get 2 columns with exact same name: " << com->arguments[i] << "." << std::endl;
            return;
        }

        columns_names.insert(com->arguments[i]);
        
        if (com->arguments[i + 1] == KEYWROD_DATATYPE_INTEGER)
            type = Column::INTEGER;
        else if (com->arguments[i + 1] == KEYWROD_DATATYPE_TEXT)
            type = Column::TEXT;
        else
        {
            std::cout << "TinySQL Engine ERROR: Unknown data type for column: " << com->arguments[i] << std::endl;
            return;
        }

        columns.push_back(Column(com->arguments[i], type));
    }

    Table table = Table(com->table_name, columns);
    tables.insert({com->table_name, Table(com->table_name, columns)});
}

void ts_engine::execute_insert_into_values(Command *com, std::unordered_map<std::string, Table> &tables)
{
    // Check if command is correct 
    if (com -> command != COMMAND_INSERT_INTO_VALUES)
    {
        std::cout << "TinySQL Engine ERROR: command type is unmatch" << std::endl;
        return;
    }

    // Check if table to insert exists already exists
    if(tables.find(com -> table_name) == tables.end())
    {
        std::cout << "TinySQL Engine ERROR: Table with name: " << com->table_name  << " - do not exists in db"<< std::endl;
        return;
    }

    // Check if arguments num is equals to columns num
    if(tables[com->table_name].columns.size() != com->arguments.size())
    {
        std::cout << "TinySQL Engine ERROR: Not enough values to insert into tabel" << std::endl;
        return;
    }


    // Create row
    std::vector<Value> row;
    size_t args_num = com->arguments.size();
    Column::Type arg_type;

    int integer_arg = 0;
    

    for (int i = 0; i < args_num; i++)
    {
        arg_type = get_literal_type(com->arguments.at(i));

        // Check if arg type is like col type 
        if (arg_type != tables[com->table_name].columns[i].type)
        {
            std::cout << "TinySQL Engine ERROR: Unmuched value for column: " << tables.at(com->table_name).columns[i].name << std::endl;
            return;
        }

        row.push_back(ts_parser::convert_to_value(com->arguments.at(i)));
    }

    tables[com->table_name].rows.push_back(row);
}

void ts_engine::execute_select(Command *com, std::unordered_map<std::string, Table> &tables)
{
    std::vector <int> col_indexes; 
    std::vector <int> row_indexes; 

    bool found_status = false;
    std::unordered_set<std::string> founded_names;


    // Check if command is correct 
    if (com -> command != COMMAND_SELECT)
    {
        std::cout << "TinySQL Engine ERROR: command type is unmatch" << std::endl;
        return;
    }
    
    // Check if table already exists
    if(tables.find(com -> table_name) == tables.end())
    {
        std::cout << "TinySQL Engine ERROR: Table with name: " << com->table_name  << " - is not found in db"<< std::endl;
        return;
    }

    // Create col_indexes vector for printer

    // Check if all rows needed by all symbol
    if (com->arguments.size() == 1 && com->arguments.at(0) == SYMBOL_ALL)
    {
        // add all rows indexes to 
        for(int i = 0; i < tables[com->table_name].columns.size(); i++)
            col_indexes.push_back(i);
    }

    // If there is a specific cols name in arguments
    else {
        for (std::string arg_name: com->arguments)
        {
            found_status = false;

            for (int col_index = 0; col_index < tables[com->table_name].columns.size(); col_index++)
            {
                if (arg_name == tables[com->table_name].columns[col_index].name)
                {
                    // Check if not arg name is already used
                    if (founded_names.find(tables[com->table_name].columns[col_index].name) != founded_names.end())
                    {
                        std::cout << "TinySQL Engine ERROR: Column with name: " << tables[com->table_name].columns[col_index].name  << " - is used twice in SELECT."<< std::endl;
                        return;
                    }

                    found_status = true;
                    
                    founded_names.insert(tables[com->table_name].columns[col_index].name);
                    col_indexes.push_back(col_index);
                }
            }

            if (found_status == false)
            {
                std::cout << "TinySQL Engine ERROR: Column with name: " << arg_name  << " - is not found in table."<< std::endl;
                return;
            }
        }
    }




    // Check special args
    if (com->whereClause.condition.column_name == "")
        for(int i = 0; i < tables[com->table_name].rows.size(); i++)
            row_indexes.push_back(i);

    else if (com->whereClause.condition.column_name != "")
    {
        row_indexes = execute_where_argument(com, tables[com->table_name]);
    }
    
    
    
    
    // Call printer 
    ts_printer::print_table_with_rows(tables[com->table_name], col_indexes, row_indexes);
}

void ts_engine::execute_delete_from(Command *com, std::unordered_map<std::string, Table> &tables)
{
    // Check if command is correct 
    if (com -> command != COMMAND_DELETE_FROM)
    {
        std::cout << "TinySQL Engine ERROR: command type is unmatch" << std::endl;
        return;
    }
    
    // Check if table exists
    if(tables.find(com -> table_name) == tables.end())
    {
        std::cout << "TinySQL Engine ERROR: Table with name: " << com->table_name  << " - is not found in db"<< std::endl;
        return;
    }

    std::vector <int> row_indexes; 


    // If Where is not used -> remove all rows
    if (com->whereClause.condition.column_name == "")
    {
        tables[com->table_name].rows.clear();
        tables[com->table_name].rows.shrink_to_fit();
    
    }
    else if (com->whereClause.condition.column_name != "")
    {
        row_indexes = execute_where_argument(com, tables[com->table_name]);


        // Remove all matching rows 
        // Walking from big to small index to remove rows -> after removing x index row, index x-5 is not changed the index
        for (int index = row_indexes.size() - 1; index > -1; index--)
        {
            tables[com->table_name].rows.erase(tables[com->table_name].rows.begin() + row_indexes[index]);
        }
    }

}

void ts_engine::execute_drop_table(Command *com, std::unordered_map<std::string, Table> &tables)
{
    // Check if command is correct 
    if (com -> command != COMMAND_DROP_TABLE)
    {
        std::cout << "TinySQL Engine ERROR: command type is unmatch" << std::endl;
        return;
    }
    
    // Check if table exists
    if(tables.find(com -> table_name) == tables.end())
    {
        std::cout << "TinySQL Engine ERROR: Table with name: " << com->table_name  << " - is not found in db"<< std::endl;
        return;
    }

    tables.erase(com->table_name);
}

std::vector<int> ts_engine::execute_where_argument(Command *com, Table &table)
{
    std::vector <int> row_indexes; 

    std::string col_name;
    int col_index;

    Value row_value;
    Value target_value;

    // Get Column name target
    col_name = com->whereClause.condition.column_name;

    // Get target value 
    target_value = com->whereClause.condition.value;


    // Find needed column index
    bool found_status = false;
    for (int i = 0; i < table.columns.size(); i++)
    {
        if(col_name == table.columns[i].name)
        {
            found_status = true;
            col_index = i;
            break;
        }
    }

    if (found_status == false)
    {
        std::cout << "TinySQL Engine ERROR: WHERE col name not found" << std::endl;
        return std::vector<int>();
    }
    

    // Find all matching rows
    for(int i = 0; i < table.rows.size(); i++)
    {
        // Get data from variant
        row_value = table.rows.at(i).at(col_index);    // Get value from row


        if (compare_values(row_value, target_value, com->whereClause.condition.op))
        {
            row_indexes.push_back(i);
        }
    }

    return row_indexes;
}

Column::Type ts_engine::get_literal_type(std::string literal)    
{
    if (ts_tokenizer::is_number(literal))       // INTEGER
    {
        return Column::Type::INTEGER;
    }

    else                                        // TEXT 
    {
        return Column::Type::TEXT;
    }


}

bool ts_engine::compare_values(const Value &a, const Value &b, Operator op)
{
    if (a.index() != b.index())
    {
        return false;
    }


    // Int compares 
    if (std::holds_alternative<int>(a)) {
        int av = std::get<int>(a);
        int bv = std::get<int>(b);

        switch (op) {
            case Operator::EQ:  return av == bv;
            case Operator::NEQ: return av != bv;
            case Operator::GT:  return av > bv;
            case Operator::LT:  return av < bv;
            case Operator::GTE: return av >= bv;
            case Operator::LTE: return av <= bv;
        }
    }

    if (std::holds_alternative<std::string>(a)) {
        const auto& av = std::get<std::string>(a);
        const auto& bv = std::get<std::string>(b);

        switch (op) {
            case Operator::EQ:  return av == bv;
            case Operator::NEQ: return av != bv;
            case Operator::GT:  return av > bv;
            case Operator::LT:  return av < bv;
            case Operator::GTE: return av >= bv;
            case Operator::LTE: return av <= bv;
        }
    }




    return false;
}
