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
    for (int s0, e0, s1, e1; std::cin >> s0 >> sep >> e0 >> sep >> s1 >> sep >> e1; ) {
	//if (s1 < s0) {
            // std::cout << "before  s1=" << s1 << " e1=" << e1 << " s0=" << s0 << " e0=" << e0 << "\n";
	//    std::swap(s0, s1);
	//    std::swap(e0, e1);
            // std::cout << "swapped s0=" << s0 << " e0=" << e0 << " s1=" << s1 << " e1=" << e1 << "\n";
	//}
	if (s0 <= s1 && e1 <= e0) {
	    ++count;
	}
	else if (s1 <= s0 && e0 <= e1) {
	    ++count;
	}
    }
    std::cout << count << '\n';
}
