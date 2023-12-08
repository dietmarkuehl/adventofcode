#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<std::string, std::pair<std::string, std::string>> map;
    std::string directions;
    std::cin >> directions;
    for (std::string node, dummy, left, right;
        std::getline(
            std::getline(
                std::getline(std::cin >> node, dummy, '('), left, ',') >> std::ws, right, ')') >> std::ws; )
    {
        // std::cout << node << " -> " << left << "-" << right << "\n";
        map[node] = { left, right };
    }
    std::string cursor("AAA");
    unsigned    instruction{};
    unsigned    count{};
    while (cursor != "ZZZ")
    {
        ++count;
        cursor = directions[instruction] == 'L'? map[cursor].first: map[cursor].second;
        if (++instruction == directions.size())
            instruction = 0;
    }
    std::cout << count << "\n";
}
