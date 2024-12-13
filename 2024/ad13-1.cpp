#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <cinttypes>

template <char C>
std::istream& skip(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), C);
}
auto pl = skip<'+'>;
auto eq = skip<'='>;

// a * ax + b * bx = x
// a * ay + b * by = y

// a = (x - b * bx) / ax
// b = (y * ax - x * ay) / (ax * by - bx * ay)

// a = (y - b * by) / ay 
// b = (x * ay - y * ax) / (ax * by - ay * bx)

int main() {
    std::uint64_t ax, ay, bx, by, x, y;
    std::uint64_t sum{};
    while (std::cin >> pl >> ax >> pl >> ay >> pl >> bx >> pl >> by >> eq >> x >> eq >> y)
    {
        auto b1 = (y * ax - x * ay) / (ax * by - bx * ay);
        auto a1 = (x - b1 * bx) / ax;
        auto b2 = (x * ay - y * ax) / (ax * by - ay * bx);
        auto a2 = (y - b2 * by) / ay;
        auto a3 = (y * bx - x * by) / (bx * ay - ax * by);
        auto b3 = (x - a3 * ax) / bx;
        auto b4 = (x * by - y * bx) / (bx * ay - by * ax);
        auto a4 = (y - b4 * by) / ay;
        if (a1 * ax + b1 * bx == x && a1 * ay + b1 * by == y) {
            sum += 3 * a1 + b1;
        }
        else if (a2 * ax + b2 * bx == x && a2 * ay + b2 * by == y) {
            sum += 3 * a2 + b2;
        }
        else if (a3 * ax + b3 * bx == x && a3 * ay + b3 * by == y) {
            sum += 3 * a3 + b3;
        }
        else if (a4 * ax + b4 * bx == x && a4 * ay + b4 * by == y) {
            sum += 3 * a4 + b4;
        }
    }
    std::cout << sum << "\n";
}
