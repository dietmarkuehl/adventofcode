#include <iostream>
#include <string>
#include <sstream>
#include <queue>

int main() {
    std::priority_queue<int> queue;
    int current = 0;
    for (std::string line; std::getline(std::cin, line); ) {
       int value{};
       if (std::istringstream(line) >> value) {
            current += value;
       }
       else {
	    queue.push(current);
            current = 0;
        }
    }
    int top = 0;
    for (int i = 0; i != 3 && !queue.empty(); ++i) {
        top += queue.top();
	queue.pop();
    }
    std::cout << top << "\n";
}
