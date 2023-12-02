#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

int main() {
    int sum{};
    auto digit = [](unsigned char c){ return std::isdigit(c); };
    for (std::string line; std::cin >> line; )
    {
        auto it = std::find_if(line.begin(), line.end(), digit);
        auto rit = std::find_if(line.rbegin(), line.rend(), digit);
        std::string v;
        v.push_back(*it);
        v.push_back(*rit);
        sum += std::atoi(v.c_str());
    }
    std::cout << sum << "\n";
}
