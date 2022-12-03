#include <iostream>
#include <string>
#include <sstream>

int main() {
    int max = 0;
    int current = 0;
    for (std::string line; std::getline(std::cin, line); ) {
        int value{};
        if (std::istringstream(line) >> value) {
            current += value;
        }
        else {
            max = std::max(current, max);
            current = 0;
        }
    }
    std::cout << max << "\n";
}
