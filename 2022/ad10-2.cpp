#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct cpu {
    int clock{1};
    int X{1};
    friend std::ostream& operator<< (std::ostream& out, cpu const& c) {
        return out << c.clock << " " << c.X;
    }
};

int main() {
    std::vector<std::function<void(cpu&)>> program;
    for (std::string line; std::getline(std::cin, line); ) {
        std::istringstream lin(line);
        std::string command;
        int         arg;
        if (lin >> command) {
            if (command == "noop") {
                program.push_back([](cpu& c){ c.clock += 1; });
            }
            else if (command == "addx" && lin >> arg) {
                program.push_back([arg](cpu& c){ c.clock += 2; c.X += arg; });
            }
        }
    }
    cpu c;
    auto it = program.begin();
    for (int i = 0; it != program.end(); ++i) {
        int pos = i % 40;
        if (pos == 0) {
            std::cout << "\n";
        }
        std::cout << (std::abs(pos - c.X) <= 1? '#': '.');
        if (i == c.clock) {
            (*it)(c);
            ++it;
        }
    }
    std::cout << "\n";
}
