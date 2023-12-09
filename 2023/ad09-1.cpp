#include <algorithm>
#include <numeric>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::int64_t sum{};
    for (std::string line; std::getline(std::cin, line); )
    {
        std::vector<int> sequence{std::istream_iterator<int>(std::istringstream(line) >> std::ws), {}};
        
        while (true)
        {
            sum += sequence.back();
            std::vector<int> tmp;
            std::adjacent_difference(sequence.begin(), sequence.end(), std::back_inserter(tmp));
            tmp.erase(tmp.begin());
            if (std::all_of(tmp.begin(), tmp.end(), [](int x){ return x == 0; })) {
                break;
            }
            sequence = std::move(tmp);
        }
    }
    std::cout << sum << "\n";
}
