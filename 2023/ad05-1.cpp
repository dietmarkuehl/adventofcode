#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

auto read_seeds()
{
    std::string line;
    std::getline(std::cin.ignore(10, ':'), line) >> std::ws;
    std::istringstream sin(line);
    return std::vector<std::uint64_t>{std::istream_iterator<std::uint64_t>(sin), {}};
}

int main() {
    auto values = read_seeds();
    while (std::cin)
    {
        std::string head;
        std::getline(std::cin, head);
        std::cout << "head=" << head << "\n";
        std::vector<std::uint64_t> tmp;
        std::sort(values.begin(), values.end());
        for (std::string line; std::getline(std::cin, line); )
        {
            std::uint64_t target, source, size;
            if (line.empty())
                break;
            std::istringstream(line) >> target >> source >> size;
            std::cout << line << "\nsource=" << source << " target=" << target << " size=" << size << "\n";
            for (auto it = std::lower_bound(values.begin(), values.end(), source);
                 it != values.end() && *it < source + size; )
            {
                tmp.push_back(target + (*it - source));
                it = values.erase(it);
            }
        }
        tmp.insert(tmp.end(), values.begin(), values.end());
        if (values.size() != tmp.size())
        {
            std::cout << "size mismatch\n";
        }
        values = std::move(tmp);
    }
    std::sort(values.begin(), values.end());
    std::cout << values.front() << "\n";
}
