#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>

struct coord
{
    int r;
    int c;
};

coord dirs[]{ {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::size_t fill(auto& map, auto sr, auto sc) {
    char match{map[sr][sc]};
    std::size_t area{};
    std::size_t perimeter{};
    std::vector<coord> all;
    std::queue<coord> q;
    auto mark{[&q, &map, &area, &all](auto r, auto c){ ++area; map[r][c] = '+'; all.emplace_back(r, c); q.emplace(r, c); }};
    mark(sr, sc);
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
                 ++perimeter;
             }
         }
    }
    for (auto[r, c]: all) {
        map[r][c] = '.';
    }
    return area * perimeter;
}

int main() {
    std::vector<std::string> map(std::istream_iterator<std::string>{std::cin}, {});
    auto print{[&map]{ for (auto&& x: map) std::cout << x << "\n"; std::cout << "---\n"; }};
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
