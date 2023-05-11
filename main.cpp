#include <iostream>
#include <fstream>
#include <vector>

#include "table.hpp"
#include "terminal.hpp"

int main(int argc, char** argv)
{
    // get filename
    if (argc <= 1)
    {
        std::cerr << "Filename is not specified." << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    // get table
    Table *table = csv::load_csv(filename);

    int row_start = 0;
    int row_end = table->get_rows();
    std::vector<int> column_lengths = std::vector<int>();

    for (int i = 0; i < table->get_columns(); i++)
    {
        int column_length = 0;
        for (int j = row_start; j < row_end; j++)
        {
            int length = table->get(j, i)->length();
            if (column_length < length)
                column_length = length;
        }
        column_lengths.push_back(column_length);
    }

    // print table
    terminal::clear();
    terminal::move(1, 1);

    for (int i = row_start; i < row_end; i++)
    {
        std::cout << "| ";
        for (int j = 0; j < table->get_columns(); j++)
        {
            std::cout << std::left << std::setw(column_lengths[j]) << *table->get(i, j) << " | ";
        }
        std::cout << std::endl;
    }

    // move cursor by cells
    int cx = 0, cy = 0;
    while (true)
    {
        // move to cell currently at
        int x = 3, y;
        for (int i = 0; i < cx; i++)
            x += column_lengths[i] + 3;
        y = cy + 1;
        terminal::move(y, x);

        // get key input
        char ch = terminal::getch();

        if (ch == 'h')
        {
            if (x > 0)
                cx--;
        }
        else if (ch == 'j')
        {
            if (y < row_end - 1)
                cy++;
        }
        else if (ch == 'k')
        {
            if (y > 0)
                cy--;
        }
        else if (ch == 'l')
        {
            if (x < table->get_columns())
                cx++;
        }
        else if (ch == 'q')
            break;
    }

    // free memory
    delete table;

    return 0;
}
