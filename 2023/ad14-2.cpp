#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

auto measure(auto const& map)
{
    std::uint64_t sum{};
    for (unsigned c{}; c != map[0].size(); ++c)
    {
        std::size_t row_weight{map.size()};
        for (unsigned r{}; r != map.size(); ++r)
        {
            if (map[r][c] == 'O')
            {
                sum += row_weight;
            }
            --row_weight;
        }
    }
    return sum;
}

auto spin(auto access, unsigned width, unsigned height)
{
    for (unsigned c{}; c != width; ++c)
    {
        std::size_t free{};
        for (unsigned r{}; r != height; ++r)
        {
            switch (access(c, r))
            {
            case '.': break;
            case 'O':
                access(c, r) = '.';
                access(c, free++) = 'O';
                break;
            case '#': free = r + 1; break;
            }
        }
    }
}

auto print(auto const& map)
{
    for (auto const&r: map)
    {
        std::cout << r << "\n";
    }
    std::cout << "\n";
}

auto spin(auto& map)
{
    spin([&map](unsigned c, unsigned r)->char&{ return map[r][c]; }, map[0].size(), map.size());
    spin([&map](unsigned c, unsigned r)->char&{ return map[c][r]; }, map.size(), map[0].size());
    spin([&map](unsigned c, unsigned r)->char&{ return map[map.size() - r - 1][c]; }, map[0].size(), map.size());
    spin([&map](unsigned c, unsigned r)->char&{ return map[c][map[0].size() - r - 1]; }, map.size(), map[0].size());
}

int main() {
    std::cout << std::unitbuf;
    std::vector<std::string> map;
    for (std::string line; std::getline(std::cin, line); )
    {
         map.push_back(line);
    }

    unsigned count{1'000'000'000};
    // unsigned count{1'000};

    auto min = measure(map);
    unsigned  prev{};
    std::vector<std::string> minmap;
    for (unsigned i{}; i != count; ++i)
    {
        spin(map);
        auto m = measure(map);
        if (m < min)
        {
            std::cout << i << "->" << measure(map) << "\n";;
            min = m;
            prev = i;
            minmap = map;
        }
        if (m == min)
        {
            std::cout << "min=" << min << " i=" << i << " diff=" << (i - prev) << " "
                      << " same=" << std::boolalpha << (map == minmap) << "\n";
            if (true && prev != i && map == minmap)
            {
                unsigned remain{count - i - 1};
                unsigned offset{i + 1 - prev};
                unsigned sremain{remain % offset};
                std::cout << "sremain=" << sremain << " offset=" << (i + 1 - prev) << "\n";
                std::cout << "rem=" << remain << " srem=" << sremain << " "
                          << "sum=" << ( i + 1 + offset * (remain / offset) + sremain )
                          << "\n";
                sremain += 4;
                while (0 < sremain--)
                {
                    spin(map);
                    std::cout << "   " << measure(map) << "\n";
                }
                auto tm = measure(map);
                std::cout << "expected=" << tm << "\n";
                return 0;
            }

            prev = i;
            //print(map);
        }
        if (i % 1'000'000 == 0)
            std::cout << i << "\n";
    }
    std::cout << measure(map) << "\n";
}
