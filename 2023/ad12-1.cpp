#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int check(std::string damage, std::vector<int> const& counts)
{
    auto it{std::find(damage.begin(), damage.end(), '?')};
    if (it != damage.end())
    {
        int sum{};
        *it = '.';
        sum += check(damage, counts);
        *it = '#';
        sum += check(damage, counts);
        return sum;
    }
    auto dit{damage.begin()};
    auto cit{counts.begin()};
    while ((dit = std::find(dit, damage.end(), '#')) != damage.end() && cit != counts.end())
    {
        auto it{std::find(dit, damage.end(), '.')};
        if (distance(dit, it) != *cit)
        {
            return 0;
        }
        ++cit;
        dit = it;
    }
    return dit == damage.end() && cit == counts.end();
}

int main() {
    int sum{};
    for (std::string line; std::getline(std::cin, line); )
    {
        std::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream lin(line);
        std::string damage;
        lin >> damage;
        std::vector<int> counts{std::istream_iterator<int>(lin), {}};

        sum += check(damage, counts);
    }
    std::cout << sum << "\n";
}
