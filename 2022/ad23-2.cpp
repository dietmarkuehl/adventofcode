#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

std::vector<std::string> read(std::istream& in) {
    return std::vector<std::string>(std::istream_iterator<std::string>(in), std::istream_iterator<std::string>{});
   
}

std::vector<std::string> expand(auto const& m, std::size_t n) {
    std::vector<std::string> rc;
    std::fill_n(std::back_inserter(rc), n, std::string(2u * n + m[0].size(), '.'));
    for (std::string const& s: m) {
        rc.emplace_back(std::string(n, '.') + s + std::string(n, '.'));
    }

    std::fill_n(std::back_inserter(rc), n, std::string(2u * n + m[0].size(), '.'));
    return rc;

}

struct position {
    std::ptrdiff_t x{};
    std::ptrdiff_t y{};
                 
    bool operator== (position const&) const = default;
    position operator+ (position const& other) const { 
        return { this->x + other.x, this->y + other.y };
    }
    friend std::ostream& operator<< (std::ostream& out, position const& p) {
        return out << "(" << p.x << ", " << p.y << ")";
    }
};

using direction_t  = std::array<position, 3>;
using directions_t = std::array<direction_t, 4>;
directions_t directions{
    direction_t{ position{ -1, -1 }, position{  0, -1 }, position{  1, -1 } },
    direction_t{ position{ -1,  1 }, position{  0,  1 }, position{  1,  1 } },
    direction_t{ position{ -1, -1 }, position{ -1,  0 }, position{ -1,  1 } },
    direction_t{ position{  1, -1 }, position{  1,  0 }, position{  1,  1 } }
};

std::array<position, 8> surround{
    position{ -1, -1 }, position{  0, -1 }, position{  1, -1 },
    position{ -1,  0 },                     position{  1,  0 },
    position{ -1,  1 }, position{  0,  1 }, position{  1,  1 },
};

struct elf {
    position current;
    position proposed;
};

position max(position const& a, position const& b) {
    return position{ std::max(a.x, b.x), std::max(a.y, b.y) };
}
position min(position const& a, position const& b) {
    return position{ std::min(a.x, b.x), std::min(a.y, b.y) };
}

std::vector<elf> get_elves(auto const& m) {
    std::vector<elf> elves;
    for (std::size_t y{}; y != m.size(); ++y) {
        for (std::size_t x{}; x != m[y].size(); ++x) {
            if (m[y][x] == '#') {
                elves.push_back({ { std::ptrdiff_t(x), std::ptrdiff_t(y) }, {} });
            }
        }
    }
    return elves;
}

int main() {
    auto map{read(std::cin)};
    auto print = [&map]{ std::copy(map.begin(), map.end(), std::ostream_iterator<std::string>(std::cout, "\n")); };
    (void)print;
    map = expand(map, map.size());
    
    auto elves = get_elves(map);
    auto at = [&map](position p)->char&{ return map[p.y][p.x]; };

    for (std::size_t i{}; ; ++i) {
        for (elf& e: elves) {
            e.proposed = e.current;
            if (std::any_of(surround.begin(), surround.end(),
                            [&e, &at](position const& p){ return at(e.current + p) == '#'; })) {
                for (std::size_t d{}; d != 4; ++d)  {
                    if (std::none_of(directions[(i + d) % 4].begin(), directions[(i + d) %4].end(),
                                     [&e, &at](position const& p){ return at(e.current + p) == '#'; })) {
                        e.proposed = e.current + directions[(i + d) % 4][1];
                        at(e.proposed) = at(e.proposed) == 'P'? 'N': 'P';
                        break;
                    }
                }
            }
        }
        if (std::none_of(elves.begin(), elves.end(),
                         [](elf const& e){ return e.current != e.proposed; })) {
            std::cout << (i + 1) << "\n";
            break;
        }
        for (elf& e: elves) {
           if (at(e.proposed) == 'P') {
              at(e.proposed) = '#';
              at(e.current) = '.';
              e.current = e.proposed;
           }
           else if (at(e.proposed) == 'N') {
               at(e.proposed) = '.';
           }
        }
    }
}
