#include <iostream>
#include <string>
#include <sstream>
#include <queue>

int main() {
    std::priority_queue<int> queue;
    int current = 0;
    for (std::string line; std::getline(std::cin, line); ) {
        if (line.empty()) {
	    queue.push(current);
            current = 0;
        }
        else {
            int value{};
            std::istringstream(line) >> value;
            current += value;
        }
    }
    int top = 0;
    for (int i = 0; i != 3 && !queue.empty(); ++i) {
        top += queue.top();
	queue.pop();
    }
    std::cout << top << "\n";
}
