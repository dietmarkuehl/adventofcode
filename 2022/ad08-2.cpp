#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int get_scene(auto const& grid, std::size_t row, std::size_t column) {
    int scene{1};
    int count{0};
    for (std::size_t i{row}; 0u < i; ) {
        ++count;
        if (grid[row][column] <= grid[--i][column]) {
           break;
        }
    }
    scene *= count;

    count = 0;
    for (std::size_t i{row}; ++i < grid.size(); ) {
        ++count;
        if (grid[row][column] <= grid[i][column]) {
           break;
        }
    }
    scene *= count;

    count = 0;
    for (std::size_t i{column}; 0u < i; ) {
        ++count;
        if (grid[row][column] <= grid[row][--i]) {
           break;
        }
    }
    scene *= count;

    count = 0;
    for (std::size_t i{column}; ++i < grid[row].size(); ) {
        ++count;
        if (grid[row][column] <= grid[row][i]) {
           break;
        }
    }
    scene *= count;

    return scene;
}

int main() {
    std::vector<std::string> grid;
    std::vector<std::vector<int>> scene;
    for (std::string line; std::getline(std::cin, line); ) {
        grid.push_back(line);
        scene.push_back(std::vector<int>(line.size(), 1));
    }
    int max{};
    for (std::size_t row{}; row != grid.size(); ++row) {
        for (std::size_t column{}; column != grid[row].size(); ++column) {
            int scene = get_scene(grid, row, column);
            max = std::max(scene, max);
        }
    }
    std::cout << max << '\n';
}
