#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct coord
{
   int x;
   int y;
   bool operator== (coord const&) const = default;
   bool operator< (coord const& o) const {
       return x == o.x? y < o.y: x < o.x;
   }
   friend std::ostream& operator<< (std::ostream& out, coord const& c) {
       return out << "(" << c.x << ", " << c.y << ")";
   }
};

coord const dirs[]{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::size_t walk(auto const& map, int x, int y)
{
    std::vector<coord> ts;
    std::queue<coord> q;
    q.emplace(x, y);
    while (not q.empty()) {
        auto pos = q.front();
        q.pop();
        if (map[pos.y][pos.x] == '9') {
            ts.push_back(pos);
        }
        else {
            for (auto const& d: dirs) {
                auto np = pos;
                np.x += d.x;
                np.y += d.y;
                if (0 <= np.x && np.x < int(map[0].size())
                    && 0 <= np.y && np.y < int(map.size())
                    && map[np.y][np.x] == 1 + map[pos.y][pos.x]) {
                    q.push(np);
                }
            }
       }
    }
    std::sort(ts.begin(), ts.end());
    auto it = std::unique(ts.begin(), ts.end());
    return it - ts.begin();
}

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    std::size_t count{};
    for (std::size_t y{}; y != map.size(); ++y)
        for (std::size_t x{}; x != map[y].size(); ++x)
            if (map[y][x] == '0')
                count += walk(map, x, y);
    std::cout << count << "\n";
}
