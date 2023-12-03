#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <deque>
#include <cctype>

int main() {
    auto is_digit = [](unsigned char c){ return std::isdigit(c); };

    std::deque<std::string> engine;
    for (std::string line; std::getline(std::cin, line); )
    {
        engine.push_back('.' + line + '.');
    }
    engine.push_front(std::string(engine.front().size(), '.'));
    engine.push_back(std::string(engine.front().size(), '.'));

    std::size_t sum{};
    for (std::size_t r{1}; r < engine.size() - 1; ++ r)
    {
        auto begin = engine[r].begin();
        auto end = engine[r].end();
        auto it =  begin;
        while (end != (it = std::find(it, end, '*')))
        {
            std::size_t c(it - begin);
            int adjacent = 0;
            int factor{1};
            for (int j(-1); j <= 1; ++j)
            {
                for (int i(-1); i <= 1; ++i)
                {
                    adjacent += is_digit(engine[r + j][c + i]);
                    if (is_digit(engine[r + j][c + i]))
                    {
                        std::string const& row(engine[r + j]);
                        int s(i);
                        while (is_digit(row[c + s - 1]))
                            --s;
                        while (++i <= 1 && is_digit(row[c + i]))
                            ;
                        int e(i);
                        while (is_digit(row[c + e]))
                            ++e;
                        factor *= std::atoi(std::string(row.begin() + c + s, row.begin() + c + e).c_str());
                    }
                }
            }
            if (adjacent == 2)
            {
                sum += factor;
            }
            it = begin + c + 1;
        }
    }
    std::cout << sum << "\n";
}
