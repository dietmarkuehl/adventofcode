#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::vector<int> queue;
    int current = 0;
    for (std::string line; std::getline(std::cin, line); ) {
       if (int value{}; std::istringstream(line) >> value) {
            current += value;
       }
       else {
	        queue.push_back(std::exchange(current, 0));
        }
    }
    std::sort(queue.begin(), queue.end(), std::greater<>());
    std::cout << std::accumulate(queue.begin(), queue.begin() + std::min(std::size_t(3), queue.size()), 0) << "\n";
}
