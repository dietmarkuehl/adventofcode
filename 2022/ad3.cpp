#include <iostream>

int main() {
    int score(0);
    for (char other, mine; std::cin >> other >> mine; ) {
        int offset = (other - mine + 'X') % 3;
        score += mine - 'X' + 1 + 3 * (offset? offset == 1? 2: 1: 0);
    }
    std::cout << score << '\n';
}
