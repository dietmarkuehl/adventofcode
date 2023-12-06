#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

auto readline(std::istream& in)
{
    return std::vector<int>{std::istream_iterator<int>{in.ignore(10000, ':')}, {}};
}

int main() {
    auto times = readline(std::cin);
    std::cin.clear();
    auto distances = readline(std::cin);
    int factor{1};
    for (unsigned i{}; i < times.size(); ++i)
    {
        int count{0};
        for (int j{0}; j < times[i]; ++j)
        {
             if (distances[i] < j * (times[i] - j))
             {
                 ++count;
             }
        }
        factor *= std::max(1, count);
    }
    std::cout << factor << "\n";
}
