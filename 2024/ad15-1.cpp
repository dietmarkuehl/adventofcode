#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map;
    //auto print{[&map]{ for (auto const& l: map) std::cout << l << "\n"; std::cout << "\n"; }};
    for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
        map.push_back(line);
    }
    std::string moves{std::istreambuf_iterator<char>(std::cin), {}};
    moves.erase(std::remove(moves.begin(), moves.end(), '\n'), moves.end());
    //std::cout << moves << "\n\n";
    //print();
    int x{}, y{};
    while (true) {
        x = map[y].find('@');
        if (x != int(std::string::npos))
            break;
        ++y;
    }
    map[y][x] = '.';
    //std::cout << "(" << x << ", " << y << ")\n";
    struct dir { int x; int y; };
    for (char c: moves) {
        auto[dx, dy] =[](char c){
            switch (c){
            default: return dir{0, 0};
            case '<': return dir{-1, 0};
            case '>': return dir{1, 0};
            case '^': return dir{0, -1};
            case 'v': return dir{0, 1};
            }
        }(c);
        int tx = x + dx;
        int ty = y + dy;
        while (map[ty][tx] == 'O') {
            tx += dx;
            ty += dy;
        }
        if (map[ty][tx] == '.') {
            x += dx;
            y += dy;
            map[ty][tx] = map[y][x];
            map[y][x] = '.';
        }
        //std::cout << "(" << x << ", " << y << "): " << c << "\n";
        //print();
    }

    std::uint64_t sum{};
    for (y = 0; y != int(map.size()); ++y) {
        for (x = 0; x != int(map[0].size()); ++x) {
            if (map[y][x] == 'O') {
                sum += 100 * y + x;
            }
        }
    }
    std::cout << sum << "\n";
}
