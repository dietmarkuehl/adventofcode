#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <cinttypes>

#if 0
#include <iostream>
int main() {
    for (int n = 0; n < 103; ++n) {
        if ((59 + 101 * n) % 103 == 0)
            std::cout << "n=" << n << " -> " << (89 + 101 * n) << "\n";
    }
}
#endif

template <char C>
std::istream& skip(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), C);
}
auto eq = skip<'='>;
auto cm = skip<','>;

struct vpos {
    std::int64_t px, py;
    std::int64_t vx, vy;
};

int main() {
    char fill(' ');
    int width{}, height{};
    std::cin >> width >> height;
    int midx{width / 2}, midy{height / 2};
    std::vector<std::string> map(height, std::string(width, fill));
    auto print = [&map]{ for (auto l: map) std::cout << l << "\n"; std::cout << "\n"; };

    std::int64_t count = 100;
    std::int64_t px, py, vx, vy;
    std::vector<vpos> robots;
    while (std::cin >> eq >> px >> cm >> py >> eq >> vx >> cm >> vy) {
        robots.push_back(vpos{px, py, vx, vy});
    }
    for (int count{}; count < 10000; ++count) {
        std::fill(map.begin(), map.end(), std::string(width, fill));
        for (auto[px, py, vx, vy]: robots) {
            auto nx = (px + count * (vx + width)) % width;
            auto ny = (py + count * (vy + height)) % height;
            auto& c = map[ny][nx];
            c = c == fill? '1': (c+1);
        }
        std::cout << count << "\n";
        print();
    }
}
