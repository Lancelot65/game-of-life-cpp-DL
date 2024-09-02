#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <conio.h>
#include <algorithm>
#include <cstdlib>

struct Position
{
    int x;
    int y;

    Position(int x, int y) : x(x), y(y) {}

    bool operator==(const Position &other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

class Table
{
private:
    std::vector<Position> table;
    size_t width, height;
    Position init_pos;

    bool position_exists(const Position &pos) const
    {
        return std::find(table.begin(), table.end(), pos) != table.end();
    }

    // Add a position to the table if it doesn't already exist
    void add_position(const Position &pos)
    {
        if (!position_exists(pos))
        {
            table.push_back(pos);
        }
    }

    void remove_position(const Position &pos)
    {
        auto it = std::remove(table.begin(), table.end(), pos);
        if (it != table.end())
        {
            table.erase(it, table.end());
        }
    }

public:
    Table(size_t width = 40, size_t height = 40) : width(width), height(height), init_pos(0, 0) {}

    void set_value(const Position &position)
    {
        add_position(position);
    }

    void pixel_in_life(const Position &position, bool already_exist, const std::vector<Position> &old_table)
    {
        const std::vector<std::pair<int, int>> neighbors{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
        int counter = 0;

        for (const auto &position_neighbor : neighbors)
        {
            Position temp(position.x + position_neighbor.first, position.y + position_neighbor.second);
            if (std::find(old_table.begin(), old_table.end(), temp) != old_table.end())
            {
                counter++;
            }
        }

        if (counter == 3 && !already_exist)
        {
            add_position(position);
        }
        else if ((counter < 2 || counter > 3) && already_exist)
        {
            remove_position(position);
        }
    }

    void update()
    {
        std::vector<Position> old_table = this->table;

        for (const auto &position : old_table)
        {
            pixel_in_life(position, true, old_table);
        }

        for (const auto &position : old_table)
        {
            const std::vector<std::pair<int, int>> neighbors{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
            for (const auto &position_neighbor : neighbors)
            {
                Position neighbor(position.x + position_neighbor.first, position.y + position_neighbor.second);
                if (!position_exists(neighbor))
                {
                    pixel_in_life(neighbor, false, old_table);
                }
            }
        }
    }

    void print_table() const
    {
        std::system("cls");
        std::string output;
        int start_y = this->init_pos.y;
        int end_y = this->height + this->init_pos.y;
        int start_x = this->init_pos.x;
        int end_x = this->width + this->init_pos.x;

        for (int y = start_y; y < end_y; y++)
        {
            for (int x = start_x; x < end_x; x++)
            {
                Position temp(x, y);
                if (position_exists(temp))
                {
                    output += '#';
                }
                else
                {
                    output += ' ';
                }
            }
            output += '\n';
        }
        std::cout << output;
    }

    void loop(size_t number_iteration)
    {
        for (size_t i = 0; i < number_iteration; i++)
        {
            // print_table();
            update();

            if (_kbhit())
            {
                int ch = _getch();
                if (ch == 0 || ch == 0xE0)
                {
                    ch = _getch();
                    switch (ch)
                    {
                    case 72:
                        this->init_pos.y--;
                        break;
                    case 80:
                        this->init_pos.y++;
                        break;
                    case 75:
                        this->init_pos.x--;
                        break;
                    case 77:
                        this->init_pos.x++;
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

int main()
{
    Table test;
    test.set_value(Position(15, 19));
    test.set_value(Position(15, 20));
    test.set_value(Position(15, 21));
    test.set_value(Position(14, 20));
    test.set_value(Position(16, 21));

    test.loop(1000);

    return 0;
}