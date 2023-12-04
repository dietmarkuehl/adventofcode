#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    auto convert = [](auto const& list)
    {
        std::istringstream in(list);
        return std::vector<int>{std::istream_iterator<int>(in), std::istream_iterator<int>()};
    };
    unsigned long long sum{};
    for (std::string numbers, winning;
         std::getline(std::getline(std::cin.ignore(10000, ':'), numbers, '|'), winning); )
    {
        auto n = convert(numbers);
        std::sort(n.begin(), n.end());
        auto w = convert(winning);
        std::sort(w.begin(), w.end());
        std::vector<int> intersection;
        std::set_intersection(n.begin(), n.end(), w.begin(), w.end(), std::back_inserter(intersection));
        if (!intersection.empty())
            sum += 1ull << (intersection.size() - 1);
    }
    std::cout << sum << "\n";
}
