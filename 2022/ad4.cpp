#include <iostream>

int main() {
    int score(0);
    for (char other, result; std::cin >> other >> result; ) {
        score += (result - 'X')  * 3;
        other -= 'A';
        score += (result == 'Y')? other + 1: result == 'Z'? (other + 1) % 3 + 1: (other? other: 3);
    }
    std::cout << score << '\n';
}
