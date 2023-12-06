#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

auto readline(std::istream& in)
{
    std::string line;
    std::getline(in, line);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    return std::vector<std::uint64_t>{std::istream_iterator<std::uint64_t>{std::istringstream(line).ignore(10000, ':')}, {}};
}

int main() {
    auto times = readline(std::cin);
    std::cin.clear();
    auto distances = readline(std::cin);
    uint64_t factor{1};
    for (unsigned i{}; i < times.size(); ++i)
    {
        int count{0};
        for (std::uint64_t j{0}; j < times[i]; ++j)
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
