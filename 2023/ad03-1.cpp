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
    auto is_symbol = [&engine, &is_digit](auto r, auto c){
        return (engine[r][c] != '.' && !is_digit(engine[r][c]))
            || engine[r - 1][c] != '.'
            || engine[r + 1][c] != '.'
            ;
    };
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
        while (end != (it = std::find_if(it, end, is_digit)))
        {
            std::size_t c(it - begin);
            bool use(is_symbol(r, c - 1));
            for (; is_digit(engine[r][c]); ++c)
            {
                use = use || is_symbol(r, c);
            }
            use = use || is_symbol(r, c);
            if (use)
            {
                sum += std::atoi(std::string(it, begin + c).c_str());
            }
            it = begin + c;
        }
    }
    std::cout << sum << "\n";
}
