#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <set>
#include <vector>

int main() {
    char direction;
    int distance;
    std::set<std::pair<int, int>> covered;

    int hx{}, hy{};
    int tx{}, ty{};
    int mx{}, my{};

    while (std::cin >> direction >> distance) {
        //std::cout << direction << " " << distance << "\n";
        int& head = (direction == 'D' || direction == 'U')? hy: hx;
        int  off = (direction == 'U' || direction == 'R')? 1: -1;

        for (int i{}; i < distance; ++i) {
            int px(hx), py(hy);
            head += off;
            mx = std::max(mx, hx);
            my = std::max(my, hy);
            if (1 < std::abs(tx - hx) || 1 < std::abs(ty - hy)) {
                tx = px;
                ty = py;
            }

            covered.insert(std::pair(tx, ty));
            continue;
            std::cout << "prev=(" << px << ", " << py << ") head=(" << hx << ", " << hy << ") tail=(" << tx << " ," << ty << ") ->" << covered.size() << "\n";
            for (int y = 0; y <= my; ++y) {
                for (int x = 0; x <= mx; ++x) {
                    if (hx == x && hy == y) {
                        std::cout << 'H';
                    }
                    else if (tx == x && ty == y) {
                        std::cout << 'T';
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
        }
    }
    std::cout << covered.size() << "\n";
}
