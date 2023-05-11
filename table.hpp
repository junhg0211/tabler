#include <iostream>
#include <fstream>

class Table
{
    int rows;
    int columns;
    std::string **data;

public:
    Table()
    {
        this->rows = 0;
        this->columns = 0;
        this->data = nullptr;
    }

    Table(int rows, int columns)
    {
        this->rows = rows;
        this->columns = columns;
        this->data = new std::string*[rows * columns];
    }

    ~Table()
    {
        for (int i = 0; i < this->rows * this->columns; i++)
            delete this->data[i];

        delete[] data;
    }

    std::string* get(int row, int column)
    {
        return this->data[row * this->columns + column];
    }

    void set(int row, int column, std::string* data)
    {
        this->data[row * this->columns + column] = data;
    }

    void print()
    {
        // getting lengths of columns
        int lengths[this->columns];

        for (int i = 0; i < this->columns; i++)
            lengths[i] = 0;

        for (int row = 0; row < this->rows; row++)
        {
            for (int column = 0; column < this->columns; column++)
            {
                int length = this->get(row, column)->length();
                if (lengths[column] < length)
                    lengths[column] = length;
            }
        }

        // printing cells
        for (int row = 0; row < this->rows; row++)
        {
            std::cout << "| ";
            for (int column = 0; column < this->columns; column++)
            {
                std::cout << std::left << std::setw(lengths[column]) << *this->get(row, column) << " | ";
            }
            std::cout << std::endl;
        }
    }

    int get_rows()
    {
        return this->rows;
    }

    int get_columns()
    {
        return this->columns;
    }
};

namespace csv
{
    Table* load_csv(std::string filename)
    {
        // opening file
        std::fstream file;
        file.open(filename, std::ios::in);

        if (file == nullptr)
        {
            std::cerr << "File is not found." << std::endl;
            file.close();
            return nullptr;
        }

        // get size of table
        int rows = -1;
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            rows++;
        }
        file.clear();
        file.seekg(0, std::ios::beg);

        int columns = 1;
        {
            std::string line;
            std::getline(file, line);

            char c;
            bool iq = false, esc = false, is = true;
            for (int i = 0; i < line.length(); i++)
            {
                c = line[i];

                if (!iq)
                {
                    if (c == '"')
                    {
                        if (!is)
                            continue;

                        iq = true;
                        continue;
                    }

                    if (c == ',')
                    {
                        columns++;
                        is = true;
                        continue;
                    }
                }
                else
                {
                    if (c == '"')
                    {
                        if (esc)
                        {
                            esc = false;
                            continue;
                        }

                        esc = true;
                        continue;
                    }

                    if (c == ',' && esc)
                    {
                        columns++;

                        esc = false;
                        iq = false;
                        is = true;
                        continue;
                    }
                }
            }
        }
        file.clear();
        file.seekg(0, std::ios::beg);

        std::cout << "rows: " << rows << std::endl;
        std::cout << "columns: " << columns << std::endl;

        Table *result = new Table(rows, columns);

        // reading file and writing table
        for (int row = 0; !file.eof(); row++)
        {
            std::string line;
            std::getline(file, line);

            // parsing csv
            int column = 0;
            std::string *data = new std::string;

            bool in_quote = false;
            bool escape = false;
            bool is_start = true;

            char c;
            for (int i = 0; i < line.length(); i++)
            {
                c = line[i];

                if (!in_quote)
                {
                    if (c == '"')
                    {
                        if (!is_start)
                        {
                            data->append(1, c);
                            continue;
                        }

                        in_quote = true;
                        continue;
                    }

                    if (c == ',')
                    {
                        result->set(row, column, data);
                        data = new std::string;
                        column++;
                        is_start = true;
                        continue;
                    }
                }
                else
                {
                    if (c == '"')
                    {
                        if (escape)
                        {
                            data->append(1, c);
                            escape = false;
                            continue;
                        }

                        escape = true;
                        continue;
                    }

                    if (c == ',' && escape)
                    {
                        result->set(row, column, data);
                        data = new std::string;
                        column++;

                        escape = false;
                        in_quote = false;
                        is_start = true;
                        continue;
                    }
                }

                data->append(1, c);
            }

            result->set(row, column, data);
        }

        // closing file
        file.close();

        return result;
    }
}
