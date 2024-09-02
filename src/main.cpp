#include <iostream>
#include <deque>
#include <chrono>
#include <vector>
#include <numeric>
#include <locale>
#include <codecvt>
#include <conio.h>

class Table
{
private:
    std::deque<std::deque<bool>> table;
    size_t row, column;

public:
    Table(size_t row_, size_t column_) : table(row_, std::deque<bool>(column_, false)), row(row_), column(column_) {}

    bool in_table(int row_, int column_) const
    {
        return row_ >= 0 && row_ < int(this->row) && column_ >= 0 && column_ < int(this->column);
    }

    void add_up()
    {
        this->table.push_front(std::deque<bool>(column, false));
        this->row++;
    }
    void add_down()
    {
        this->table.push_back(std::deque<bool>(column, false));
        this->row++;
    }
    void add_right()
    {
        for (auto &i : this->table)
        {
            i.push_back(0);
        }
        this->column++;
    }
    void add_left()
    {
        for (auto &i : this->table)
        {
            i.push_front(0);
        }
        this->column++;
    }

    void pixel_in_life(size_t row_, size_t column_, std::deque<std::deque<bool>> &copie)
    {
        int counter = 0;
        const std::vector<std::pair<size_t, size_t>> liste{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
        for (auto &[dx, dy] : liste)
        {
            if (in_table(row_ + dx, column_ + dy))
            {
                counter += copie[row_ + dx][column_ + dy];
            }
        }

        if (counter == 3)
        {
            this->table[row_][column_] = true;
        }
        else if (counter != 2)
        {
            this->table[row_][column_] = false;
        }
    }

    void check_limit()
    {
        if (std::accumulate(this->table.front().begin(), this->table.front().end(), 0) != 0)
        {
            this->add_up();
        }
        if (std::accumulate(this->table.back().begin(), this->table.back().end(), 0) != 0)
        {
            this->add_down();
        }
        for (auto &i : this->table)
        {
            if (i.front() != 0)
            {
                this->add_left();
                break;
            }
            if (i.back() != 0)
            {
                this->add_right();
                break;
            }
        }
    }

    void update()
    {
        this->check_limit();
        std::deque<std::deque<bool>> copie = this->table;
        for (size_t i = 0; i < this->table.size(); i++)
        {
            for (size_t y = 0; y < this->table[i].size(); y++)
            {
                this->pixel_in_life(i, y, copie);
            }
        }
    }

    // bool &operator()(int row, int column) { return table[row][column]; }
    // const bool &operator()(int row, int column) const { return table[row][column]; }

    void set_value(size_t row_, size_t column_)
    {
        if (this->in_table(row_, column_))
        {
            this->table[row_][column_] = true;
        }
    }

    void print_table()
    {
        std::string output;
        for (size_t i = 0; i < this->table.size(); i++)
        {
            for (size_t y = 0; y < this->table[i].size(); y++)
            {
                output += (this->table[i][y] ? '#' : '.');
            }
            output += '\n';
        }
        output += '\n';
        std::cout << output << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Table test(30, 50);
    test.set_value(15, 20);
    test.set_value(16, 20);
    test.set_value(17, 20);
    test.set_value(16, 19);
    test.set_value(17, 21);
    for (size_t i = 0; i < 1000; i++)
    {
        system("cls");
        test.print_table();
        test.update();
    }

    // Calculer le temps écoulé
    return 0;
}

//! faire un truc pour se déplacer