#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> grid;
    std::vector<std::vector<bool>> counted;
    for (std::string line; std::getline(std::cin, line); ) {
        grid.push_back(line);
        counted.push_back(std::vector<bool>(line.size()));
    }
    int total{};
    for (std::size_t row{}; row != grid.size(); ++row) {
        char max_l{'0' - 1};
        char max_r{'0' - 1};
        for (std::size_t column{}; column != grid[row].size(); ++column) {
            if (max_l < grid[row][column]) {
                if (!counted[row][column]) {
                    counted[row][column] = true;
                    ++total;
                }
                max_l = grid[row][column];
            }
            std::size_t pos(grid[row].size() - 1 - column);
            if (max_r < grid[row][pos]) {
                if (!counted[row][pos]) {
                    counted[row][pos] = true;
                    ++total;
                }
                max_r = grid[row][pos];
            }
        }
    }
    for (std::size_t column{}; column != grid[0].size(); ++column) {
        char max_t{'0' - 1};
        char max_b{'0' - 1};
        for (std::size_t row{}; row != grid.size(); ++row) {
            if (max_t < grid[row][column]) {
                if (!counted[row][column]) {
                    counted[row][column] = true;
                    ++total;
                }
                max_t = grid[row][column];
            }
            std::size_t pos(grid[0].size() - 1 - row);
            if (max_b < grid[pos][column]) {
                if (!counted[pos][column]) {
                    counted[pos][column] = true;
                    ++total;
                }
                max_b = grid[pos][column];
            }
        }
    }
    std::cout << total << '\n';
}
