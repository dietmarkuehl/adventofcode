#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

std::istream& skip(std::istream& in) {
    std::string aux;
    return in >> aux;
}

int main() {
    std::vector<std::vector<char>> stacks;
    for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
	std::size_t len(1 + line.size() / 4);
	stacks.resize(len);
        for (std::size_t i = 0; i < len; ++i) {
	     char c(line[1 + 4 * i]);
	     if (c != ' ') {
	         stacks[i].push_back(c);
	     }
        }
    }
    for (auto& stack: stacks) {
        std::reverse(stack.begin(), stack.end());
    }
    for (int count, from, to; std::cin >> skip >> count >> skip >> from >> skip >> to; ) {
        --to;
        --from;
	for (int i = 0; i < count; ++i) {
	    stacks[to].push_back(stacks[from].back());
	    stacks[from].pop_back();
	}
	std::reverse(stacks[to].end() - count, stacks[to].end());
    }
    for (auto& stack: stacks) {
        std::cout << stack.back();
    }
    std::cout << '\n';
}
