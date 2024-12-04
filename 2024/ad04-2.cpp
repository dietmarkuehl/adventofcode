#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

using grid_t = std::vector<std::string>;
bool walk(grid_t const& grid, std::size_t xp, std::size_t yp)
{
    return (
               (grid[yp - 1][xp - 1] == 'M' && grid[yp + 1][xp + 1] == 'S')
            && (grid[yp - 1][xp + 1] == 'M' && grid[yp + 1][xp - 1] == 'S')
           )
           ||
           (
               (grid[yp - 1][xp - 1] == 'S' && grid[yp + 1][xp + 1] == 'M')
            && (grid[yp - 1][xp + 1] == 'S' && grid[yp + 1][xp - 1] == 'M')
           )
           ||
           (
               (grid[yp - 1][xp - 1] == 'S' && grid[yp + 1][xp + 1] == 'M')
            && (grid[yp - 1][xp + 1] == 'M' && grid[yp + 1][xp - 1] == 'S')
           )
           ||
           (
               (grid[yp - 1][xp - 1] == 'M' && grid[yp + 1][xp + 1] == 'S')
            && (grid[yp - 1][xp + 1] == 'S' && grid[yp + 1][xp - 1] == 'M')
           )
        ;
}

int main() {
    grid_t grid{std::istream_iterator<std::string>(std::cin), {}};;
    std::size_t count{};

    bool (*walks[])(grid_t const&, std::size_t, std::size_t) = {
        walk,
    };
    for (auto y{grid.size()}; 0< --y; )
        for (auto x{grid.size()}; 0 < --x; )
            if (grid[y][x] == 'A')
                for (auto walk: walks)
                    count += walk(grid, x, y);

    std::cout << count << "\n";
}
