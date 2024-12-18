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
    auto solve([&map,max](int flag) {
        std::queue<pos> q;
        q.emplace(0u, 0u);
        while (not q.empty()) {
             auto[x, y, dist] = q.front();
             q.pop();
             if (x == max && y == max) {
                 return true;
             }
             for (auto[dx,dy]: dirs) {
                 if (0 <= x + dx && 0 <= y + dy && x + dx <= max && y + dy <= max) {
                     auto& ref(map[y + dy][x + dx]);
                     if (ref < flag) {
                         map[y + dy][x + dx] = flag;
                         q.emplace(x + dx, y + dy, dist + 1);
                     }
                 }
             }
        }
        return false;
    });

    int flag{};
    for (int x, y; (std::cin >> x).ignore(1, ',') >> y;) {
        map[y][x] = max * max + 1;
        if (not solve(++flag)) {
            std::cout << x << "," << y << "\n";
            break;
        }
    }
}
