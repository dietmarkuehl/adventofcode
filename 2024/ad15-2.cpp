#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

bool move_vertical(auto& map, std::string const& o, int y, int dy) {
    std::string p(o.size(), ' ');
    for (std::size_t i(0); i < o.size(); ++i) {
        if (o[i] != ' ') {
            switch (map[y + dy][i]) {
            case '#': return false;
            case '[':
                p[i] = '[';
                p[i + 1] = ']';
                break;
            case ']':
                p[i - 1] = '[';
                p[i] = ']';
                break;
            }
        }
    }
    if (p.end() == std::find_if(p.begin(), p.end(), [](char c){ return c != ' '; })
        || move_vertical(map, p, y + dy, dy)) {
        for (std::size_t i(0); i < o.size(); ++i) {
            if (o[i] != ' ') {
                map[y][i] = '.';
            }
            if (o[i] != ' ') {
                map[y + dy][i] = o[i];
            }
        }
        return true;
    }
    return false;
}
int move_vertical(auto& map, int x, int y, int dy)
{
    std::string p(map[0].size(), ' ');
    switch (map[y + dy][x]) {
    case '#': return y;
    case '.': return y + dy;
    case '[':
        p[x] = '[';
        p[x + 1] = ']';
        break;
    case ']':
        p[x - 1] = '[';
        p[x] = ']';
        break;
    }
    if (move_vertical(map, p, y + dy, dy))
        y += dy;
    return y;
}

int main() {
    std::vector<std::string> map;
    //auto print{[&map]{ for (auto const& l: map) std::cout << l << "\n"; std::cout << "\n"; }};
    for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
        std::string l;
        for (auto c: line) {
            switch (c) {
            case '#': l += "##"; break;
            case '.': l += ".."; break;
            case 'O': l += "[]"; break;
            case '@': l += "@."; break;
            }
        }
        map.push_back(l);
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
         switch (c){
         default: break;
         case '<': {
                int tx(x - 1);
                while (map[y][tx] == ']') {
                    tx -= 2;
                }
                if (map[y][tx] == '.') {
                    std::rotate(map[y].begin() + tx, map[y].begin() + tx + 1, map[y].begin() + x + 1);
                    --x;
                }
            }
            break;
         case '>': {
                int tx(x + 1);
                while (map[y][tx] == '[') {
                    tx += 2;
                }
                if (map[y][tx] == '.') {
                    std::rotate(map[y].begin() + x, map[y].begin() + tx, map[y].begin() + tx + 1);
                    ++x;
                }
            }
            break;
         case '^':
            y = move_vertical(map, x, y, -1);
            break;
         case 'v':
            y = move_vertical(map, x, y, 1);
            break;
        }
        //std::cout << "(" << x << ", " << y << "): " << c << "\n";
        map[y][x] = '@';
        //print();
        map[y][x] = '.';
    }

    std::uint64_t sum{};
    for (y = 0; y != int(map.size()); ++y) {
        for (x = 0; x != int(map[0].size()); ++x) {
            if (map[y][x] == '[') {
                sum += 100 * y + x;
            }
        }
    }
    std::cout << sum << "\n";
}
