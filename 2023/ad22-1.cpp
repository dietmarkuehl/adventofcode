#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iomanip>

struct coord
{
    std::size_t z{};
    std::size_t y{};
    std::size_t x{};

    auto operator<=> (coord const&) const = default;
    friend std::ostream& operator<< (std::ostream& out, coord const& c)
    {
       return out << '(' << c.x << ", " << c.y << ", " << c.z << ')';
    }
    friend std::istream& operator>> (std::istream& in, coord& c)
    {
        (in >> c.x).ignore();
        (in >> c.y).ignore();
        (in >> c.z).ignore();
        return in;
    }
};

enum class orientation
{
    x,
    y,
    z
};

std::ostream& operator<< (std::ostream& out, orientation o)
{
    switch (o)
    {
    case orientation::x: return out << 'x';
    case orientation::y: return out << 'y';
    case orientation::z: return out << 'z';
    };
}

struct brick
{
    coord       c[2];
    orientation o;
    std::vector<brick const*> supporting;
    std::vector<brick const*> supported;
    brick(coord a, coord b): c{a, b}
    {
        if (c[1] < c[0]) std::swap(c[0], c[1]);
        if (c[0].x != c[1].x) o = orientation::x;
        if (c[0].y != c[1].y) o = orientation::y;
        if (c[0].z != c[1].z) o = orientation::z;
    }
    bool operator< (brick const& o) const { return c[0] < o.c[0]; }
    friend std::ostream& operator<< (std::ostream& out, brick const& b)
    {
        return out << '[' << b.c[0] << ", " << b.c[1] << ", " << b.o << ']';
    }
};

int main() {
    std::vector<brick> bricks;
    std::size_t maxx{}, maxy{}, maxz{};
    for (coord a, b; std::cin >> a >> b; )
    {
        maxx = std::max(maxx, b.x);
        maxy = std::max(maxy, b.y);
        maxz = std::max(maxz, b.z);
        bricks.emplace_back(a, b);
    }
    std::sort(bricks.begin(), bricks.end());

    std::vector<std::vector<std::size_t>> grid{maxy + 1u, std::vector<std::size_t>(maxx + 1u)};
    std::vector<std::vector<brick*>> supports{maxy + 1u, std::vector<brick*>(maxx + 1u)};
    for (auto& b: bricks)
    {
        std::size_t mz{};
        switch (b.o)
        {
        case orientation::x:
            for (std::size_t i(b.c[0].x); i <= b.c[1].x; ++i)
                mz = std::max(mz, grid[b.c[0].y][i]);
            b.c[0].z = b.c[1].z = mz;
            for (std::size_t i(b.c[0].x); i <= b.c[1].x; ++i)
            {
                if (grid[b.c[0].y][i] == mz && mz != 0)
                {
                    auto s = supports[b.c[0].y][i];
                    s->supporting.push_back(&b);
                    b.supported.push_back(s);
                }
                grid[b.c[0].y][i] = mz + 1;
                supports[b.c[0].y][i] = &b;
            }
            break;
        case orientation::y:
            for (std::size_t i(b.c[0].y); i <= b.c[1].y; ++i)
                mz = std::max(mz, grid[i][b.c[0].x]);
            b.c[0].z = b.c[1].z = mz;
            for (std::size_t i(b.c[0].y); i <= b.c[1].y; ++i)
            {
                if (grid[i][b.c[0].x] == mz && mz != 0)
                {
                    auto s = supports[i][b.c[0].x];
                    s->supporting.push_back(&b);
                    b.supported.push_back(s);
                }
                grid[i][b.c[0].x] = mz + 1;
                supports[i][b.c[0].x] = &b;
            }
            break;
        case orientation::z:
            mz = grid[b.c[0].y][b.c[0].x];
            if (mz != 0)
            {
                auto s = supports[b.c[0].y][b.c[0].x];
                s->supporting.push_back(&b);
                b.supported.push_back(s);
            }
            b.c[1].z = mz + b.c[1].z - b.c[0].z;
            b.c[0].z = mz;
            grid[b.c[0].y][b.c[0].x] = b.c[1].z + 1;
            supports[b.c[0].y][b.c[0].x] = &b;
            break;
        }
        b.supported.erase(std::unique(b.supported.begin(), b.supported.end()), b.supported.end());
    }
    std::unordered_set<brick const*> all;
    for (auto& b: bricks)
    {
        all.insert(&b);
    }
    for (auto& b: bricks)
    {
        if (b.supported.size() == 1)
        {
             auto it{all.find(b.supported[0])};
             if (it != all.end())
                 all.erase(it);
        }
    }
    std::cout << all.size() << "\n";
}
