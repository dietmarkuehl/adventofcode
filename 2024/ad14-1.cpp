#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <cinttypes>

template <char C>
std::istream& skip(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), C);
}
auto eq = skip<'='>;
auto cm = skip<','>;

int main() {
    int width{}, height{};
    std::cin >> width >> height;
    int midx{width / 2}, midy{height / 2};
    std::cout << "width=" << width << " height=" << height << " midx=" << midx << " midy=" << midy << "\n";
    std::vector<std::string> map(height, std::string(width, '.'));
    auto print = [&map]{ for (auto l: map) std::cout << l << "\n"; };

    std::int64_t count = 100;
    std::int64_t px, py, vx, vy;
    std::array<int, 4> factors{};
    while (std::cin >> eq >> px >> cm >> py >> eq >> vx >> cm >> vy) {
        auto nx = (px + count * (vx + width)) % width;
        auto ny = (py + count * (vy + height)) % height;
        std::cout << "nx=" << nx << " ny=" << ny << "\n";
        auto& c = map[ny][nx];
        c = c == '.'? '1': (c+1);
        if (nx != midx && ny != midy)
            ++factors[2 * (midx < nx) + (midy < ny)];
    }

    print();
    std::cout << std::accumulate(factors.begin(), factors.end(), 1, std::multiplies<>{}) << "\n";
}
