#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

constexpr auto is_digit = [](unsigned char c){ return std::isdigit(c); };
std::string const digits[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int main()
{
    int sum{};
    auto getdigit = [](auto begin, auto end, auto make_range){
        auto it(std::find_if(begin, end, is_digit));
        char rc{it == end? '0': *it};
        for (auto const& digit: digits) {
            auto wit = std::search(begin, end, make_range(digit).first, make_range(digit).second);
            if (wit < it) {
                it = wit;
                rc = '1' + &digit - digits;
            }
        }
        return rc;
    };
    for (std::string line; std::cin >> line; )
    {
        char v[] = {
            getdigit(line.begin(), line.end(),
                     [](auto const& s){ return std::pair{s.begin(), s.end()}; }),
            getdigit(line.rbegin(), line.rend(),
                     [](auto const& s){ return std::pair{s.rbegin(), s.rend()}; }),
            char{}
        };
        sum += std::atoi(v);
    }
    std::cout << sum << "\n";
}
