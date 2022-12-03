#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    long total{};
    auto sort_unique = [](auto begin, auto end){
        std::sort(begin, end);
        return std::unique(begin, end);
    };
    for (std::string line; std::getline(std::cin, line); ) {
        std::string inter;
        auto mid = line.begin() + line.size() / 2;
        std::set_intersection(
            line.begin(), sort_unique(line.begin(), mid),
            mid, sort_unique(mid, line.end()),
            std::back_inserter(inter)
        );
        total += inter.empty()? 0: std::islower(inter[0])? inter[0] - 'a' + 1: (inter[0] - 'A' + 27);
    }
    std::cout << total << '\n';
}
