#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

int main() {
    int max{};
    int current{};
    for (std::string line; std::getline(std::cin, line); ) {
        if (int value{}; std::istringstream(line) >> value) {
            current += value;
        }
        else {
            max = std::max(current, max);
            current = 0;
        }
    }
    std::cout << max << "\n";
}
