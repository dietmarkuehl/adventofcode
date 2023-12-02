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
        std::string draw;
        bool valid(true);
        for (std::istringstream lin(line); std::getline(lin, draw, ';'); )
        {
            int cubes{};
            std::string color;
            for (std::istringstream lin(draw); std::getline(lin >> cubes >> std::ws, color, ','); )
            {
                if ((color == "red" && 12 < cubes)
                    || (color == "green" && 13 < cubes)
                    || (color == "blue" && 14 < cubes))
                {
                    valid = false;
                }
            }
        }
        sum += valid? id: 0;
    }
    std::cout << sum << "\n";
}
