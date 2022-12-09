#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <set>
#include <vector>
#include <array>

int main() {
    char direction;
    int distance;
    std::set<std::pair<int, int>> covered;

    std::array<int, 10> rx{}, ry{};
    std::fill(std::begin(rx), std::end(rx), 0);
    std::fill(std::begin(ry), std::end(ry), 0);
    int mx{}, my{};
    int nx{}, ny{};

    int print(false);
    while (std::cin >> direction >> distance) {
        if (print) std::cout << direction << " " << distance << "\n";
        int& head = (direction == 'D' || direction == 'U')? ry[0]: rx[0];
        int  off = (direction == 'U' || direction == 'R')? 1: -1;

        for (int i{}; i < distance; ++i) {
            head += off;
            nx = std::min(nx, rx[0]);
            ny = std::min(ny, ry[0]);
            mx = std::max(mx, rx[0]);
            my = std::max(my, ry[0]);
            for (int r(1); r < rx.size(); ++r) {
                //std::cout << "prev(" << rx[r-1] << ", " << ry[r-1] << ") ";
                //std::cout << "node(" << rx[r] << ", " << ry[r] << ") ";
                if (1 < rx[r] - rx[r-1]) {
                   --rx[r];
                   ry[r] += ry[r] == ry[r-1]? 0: ry[r] < ry[r-1]? 1: -1;
                }
                else if (rx[r] - rx[r-1] < -1) {
                   ++rx[r];
                   ry[r] += ry[r] == ry[r-1]? 0: ry[r] < ry[r-1]? 1: -1;
                }
                else if (1 < ry[r] - ry[r-1]) {
                   --ry[r];
                   rx[r] += rx[r] == rx[r-1]? 0: rx[r] < rx[r-1]? 1: -1;
                }
                else if (ry[r] - ry[r-1] < -1) {
                   ++ry[r];
                   rx[r] += rx[r] == rx[r-1]? 0: rx[r] < rx[r-1]? 1: -1;
                }
                //std::cout << " -> new(" << rx[r] << ", " << ry[r] << ")\n";
            }

            covered.insert(std::pair(rx[9], ry[9]));
            if (!print) continue;
            if (i != distance -1) continue;
            //std::cout << "max=(" << mx << ", " << my << ")\n";
            std::cout << "head=(" << rx[0] << ", " << ry[0] << ")\n";
            auto in_rope = [&rx,ry](int x, int y){
                for (int r(1); r < rx.size(); ++r) {
                    if (rx[r] == x && ry[r] == y) {
                        return char('0' + r);
                    }
                }
                return char{};
            };
            for (int y = my; ny <= y; --y) {
                for (int x = nx; x <= mx; ++x) {
                    if (rx[0] == x && ry[0] == y) {
                        std::cout << 'H';
                    }
                    else if (in_rope(x, y)) {
                        std::cout << in_rope(x, y);
                    }
                    else if (covered.find(std::pair(x, y)) != covered.end()) {
                        std::cout << 'X';
                    }
                    else {
                        std::cout << '.';
                    }
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }
    }
    std::cout << covered.size() << "\n";
}
