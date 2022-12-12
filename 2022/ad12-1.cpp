#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

struct position {
    std::size_t x;
    std::size_t y;
    std::size_t height;
    std::size_t len;
};

int main() {
    std::vector<std::string> map;
    for (std::string line; std::getline(std::cin, line); ) {
        map.push_back(line);
    }
    std::size_t x{}, y{};
    std::size_t ex{}, ey{};
    for (std::size_t i(0); i != map.size(); ++i) {
        auto pos(map[i].find('S'));
        if (pos != std::string::npos) {
            x = pos;
            y = i;
        }
        pos = map[i].find('E');
        if (pos != std::string::npos) {
            ex = pos;
            ey = i;
        }
    }
    if (map[y][x] != 'S') {
        std::cout << "no start\n";
        return 1;
    }
    if (map[ey][ex] != 'E') {
        std::cout << "no end\n";
        return 1;
    }
    map[ey][ex] = 'z';

    std::queue<position> queue;
    auto push = [&](std::size_t x, std::size_t y, std::size_t height, std::size_t len) {
        if (y < map.size()
            && x < map[y].size()
            && map[y][x] != '.' && std::size_t(map[y][x]) <= height + 1u
            ) {
            if (x == ex && y == ey) {
                std::cout << ++len << "\n";
                exit(0);
            }
            queue.push(position{x, y, std::size_t(map[y][x]), len+1});
            map[y][x] = '.';
        }
    };
    queue.push(position{x, y, 'a', 0});
    while (!queue.empty()) {
        auto[x, y, height, len] = queue.front();
        queue.pop();
        push(x-1, y, height, len);
        push(x+1, y, height, len);
        push(x, y-1, height, len);
        push(x, y+1, height, len);
    }
}
