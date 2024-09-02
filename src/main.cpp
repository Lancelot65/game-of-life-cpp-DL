#include <iostream>
#include <deque>
#include <chrono>
#include <vector>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <conio.h>

class Table
{
private:
    std::deque<std::deque<bool>> table;
    size_t row, column;
    std::pair<size_t, size_t> max_size;
    std::pair<size_t, size_t> init_pos = std::make_pair(0, 0);

public:
    Table(size_t row_, size_t column_, std::pair<size_t, size_t> max_size_) : table(row_, std::deque<bool>(column_, false)), row(row_), column(column_), max_size(max_size_) {}

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
        for (std::deque<bool> &i : this->table)
        {
            i.push_back(0);
        }
        this->column++;
    }
    void add_left()
    {
        for (std::deque<bool> &i : this->table)
        {
            i.push_front(0);
        }
        this->column++;
    }

    void pixel_in_life(size_t row_, size_t column_, std::deque<std::deque<bool>> &copy)
    {
        int counter = 0;
        const std::vector<std::pair<int, int>> list{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
        for (auto &[dx, dy] : list)
        {
            if (in_table(row_ + dx, column_ + dy))
            {
                counter += copy[row_ + dx][column_ + dy];
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
        if (any_of(this->table.front().begin(), this->table.front().end(), [](bool v)
                   { return v; }))
        {
            this->add_up();
        }
        if (any_of(this->table.back().begin(), this->table.back().end(), [](bool v)
                   { return v; }))
        {
            this->add_down();
        }
        for (std::deque<bool> &i : this->table)
        {
            if (i.front())
            {
                this->add_left();
                break;
            }
            if (i.back())
            {
                this->add_right();
                break;
            }
        }
    }

    void update()
    {
        this->check_limit();
        std::deque<std::deque<bool>> copy = this->table;
        for (size_t i = 0; i < this->table.size(); i++)
        {
            for (size_t y = 0; y < this->table[i].size(); y++)
            {
                this->pixel_in_life(i, y, copy);
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

        const size_t size = (this->row) * (this->column + 1) + 1;
        char buffer[size];
        size_t index = 0;
        for (size_t i = 0; i < this->table.size(); i++)
        {
            for (size_t y = 0; y < this->table[i].size(); y++)
            {
                buffer[index] = (this->table[i][y] ? '#' : '.');
                index++;
            }
            buffer[index] = '\n';
            index++;
        }
        buffer[index] += '\n';
        fwrite(buffer, size, 1, stderr);
    }
};

int main(int argc, char const *argv[])
{
    Table test(30, 30, std::make_pair(20, 20));
    test.set_value(15, 20);
    test.set_value(16, 20);
    test.set_value(17, 20);
    test.set_value(16, 19);
    test.set_value(17, 21);
    for (size_t i = 0; i < 1000; i++)
    {
        std::system("cls");
        test.print_table();
        test.update();
    }

    return 0;
}

//! faire un truc pour se déplacer