#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

int main() {
    std::cout << std::unitbuf;
    std::string digits[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    std::string rdigits[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    for (auto& s: rdigits)
    {
        std::reverse(s.begin(), s.end());
    }
    int sum{};
    auto digit = [](unsigned char c){ return std::isdigit(c); };
    auto find = [=](std::string const& line, auto const& digits) {
        auto it = std::find_if(line.begin(), line.end(), digit);
        char v = *it;
        for (std::size_t i{0}; i < std::size(digits); ++i) {
            auto const& d{digits[i]};
            auto pos = line.find(d);
            if (pos != line.npos && line.begin() + pos < it)
            {
                it = line.begin() + pos;
                v = '0' + i+1;
            }
        }
        return v;
    };
    for (std::string line; std::cin >> line; )
    {
        std::string v;
        v.push_back(find(line, digits));
        std::reverse(line.begin(), line.end());
        v.push_back(find(line, rdigits));
        sum += std::atoi(v.c_str());
    }
    std::cout << sum << "\n";
}
