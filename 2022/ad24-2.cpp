#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

struct position {
    int x{};
    int y{};
    bool operator< (position const& other) const { return this->x == other.x? (this->y < other.y): (this->x < other.y); }
    bool operator== (position const&) const = default;
    position& operator+= (position const& other) { this->x += other.x; this->y += other.y; return *this; }
    friend std::ostream& operator<< (std::ostream& out, position const& p) {
        return out << "(" << p.x << ", " << p.y << ")";
    }
};

struct blizzard {
    position                       pos;
    char                           d{};
    std::function<void(position&)> move;
};

auto get_blizzards(auto const& map) {
    std::vector<blizzard> rc;
    for (int y{}; y != int(map.size()); ++y) {
        for (int x{}; x != int(map[y].size()); ++x) {
            switch (map[y][x]) {
            case '<':
                rc.push_back(blizzard{{x, y}, '<',
                             [width=map[y].size()](position& p){ if ((p.x -= 1) == 0){ p.x = width - 2; }}});
                break;
            case '>':
                rc.push_back(blizzard{{x, y}, '>',
                             [width=map[y].size()](position& p){ if ((p.x += 1) == int(width) - 1){ p.x = 1; }}});
                break;
            case '^':
                rc.push_back(blizzard{{x, y}, '^',
                             [height=map.size()](position& p){ if ((p.y -= 1) == 1){ p.y = height - 3; }}});
                break;
            case 'v':
                rc.push_back(blizzard{{x, y}, 'v',
                             [height=map.size()](position& p){ if ((p.y += 1) == int(height) -  2){ p.y = 2; }}});
                break;
            }
        }
    }
    return rc;
}

std::array<position, 5> moves{
   position{  0,  0 },
   {  1,  0 },
   { -1,  0 },
   {  0,  1 },
   {  0, -1 }
};

int traverse(auto const& empty, auto& blizzards, auto start, auto end) {
    auto map{empty};
    auto at = [&map](position const& p)->char& { return map[p.y][p.x]; };
    std::vector<position> expeditions{start};
    std::size_t total{};
    while (true) {
        ++total;
        std::for_each(blizzards.begin(), blizzards.end(), [at](blizzard& b){ b.move(b.pos); at(b.pos) = b.d; });
        std::vector<position> tmp{};
        for (auto const& ex: expeditions) {
            for (auto p: moves) {
                if (at(p += ex) == '.') {
                    tmp.push_back(p);
                    at(p) = 'e';
                    if (p == end) {
                       return total;
                    }
                };
            }
        }
        map = empty;
        expeditions = tmp;
    }
}

int main() {
    std::vector<std::string> map(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>{});
    map.insert(map.begin(), std::string(map[0].size(), '#'));
    map.insert(map.end(), std::string(map[0].size(), '#'));
    auto print = [&map]{ std::copy(map.begin(), map.end(), std::ostream_iterator<std::string>(std::cout, "\n")); };
    (void)print;
    auto blizzards = get_blizzards(map);
    auto at = [&map](position const& p)->char& { return map[p.y][p.x]; };
    for (auto const& b: blizzards) {
        at(b.pos) = '.';
    }
    position start{1, 1};
    position end{int(map[0].size()) - 2, int(map.size()) -2};
    int total = traverse(map, blizzards, start, end);
    total += traverse(map, blizzards, end, start);
    total += traverse(map, blizzards, start, end);

    std::cout << total << "\n";
}
