#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int compute_gain(int id, auto const& match_count, auto& added_count)
{
     auto it = added_count.find(id);
     if (it != added_count.end())
         return it->second;
     auto count = match_count.at(id);
     int sum{1};
     for (auto i{id}; i < id + count; ++i)
     {
         sum += compute_gain(i + 1, match_count, added_count);
     }
     return added_count[id] = sum;
}

int main() {
    auto convert = [](auto const& list)
    {
        std::istringstream in(list);
        return std::vector<int>{std::istream_iterator<int>(in), std::istream_iterator<int>()};
    };
    unsigned long long sum{};
    std::unordered_map<int, int> match_count;
    std::unordered_map<int, int> added_count;
    int i{1};
    for (std::string numbers, winning;
         std::getline(std::getline(std::cin.ignore(10000, ':'), numbers, '|'), winning); ++i)
    {
        auto n = convert(numbers);
        std::sort(n.begin(), n.end());
        auto w = convert(winning);
        std::sort(w.begin(), w.end());
        std::vector<int> intersection;
        std::set_intersection(n.begin(), n.end(), w.begin(), w.end(), std::back_inserter(intersection));
        match_count[i] = intersection.size();
    }
    for (std::size_t id{1}; id <= match_count.size(); ++id)
    {
        sum += compute_gain(id, match_count, added_count);
    }
    std::cout << sum << "\n";
}
