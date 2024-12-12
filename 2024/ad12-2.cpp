#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <string>
#include <set>
#include <vector>
#include <cinttypes>

struct coord
{
    int r;
    int c;
    bool operator< (coord const& o) const { return r == o.r? c < o.c: r < o.r; }
};

coord dirs[]{ {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::size_t fill(auto& map, auto sr, auto sc) {
    char match{map[sr][sc]};
    std::size_t area{};
    std::vector<coord> all;
    std::queue<coord> q;
    auto mark{[&q, &map, &area, &all](auto r, auto c){ ++area; map[r][c] = '+'; all.emplace_back(r, c); q.emplace(r, c); }};
    mark(sr, sc);
    std::set<coord> hfence, vfence;
    while (not q.empty()) {
         auto[r, c] = q.front();
         q.pop();
         for (auto[dr, dc]: dirs) {
             auto pr(r + dr);
             auto pc(c + dc);
             if (map[pr][pc] == match) {
                 mark(pr, pc);
             }
             else if (map[pr][pc] != '+') {
                 (dr == 0? hfence: vfence).emplace(pr, pc);
             }
         }
    }

    std::size_t top_sides{};
    for (int r{0}; r < int(map.size()) - 1; ++r)
        for (int c{0}; c < int(map[0].size()); ) {
            auto t = c++;
            if (map[r][t] != '+' && map[r+1][t] == '+') {
                ++top_sides;
                while (map[r][c] != '+' && map[r+1][c++] == '+') {
                }
            }
        }
    std::size_t bottom_sides{};
    for (int r(map.size()); 0 < r; --r)
        for (int c{0}; c < int(map[0].size()); ) {
            auto t = c++;
            if (map[r][t] != '+' && map[r-1][t] == '+') {
                ++bottom_sides;
                while (map[r][c] != '+' && map[r-1][c++] == '+') {
                }
            }
        }
    std::size_t left_sides{};
    for (int c{0}; c < int(map[0].size()) - 1; ++c)
        for (int r{0}; r < int(map.size()); ) {
            auto t = r++;
            if (map[t][c] != '+' && map[t][c+1] == '+') {
                ++left_sides;
                while (map[r][c] != '+' && map[r++][c+1] == '+') {
                }
            }
        }
    std::size_t right_sides{};
    for (int c(map[0].size()); 0 < c; --c)
        for (int r{0}; r < int(map.size()); ) {
            auto t = r++;
            if (map[t][c] != '+' && map[t][c-1] == '+') {
                ++right_sides;
                while (map[r][c] != '+' && map[r++][c-1] == '+') {
                }
            }
        }
    std::size_t sides = top_sides + bottom_sides + left_sides + right_sides;
    for (auto[r, c]: all) {
        map[r][c] = '.';
    }
    return area * sides;
}

int main() {
    std::vector<std::string> map(std::istream_iterator<std::string>{std::cin}, {});
    //auto print{[&map]{ int r{}; for (auto&& x: map) std::cout << std::setw(2) << r++ << " "<< x << "\n"; std::cout << "---\n"; }};
    for (auto& x: map) {
        x = '.' + x + '.';
    }
    map.insert(map.begin(), std::string(map[0].size(), '.'));
    map.insert(map.end(), std::string(map[0].size(), '.'));
    //print();
    std::uint64_t price{};
    for (auto r{map.size()}; r--; ) {
        for (auto it{map[r].begin()}; map[r].end() != (it = std::find_if(it, map[r].end(), [](char c){ return c != '.'; })); ) {
            std::size_t c(it - map[r].begin()); 
            auto cost = fill(map, r, c);
            price += cost;
            //print();
        }
    }
    std::cout << price << "\n";
}
