#include <numeric>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::cout << std::unitbuf;
    std::unordered_map<std::string, std::pair<std::string, std::string>> map;
    std::vector<std::string> cursor;
    std::string directions;
    std::cin >> directions;
    for (std::string node, dummy, left, right;
        std::getline(
            std::getline(
                std::getline(std::cin >> node, dummy, '('), left, ',') >> std::ws, right, ')') >> std::ws; )
    {
        map[node] = { left, right };
        if (node[2] == 'A')
            cursor.push_back(node);
    }
    std::uint64_t lcm{1};
    for (auto const& c: cursor)
    {
        auto step = [instruction = 0u, cursor = &c, &map, &directions]()mutable{
            cursor = &(directions[instruction] == 'L'? map[*cursor].first: map[*cursor].second);
            if (++instruction == directions.size())
                instruction = 0;
            return std::pair(instruction, cursor);
        };

        for (std::uint64_t count{};; )
        {
            auto p = step();
            ++count;
            if ((*p.second)[2] == 'Z')
            {
                lcm = std::lcm(lcm, count);
                break;
            }
        }
    }
    std::cout << lcm << "\n";
}
