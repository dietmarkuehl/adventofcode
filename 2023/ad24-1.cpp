#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include "bigint.h"

struct point
{
    std::int64_t x{};
    std::int64_t y{};
    std::int64_t z{};

    friend std::ostream& operator<< (std::ostream& out, point const& p)
    {
        return out << "(" << p.x << ", " << p.y <<", " << p.z << ")";
    }
    friend std::istream& operator>> (std::istream& in, point& p)
    {
        return ((in >> p.x).ignore() >> p.y).ignore() >> p.z;
    }
};

struct hail
{
    point p{};
    point v{};
    friend std::ostream& operator<< (std::ostream& out, hail const& h)
    {
        return out << h.p << "x" << h.v;
    }
    friend std::istream& operator>> (std::istream& in, hail& h)
    {
        return (in >> h.p >> std::ws).ignore() >> h.v;
    }
};

template <typename T>
auto intersect(auto const& h0, auto const& h1, T min, T max)
{
    T x1 = h0.p.x;
    T y1 = h0.p.y;
    T vx1 = h0.v.x;
    T vy1 = h0.v.y;
    T x2 = x1 + vx1;
    T y2 = y1 + vy1;

    T x3 = h1.p.x;
    T y3 = h1.p.y;
    T vx3 = h1.v.x;
    T vy3 = h1.v.y;
    T x4 = x3 + vx3;
    T y4 = y3 + vy3;

    T nx((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    T ny((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4));
    T d((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    bool rc{(0 <= d? min * d <= nx: min * d >= nx)
         && (0 <= d? min * d <= ny: min * d >= ny)
         && (0 <= d? nx <= max * d: nx >= max * d)
         && (0 <= d? ny <= max * d: ny >= max * d)
         && (0 <= (nx / d - x1) / vx1)
         && (0 <= (ny / d - y1) / vy1)
         && (0 <= (nx / d - x3) / vx3)
         && (0 <= (ny / d - y3) / vy3)
         };
   //std::cout << h0 << " " << h1 << " " << std::boolalpha << rc << " (" << (nx / d) << ", " << (ny / d) << ")\n";
   return rc;
}

bool sign(long double x){ return x <= 0; }

int main() {
    std::vector<hail> v(std::istream_iterator<hail>{std::cin}, {});
    std::int64_t sum{};
    auto min{v.size() == 5? 7: 200000000000000};
    auto max{v.size() == 5? 27: 400000000000000};
    for (auto fit{v.begin()}; fit != v.end(); ++fit)
    {
        for (auto sit{fit + 1}; sit != v.end(); ++sit)
        {
            if (intersect<bigint>(*fit, *sit, min, max))
            {
                //std::cout << *fit << " " << *sit << " (" << px << ", " << px << ")\n";
                ++sum;
            }
        }
    }
    std::cout << sum << "\n";
}
