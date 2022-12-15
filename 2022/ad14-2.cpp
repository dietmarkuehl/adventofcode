#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::istream& comma(std::istream& in) {
    return in.ignore(1, ',');
}

std::istream& arrow(std::istream& in) {
    return (in >> std::ws).ignore(1, '-').ignore(1, '>');
}

int main() {
std::cout << std::unitbuf;
    std::vector<std::vector<std::pair<int, int>>> lines;
    int minx{std::numeric_limits<int>::max()};
    int miny{0};
    int maxx{std::numeric_limits<int>::min()};
    int maxy{std::numeric_limits<int>::min()};

    for (std::string line; std::getline(std::cin, line); ) {
        //std::cout << "line=" << line << "\n";
        int x, y;
        lines.emplace_back();
        for (std::istringstream lin(line); lin >> x >> comma >> y; lin >> arrow) {
            //std::cout << x << "->" << y << "\n";
            lines.back().emplace_back(x, y);
            minx = std::min(x -1, minx);
            maxx = std::max(x +1, maxx);
            maxy = std::max(y +2, maxy);
        }
    }
    int height = maxy - miny;
    minx -= height;
    maxx += height;

    std::vector<std::string>  rocks(maxy - miny, std::string(maxx - minx + 1, '.'));
    rocks.push_back(std::string(maxx - minx + 1, 'a'));

    for (auto const& l: lines) {
         for (std::size_t i{1}; i < l.size(); ++i) {
             if (l[i-1].first == l[i].first) {
                 for (int y = std::min(l[i -1].second, l[i].second); y != std::max(l[i -1].second, l[i].second); ++y) {
                     rocks[y][l[i].first - minx] = '#';
                 }
             }
             else if (l[i-1].second == l[i].second) {
                 for (int x = std::min(l[i -1].first, l[i].first); x != std::max(l[i -1].first, l[i].first) + 1; ++x) {
                     rocks[l[i].second][x - minx] = '#';
                 }
             }
             else {
                 std::cout << "unknown\n";
                 return 0;
             }
         }
    }
    auto print = [&]{
        for (auto const& s: rocks) {
            std::cout << s << "\n";
        }
        std::cout << "\n";
    };
    int total{};
    print();
    for (std::size_t i = 0; i < rocks.size() * rocks[0].size(); ++i) {
        int x{500 - minx}, y{0};
        while (rocks[y+1][x-1] == '.' || rocks[y+1][x] == '.' || rocks[y+1][x+1] == '.') {
            x += rocks[y+1][x] == '.'? 0: rocks[y+1][x-1] == '.'? -1: 1;
            ++y;
        }
        if (rocks[y][x] != '.') {
           break;
        }
        rocks[y][x] = 'o';
        ++total;
    }
    print();
    std::cout << total << "\n";
}
