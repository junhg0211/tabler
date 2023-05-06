#include <iostream>
#include <fstream>

#include "table.hpp"

int main(int argc, char** argv)
{
    // getting filename
    if (argc <= 1)
    {
        std::cerr << "Filename is not specified." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::cout << filename << std::endl;

    Table *table = csv::load_csv(filename);
    table->print();

    delete table;

    return 0;
}
