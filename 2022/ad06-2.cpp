#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::size_t size(14);
    for (std::string line; std::getline(std::cin, line); ) {
        for (std::size_t i{}; i < line.size() - size; ++i) {
	    std::string tmp = line.substr(i, size);
	    std::sort(tmp.begin(), tmp.end());
	    if (std::unique(tmp.begin(), tmp.end()) == tmp.end()) {
	        std::cout << (i + size) << '\n';
		break;
	    }
	}
    }
    std::cout << "---\n";
}
