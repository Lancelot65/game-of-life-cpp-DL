#include <iostream>
#include <deque>
#include <chrono>
#include <vector>
#include <algorithm>
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
        this->init_pos.first++;
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
        this->init_pos.second++;
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

    void change_pos(int change, bool is_row = true)
    {
        if (is_row)
        {
            // Change row position
            int new_pos = static_cast<int>(this->init_pos.first) + change;
            if (new_pos >= 0 && new_pos + this->max_size.first < this->table.size())
            {
                this->init_pos.first = static_cast<size_t>(new_pos);
            }
        }
        else
        {
            // Change column position
            int new_pos = static_cast<int>(this->init_pos.second) + change;
            if (new_pos >= 0 && new_pos + this->max_size.second < this->table[0].size())
            {
                this->init_pos.second = static_cast<size_t>(new_pos);
            }
        }
    }

    void set_value(size_t row_, size_t column_)
    {
        if (this->in_table(row_, column_))
        {
            this->table[row_][column_] = true;
        }
    }

    void print_table()
    {
        const size_t size = (this->max_size.first) * (this->max_size.second + 1) + 1;
        char buffer[size];
        size_t index = 0;
        for (size_t i = this->init_pos.first; i < this->max_size.first + this->init_pos.first; i++)
        {
            for (size_t y = this->init_pos.second; y < this->max_size.second + this->init_pos.second; y++)
            {
                buffer[index] = (this->table[i][y] ? '#' : '.');
                index++;
            }
            buffer[index] = '\n';
            index++;
        }
        fwrite(buffer, size, 1, stderr);
        std::cout << this->column * this->row;
    }

    void loop(size_t number_iteration)
    {
        for (size_t i = 0; i < number_iteration; i++)
        {
            std::system("cls");
            this->print_table();
            this->update();
            if (_kbhit())
            {
                int ch = _getch();
                if (ch == 0 || ch == 0xE0)
                {
                    ch = _getch();
                    switch (ch)
                    {
                    case 72:
                        this->change_pos(-1);
                        break;
                    case 80:
                        this->change_pos(1);
                        break;
                    case 75:
                        this->change_pos(-1, false);
                        break;
                    case 77:
                        this->change_pos(1, false);
                        break;
                    default:
                        break;
                    }
                }
                else if (ch == 'q')
                {
                    break;
                }
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    Table test(10000, 10000, std::make_pair(29, 49));
    test.set_value(15, 20);
    test.set_value(16, 20);
    test.set_value(17, 20);
    test.set_value(16, 19);
    test.set_value(17, 21);

    auto start = std::chrono::high_resolution_clock::now();

    test.update();

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count();

    return 0;
}