#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

enum class dir {
    north,
    east,
    south,
    west
};
std::ostream& operator<< (std::ostream& out, dir d) {
    switch (d) {
    case dir::north: return out << "north";
    case dir::east: return out << "east";
    case dir::south: return out << "south";
    case dir::west: return out << "west";
    }
}

auto off(dir d) {
    struct rc {
        int x;
        int y;
    };
    switch (d) {
    case dir::north: return rc{0, -1};
    case dir::east: return rc{1, 0};
    case dir::south: return rc{0, 1};
    case dir::west: return rc{-1, 0};
    }
}


struct pos {
    std::size_t x;
    std::size_t y;
    dir d;
    std::size_t cost{};
    bool operator< (pos const& o) const { return cost > o.cost; }
};

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    auto print{[&map]{ for (auto l: map) std::cout << l << "\n"; std::cout << "\n"; }};
    (void)print;
    //print();
    std::size_t y{};
    std::size_t x{};
    while (y != map.size()) {
        x = map[y].find('S');
        if (x != map[y].npos)
            break;
        ++y;
    }
    std::cout << "(" << x << ", " << y << ")\n";
    std::priority_queue<pos> q;
    q.emplace(x, y, dir::east, 0u);
    while (!q.empty()) {
        auto[x, y, d, cost] = q.top();
        q.pop();
        //std::cout << "(" << x << ", " << y << ") " << d << " => " << cost << "\n";
        if (map[y][x] == 'E') {
            std::cout << cost << "\n";
            std::cout << q.size() << "\n";
            while (!q.empty()) {
                auto[ex, ey, ed, ecost] = q.top();
                q.pop();
                if (cost < ecost) {
                    break;
                }
                if (ex == x && ey == y)
                    std::cout << "ecost=" << ecost << "\n";
            }
            break;
        }
        map[y][x] = (map[y][x] == '.'? 0: map[y][x]) | (1 << int(d));
        //print();
        for (auto nd: {dir::east, dir::west, dir::north, dir::south}) {
            auto o = off(nd);
            std::size_t nx = x + o.x;
            std::size_t ny = y + o.y;
            if (map[ny][nx] == '.' || map[ny][nx] == 'E' || (map[ny][nx] != '#' && (map[ny][nx] & (1 << int(nd))) == 0)) {
                q.emplace(nx, ny, nd, cost + 1 + (nd == d? 0u: 1000));
            }
        }
    }
}
