#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

int main() {
    int id{};
    int sum{};
    for (std::string dummy, line; std::getline(std::cin >> dummy >> id >> dummy, line); )
    {
        std::unordered_map<std::string, int> max{{"red", 0}, {"green", 0}, {"blue", 0}};
        std::replace(line.begin(), line.end(), ';', ',');
        int cubes{};
        std::string color;
        for (std::istringstream lin(line); std::getline(lin >> cubes >> std::ws, color, ','); )
        {
            max[color] = std::max(max[color], cubes);
        }
    }
    std::cout << sum << "\n";
}
