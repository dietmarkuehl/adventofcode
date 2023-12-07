#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

int classify(std::string const& s)
{
    std::unordered_map<char, int> counts;
    for (auto c: s)
    {
        ++counts[c];
    }
    int j{};
    if (counts.find('J') != counts.end())
    {
        j = counts['J'];
        counts.erase('J');
    }
    std::vector<int> values;
    for (auto[c, v]: counts)
    {
        values.push_back(v);
    }
    std::sort(values.begin(), values.end());

    if (j != 0)
    {
        if (values.size() <= 1u)
            return 1; // five of a kind
        else if (values.size() == 2)
            // 1, 1, j=3 => four of a kind
            // 1, 2, j=2 => four of a kind
            // 2, 2, j=1 => full-house
            return values[1] + j == 4? 2: 3; // four of a kind, full-house
        else if (values.size() == 3)
            // 1, 1, 1, j=2 => three of a kind
            // 1, 1, 2, j=1 => three of a kind
            return values[2] + j == 3? 4: 5;  // three of a kind, two pair
        else if (values.size() == 4)
            // 1, 1, 1, 1, j=1
            return 6; // one pair
        else
            return 7; // high card => can't happen
    }
    else
    {
        if (counts.size() == 1)
            return 1; // five of a kind
        else if (counts.size() == 2)
            return values[0] == 1? 2: 3; // four of a kind, full-house
        else if (counts.size() == 3)
            return values[2] == 3? 4: 5;  // three of a kind, two pair
        else if (counts.size() == 4)
            return 6; // one pair
        else
            return 7; // high card
    }
}

int main() {
    std::vector<std::tuple<int, std::string, int>> hands;
    for (auto[hand,value] = std::pair(std::string(), std::uint64_t()); std::cin >> hand >> value; )
    { 
        auto kind = classify(hand);
        hands.emplace_back(kind, hand, value);
    }
    auto cmp = [](auto const& a1, auto const& a2) {
         auto const&[k1, h1, v1] = a1;
         auto const&[k2, h2, v2] = a2;
         static char r[] = { 'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J' };
         static auto rank = [](char c){ return std::find(std::begin(r), std::end(r), c); };
         if (k1 != k2)
             return k1 < k2;
         auto[it1, it2] = std::mismatch(h1.begin(), h1.end(), h2.begin());
         return rank(*it1) < rank(*it2);

    };
    std::sort(hands.begin(), hands.end(), cmp);
    std::reverse(hands.begin(), hands.end());
    std::uint64_t sum{};
    for (unsigned r{}; r != hands.size(); ++r)
        sum += (r + 1) * std::get<2>(hands[r]);
    std::cout << sum << "\n";
}
