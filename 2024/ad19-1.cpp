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

bool match(auto& tried, auto const& ps, std::string_view value) {
    if (tried.contains(value)) {
        return false;
    }
    if (value.empty()) {
        return true;
    }
    for (auto const& p: ps) {
         if (value.starts_with(p) && match(tried, ps, value.substr(p.size())))
             return true;
    }
    tried.insert(value);
    return false;
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
        std::unordered_set<std::string_view> tried;
        count += match(tried, patterns, line);
    }
    std::cout << count << "\n";
}
