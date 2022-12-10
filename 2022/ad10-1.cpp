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
    int next{20};
    int total{};
    for (auto const& instruction: program) {
        int prev = c.X;
        instruction(c);
        if (next <= c.clock) {
            int add = next != c.clock? prev: c.X;
            total += next * add;
            //std::cout << next << " " << c.clock << " " << c.X << " -> " << (next * add) << "\n";
            next += 40;
        }
        //std::cout << c << "\n";
    }
    std::cout << total << "\n";
}
