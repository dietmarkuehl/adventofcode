#include <iostream>
#include <string>
#include <sstream>

int main() {
    int max = 0;
    int current = 0;
    for (std::string line; std::getline(std::cin, line); ) {
        if (line.empty()) {
            max = std::max(current, max);
            current = 0;
        }
        else {
            int value{};
            std::istringstream(line) >> value;
            current += value;
        }
    }
    std::cout << max << "\n";
}
