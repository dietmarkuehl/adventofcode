#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>

int main() {
    int id{};
    int sum{};
    for (std::string dummy, line; std::getline(std::cin >> dummy >> id >> dummy, line); )
    {
        std::unordered_map<std::string, int> maxs{{"red", 0}, {"blue", 0}, {"green", 0}};
        std::string draw;
        for (std::istringstream lin(line); std::getline(lin, draw, ';'); )
        {
            int cubes{};
            std::string color;
            for (std::istringstream lin(draw); std::getline(lin >> cubes >> std::ws, color, ','); )
            {
                maxs[color] = std::max(cubes, maxs[color]);
            }
        }
        int factor=1;
        for (auto[color,count]: maxs)
        {
            factor *= count;
        }
        sum += factor;
    }
    std::cout << sum << "\n";
}
