#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct pos {
    int x;
    int y;
    int dist{};
};
struct off {
    int x;
    int y;
};

off const dirs[]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

int main(int ac, char*[]) {
    int max{ac == 1? 6: 70};
    std::vector<std::vector<int>> map(max + 1, std::vector<int>(max + 1));
    std::size_t count{};
    for (int x, y; (std::cin >> x).ignore(1, ',') >> y && count < (ac == 1? 12: 1024); ++count) {
        map[y][x] = true;
    }
    std::cout << max << "\n";
    auto print{[max, &map]{
        for (int y{0}; y <= max; ++y) {
            for (int x{0}; x <= max; ++x) {
                std::cout << (map[y][x] == 0? '.': '#');
            }
            std::cout << "\n";
        }
    }};
    print();
    std::queue<pos> q;
    q.emplace(0u, 0u);
    while (not q.empty()) {
         auto[x, y, dist] = q.front();
         q.pop();
         if (x == max && y == max) {
             std::cout << dist << "\n";
             return 0;
         }
         for (auto[dx,dy]: dirs) {
             if (0 <= x + dx && 0 <= y + dy && x + dx <= max && y + dy <= max) {
                 auto& ref(map[y + dy][x + dx]);
                 if (0 == ref) {
                     map[y + dy][x + dx] = 1;
                     q.emplace(x + dx, y + dy, dist + 1);
                 }
             }
         }
    }
    std::cout << "not found\n";
}
