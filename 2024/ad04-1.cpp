#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

using grid_t = std::vector<std::string>;
char xmas[]{'X', 'M', 'A', 'S'};

template <int dx, int dy>
bool walk(grid_t const& grid, std::size_t xp, std::size_t yp)
{
    int x(xp);
    int y(yp);
    for (auto c: xmas) {
        if (!(0 <= y && y < int(grid.size()) && 
              0 <= x && x < int(grid[y].size()) &&
              grid[y][x] == c)) {
              return false;
        }
        x += dx;
        y += dy;
    }
    return true;
}

int main() {
    bool (*walks[])(grid_t const&, std::size_t, std::size_t) = {
        &walk<-1, -1>,
        &walk<-1, 0>,
        &walk<-1, 1>,
        &walk<0, -1>,
        &walk<0, 0>,
        &walk<0, 1>,
        &walk<1, -1>,
        &walk<1, 0>,
        &walk<1, 1>,
    };
    grid_t grid{std::istream_iterator<std::string>(std::cin), {}};;
    std::size_t count{};

    for (auto y{grid.size()}; y--; )
        for (auto x{grid.size()}; x--; )
            for (auto walk: walks)
                count += walk(grid, x, y);
    std::cout << count << "\n";
}
