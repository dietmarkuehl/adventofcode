#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <numeric>
#include <vector>
#include <deque>

int main() {
    auto print = [](auto const& r){ for (auto x: r) { std::cout << x << " "; } std::cout << "|"; };
    int count{};
    auto check = [](auto const& vals) {
        std::deque<int> diff;
        std::adjacent_difference(vals.begin(), vals.end(), std::back_inserter(diff));
        diff.pop_front();
        bool increase(0 < diff.front());
        bool same = std::ranges::all_of(diff, [increase](auto x){ return increase? 0 < x: (x < 0); });
        bool max = std::ranges::all_of(diff, [](auto x){ return std::abs(x) < 4; });
        return same && max;
    };
    for (std::string line; std::getline(std::cin, line); )
    {
        std::istringstream lin(line);
        std::vector<int> vals{std::istream_iterator<int>(lin), {}};
        if (check(vals)) {
            ++count;
        }
        else {
            for (std::size_t i{}; i != vals.size(); ++i) {
                auto tmp{vals};
                tmp.erase(tmp.begin() + i);
                if (check(tmp)) {
                    ++count;
                    break;
                }
            }
        }
    }
    std::cout << count << "\n";
}
