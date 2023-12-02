#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

constexpr auto is_digit = [](unsigned char c){ return std::isdigit(c); };
int main()
{
    int sum{};
    auto getdigit = [](auto begin, auto end)
    {
        return *std::find_if(begin, end, is_digit);
    };
    for (std::string line; std::cin >> line; )
    {
        char v[] = {
            getdigit(line.begin(), line.end()),
            getdigit(line.rbegin(), line.rend()),
            char{}
        };
        sum += std::atoi(v);
    }
    std::cout << sum << "\n";
}
