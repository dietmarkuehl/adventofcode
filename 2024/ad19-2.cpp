#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdlib>
#include <string>

std::size_t match(auto& tried, auto const& ps, std::string_view value) {
    if (tried.contains(value)) {
        return tried[value];
    }
    auto& t = tried[value];
    if (value.empty()) {
        t = 1;
    }
    else {
        for (auto const& p: ps) {
            if (value.starts_with(p)) {
               t += match(tried, ps, value.substr(p.size()));
            }
        }
    }
    return t;
}

int main() {
    std::string ps;
    if (not (std::getline(std::cin, ps) >> std::ws)) {
        std::cout << "failed to read patterns\n";
        return EXIT_FAILURE;
    }
    std::replace(ps.begin(), ps.end(), ',', ' ');
    std::vector<std::string> patterns(std::istream_iterator<std::string>(std::istringstream(ps) >> std::ws), {});

    std::size_t count{};
    for (std::string line; std::cin >> line; ) {
        std::unordered_map<std::string_view, std::size_t> tried;
        auto add = match(tried, patterns, line);
        count += add;
    }
    std::cout << count << "\n";
}
