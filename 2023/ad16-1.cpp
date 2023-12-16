#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

enum class direction
{
    right,
    left,
    down,
    up
};
std::ostream& operator<< (std::ostream& out, direction dir)
{
    switch (dir)
    {
    case direction::right: return out << "right";
    case direction::left: return out << "left";
    case direction::down: return out << "down";
    case direction::up: return out << "up";
    }
}

unsigned bit[] = { 1u, 2u, 4u, 8u };
int xdelta[] = { 1, -1, 0,  0 };
int ydelta[] = { 0,  0, 1, -1 };
direction backslash[] = {
    direction::down,
    direction::up,
    direction::right,
    direction::left,
};
direction slash[] = {
    direction::up,
    direction::down,
    direction::left,
    direction::right,
};

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    std::vector<std::string> energized(map.size(), std::string(map[0].size(), '\0'));
    std::vector<std::tuple<int, int, direction>> beams;
    beams.emplace_back(0, 0, direction::right);
    while (not beams.empty())
    {
        auto[column, row, dir] = beams.back();

        int dindex{int(dir)};
        beams.pop_back();
        if (energized[row][column] & bit[dindex]
            || column < 0 || map[0].size() <= unsigned(column)
            || row    < 0 || map.size()    <= unsigned(row)
            )
        {
            continue;
        }
        energized[row][column] |= bit[dindex];

        switch (map[row][column])
        {
        default:
            std::cout << "unexpected: '" << map[row][column] << "'"
                      << "(" << int(map[row][column]) << ")\n";
            break;
        case '.': break;
        case '/':  dir = slash[dindex]; break;
        case '\\': dir = backslash[dindex]; break;
        case '-':
            if (dir == direction::down || dir == direction::up)
            {
                beams.emplace_back(column + 1, row, direction::right);
                dir = direction::left;
            }
            break;
        case '|':
            if (dir == direction::right || dir == direction::left)
            {
                beams.emplace_back(column, row + 1, direction::down);
                dir = direction::up;
            }
            break;
        }
        dindex = int(dir);
        beams.emplace_back(column + xdelta[dindex], row + ydelta[dindex], dir);
    }

    unsigned sum{};
    for (auto e: energized)
    {
        sum += std::count_if(e.begin(), e.end(), [](auto c){ return c != '\0'; });
    }
    std::cout << sum << "\n";
}
