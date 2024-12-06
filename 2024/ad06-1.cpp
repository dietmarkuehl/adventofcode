#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    auto print{[&map]{
        std::copy(map.begin(), map.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    }};
    int x{}, y{};
    for (; y < (int)map.size(); ++y)
        if ((x = map[y].find('^')) != std::string::npos)
            break;
    struct direction { int x; int y; };
    direction dirs[] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} };
    int current{2};
    int count{1};
    while (0 <= x && x < int(map[0].size()) && 0 <= y && y < int(map.size()))
    {
        if (map[y][x] == '.')
            ++count;
        map[y][x] = 'X';
        int nx = x + dirs[current].x;
        int ny = y + dirs[current].y;
        if (map[ny][nx] == '#')
            current = (current + 1) % 4;
        else {
            x = nx;
            y = ny;
        }
    }
    std::cout << count << "\n";
}
