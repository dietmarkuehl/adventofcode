#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
    T z1 = h0.p.y;
    T vx1 = h0.v.x;
    T vy1 = h0.v.y;
    T vz1 = h0.v.z;
    T x2 = x1 + vx1;
    T y2 = y1 + vy1;

    T x3 = h1.p.x;
    T y3 = h1.p.y;
    T vx3 = h1.v.x;
    T vy3 = h1.v.y;
    T vz3 = h1.v.z;
    T x4 = x3 + vx3;
    T y4 = y3 + vy3;

    T nx((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    T ny((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4));
    T d((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    if (vx1 * vy3 == vx3 * vy1 && vx1 * vz3 == vx3 * vz1)
        std::cout << "simplified parallel: " << h0 << h1 << "\n";

    if (d == bigint(0))
        std::cout << "parallel: " << std::boolalpha << (d == bigint(0)) << " " << h0 << " vs. " << h1 << "\n";

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
    std::vector<int> candidates[3]{ std::vector<int>(2001), std::vector<int>(2001), std::vector<int>(2001) };
    for (int i{}; i != 3; ++i)
        std::generate(candidates[i].begin(), candidates[i].end(), [v=-1000]()mutable{ return v++; });
    auto record{[](auto& candidate, char c, auto coord, hail const& h0, hail const& h1){
        if (coord(h0.v) == coord(h1.v))
        {
            std::vector<int> t;
            auto diff{coord(h0.p) - coord(h1.p)};
            for (int v{-1000}; v <= 1000; ++v)
            {
                if (diff % (v - coord(h1.v)) == 0)
                {
                    t.push_back(v);
                }
            }
            std::vector<int> intersect;
            std::sort(candidate.begin(), candidate.end());
            std::sort(t.begin(), t.end());
            std::set_intersection(candidate.begin(), candidate.end(), t.begin(), t.end(), std::back_inserter(intersect));
            candidate.swap(intersect);
        }
    }};
    for (auto fit{v.begin()}; fit != v.end(); ++fit)
    {
        for (auto sit{fit + 1}; sit != v.end(); ++sit)
        {
            record(candidates[0], 'x', [](point const& p){ return p.x; }, *fit, *sit);
            record(candidates[1], 'y', [](point const& p){ return p.y; }, *fit, *sit);
            record(candidates[2], 'z', [](point const& p){ return p.z; }, *fit, *sit);
        }
    }
    for (auto c: candidates)
    {
        for (auto v: c) std::cout << v << " "; std::cout << "\n";
    }
    point v0{v[0].v.x - candidates[0].front(), v[0].v.y - candidates[1].front(), v[0].v.z - candidates[2].front()};
    point v1{v[1].v.x - candidates[0].front(), v[1].v.y - candidates[1].front(), v[1].v.z - candidates[2].front()};
    auto MA = v0.y / double(v0.x);
    auto MB = v1.y / double(v1.x);

    std::cout << "p0=" << v[0] << "\n";
    std::cout << "p1=" << v[1] << "\n";

    std::cout.precision(15);
    std::cout << "MA=" << MA << " MB=" << MB << "\n";
    auto CA = v[0].p.y - MA * v[0].p.x;
    auto CB = v[1].p.y - MB * v[1].p.x;
    std::cout << "CA=" << CA << " CB=" << CB << "\n";
    std::int64_t xpos = (CB - CA) / (MA - MB);
    std::int64_t ypos = MA * xpos + CA;
    auto time = (xpos - v[0].p.x) / (v[0].v.x - candidates[0].front());
    std::cout << "time=" << time << "\n";
    std::int64_t zpos = v[0].p.z + (v[0].v.z - candidates[2].front()) * time;
    std::cout << "pos=(" << xpos << ", " << ypos << ", " << zpos << ")\n";
    std::cout << (xpos + ypos + zpos) << "\n";
    std::cout << "v0=" << v0 << " v1=" << v1 << "\n";
    std::cout << sum << "\n";
}
