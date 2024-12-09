#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>
#include <limits>

using value_type = std::uint64_t;

bool match(value_type exp, std::vector<value_type> const& values, value_type val, std::size_t index)
{
    if (index == values.size()) {
        return val == exp;
    }
    return match(exp, values, val * values[index], index + 1)
        || match(exp, values, val + values[index], index + 1);
}

bool match(value_type exp, std::vector<value_type> const& values)
{
    return match(exp, values, values[0], 1u);
}
#if 0
bool match(value_type exp, std::vector<value_type> const& values)
{
    std::uint32_t max_ops{1u << (values.size() - 1)};
    for (std::uint32_t i{}; i < max_ops; ++i) {
        value_type val{values[0]};
        auto bits(i);
        for (std::size_t index{1}; index < values.size(); ++index)
        {
            if (bits & 1u)
                val += values[index];
            else
                val *= values[index];
            bits >>= 1;
        }
        if (val == exp)
            return true;
    }
    return false;
}
#endif

int main() {
    std::uint64_t count{};
    for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
        std::istringstream lin(line);
        value_type exp;
        (lin >> exp).ignore();
        std::vector<value_type> values{std::istream_iterator<uint64_t>(lin), {}};
        if (match(exp, values)) {
            count += exp;
        }
    }
    std::cout << count << "\n";;
}
