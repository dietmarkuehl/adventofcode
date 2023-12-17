#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

enum direction
{
    left,
    right,
    up,
    down
};
std::ostream& operator<< (std::ostream& out, direction dir)
{
    switch (dir)
    {
    case direction::left: return out << "left";
    case direction::right: return out << "right";
    case direction::up: return out << "up";
    case direction::down: return out << "down";
    }
}

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    std::vector<std::string> flag[]{
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
        { map.size(), std::string(map[0].size(), char{}) },
    };
    using value_t = std::tuple<unsigned, unsigned, unsigned, unsigned, direction>;
    auto cmp{[&map](auto const& t0, auto const& t1){
        auto[row0, column0, cost0, straight0, dir0] = t0;
        auto[row1, column1, cost1, straight1, dir1] = t1;
        return cost0 + map[row0][column0] > cost1 + map[row1][column1];
        }};
    std::priority_queue<value_t, std::vector<value_t>, decltype(cmp)> queue{cmp};
    queue.emplace(0, 1, 0, 1, direction::right);
    queue.emplace(1, 0, 0, 1, direction::down);
    while (not queue.empty())
    {
        auto[row, column, cost, straight, dir] = queue.top();
        queue.pop();
        cost += map[row][column] - '0';

        if (flag[straight - 1][row][column] & (1u << int(dir)))
        {
            continue;
        }
        flag[straight - 1][row][column] |= 1u << int(dir);

        if (row == map.size() - 1 && column == map[0].size() - 1 && 3 < straight)
        {
            std::cout << cost << "\n";
            break;
        }

        if (0u < row && dir != direction::down && (dir != direction::up || straight < 10u)
            && (3 < straight || dir == direction::up))
            queue.emplace(row - 1, column, cost,
                          dir == direction::up? straight + 1u: 1u, direction::up);

        if (0u < column && dir != direction::right && (dir != direction::left || straight < 10u)
            && (3 < straight || dir == direction::left))
            queue.emplace(row, column - 1, cost,
                          dir == direction::left? straight + 1u: 1u, direction::left);

        if (row < map.size() - 1 && dir != direction::up && (dir != direction::down || straight < 10u)
            && (3 < straight || dir == direction::down))
            queue.emplace(row + 1, column, cost,
                          dir == direction::down? straight + 1u: 1u, direction::down);

        if (column < map[0].size() - 1
            && dir != direction::left && (dir != direction::right || straight < 10u)
            && (3 < straight || dir == direction::right))
            queue.emplace(row, column + 1, cost,
                          dir == direction::right? straight + 1u: 1u, direction::right);
    }
}
