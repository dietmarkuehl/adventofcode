#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::istream& sep(std::istream& in) {
    in.ignore(1);
    return in;
}

int main() {
    int count{};
    int total{};
    for (int s0, e0, s1, e1; std::cin >> s0 >> sep >> e0 >> sep >> s1 >> sep >> e1; ) {
        ++total;
	if (!(e0 < s1) && !(e1 < s0)) {
	    ++count;
	}
    }
    std::cout << count << '\n';
}
