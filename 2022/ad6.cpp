#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    long total{};
    int count{};

    std::string inter;
    for (std::string line; std::getline(std::cin, line); ) {
        std::sort(line.begin(), line.end());
        line.erase(std::unique(line.begin(), line.end()), line.end());
        if (++count == 1) {
            inter = line;
        }
        else {
            std::string tmp;
            std::set_intersection(line.begin(), line.end(), inter.begin(), inter.end(), std::back_inserter(tmp));
            inter = tmp;

            if (count == 3) {
                if (inter.size() == 1) {
                    total += std::islower(inter[0])? inter[0] - 'a' + 1: (inter[0] - 'A' + 27);
                }
                count = 0;
            }
        }
    }
    std::cout << total << '\n';
}
