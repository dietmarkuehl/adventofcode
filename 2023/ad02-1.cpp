#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

int main() {
    int id{};
    int sum{};
    std::unordered_map<std::string, int> limits{{"red", 12}, {"green", 13}, {"blue", 14}};
    for (std::string dummy, line; std::getline(std::cin >> dummy >> id >> dummy, line); )
    {
        std::replace(line.begin(), line.end(), ';', ',');
        int cubes{};
        std::string color;
        for (std::istringstream lin(line); std::getline(lin >> cubes >> std::ws, color, ','); )
        {
            if (limits[color] < cubes)
            {
                id = 0;
            }
        }
        sum += id;
    }
    std::cout << sum << "\n";
}
