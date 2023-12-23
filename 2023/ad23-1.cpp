#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <map>
#include <iomanip>

struct node;
struct edge
{
    node*                                      target;
    std::vector<std::pair<unsigned, unsigned>> path;
};
struct node
{
    bool              flag{false};
    std::vector<edge> edges;
};

std::size_t max{};

void search(node* n, node* end, std::size_t len)
{
    n->flag = true;
    for (auto i: n->edges)
    {
        if (i.target == end)
        {
            max = std::max(max, len + i.path.size());
        }
        else if (!i.target->flag)
            search(i.target, end, len + i.path.size());
    }
    n->flag = false;
}

int main() {
    std::vector<std::string> map;
    for (std::istream_iterator<std::string> it{std::cin}, end{}; it != end; ++it)
    {
        map.push_back('#' + *it + '#');
    }
    map.insert(map.begin(), std::string(map[0].size(), '#'));
    map.insert(map.end(), std::string(map[0].size(), '#'));
    auto sit{std::find(map[1].begin(), map[1].end(), '.')};
    map[0][sit - map[1].begin()] = 'S';
    auto eit{std::find(map[map.size() - 2].begin(), map[map.size() - 2].end(), '.')};
    map[map.size() - 1][eit - map[map.size() - 2].begin()] = 'E';

    auto print{[&map]{
        std::cout << "size=(" << map[0].size() << ", " << map.size() << ")\n";
        for (std::size_t y{}; y != map.size(); ++y)
        {
            std::cout << std::setw(3) << y << " " << map[y] << "\n";
        }
    }};
    (void)print;
    //print();

    std::map<std::pair<unsigned, unsigned>, node> nodes;

    std::vector<std::tuple<int, int, char>> moves{{-1, 0, '<'}, {1, 0, '>'}, {0, -1, '^'}, {0, 1, 'v'}};
    char slopes[] = {'<', '>', '^', 'v', 'N'};
    auto move{[&map, &slopes](unsigned x, unsigned y, std::tuple<int, int, char> const& m)
        -> std::optional<std::pair<unsigned, unsigned>> {
        auto[dx, dy, s] = m;
        x += dx;
        y += dy;
        if (map[y][x] == '.'
            || std::find(std::begin(slopes), std::end(slopes), map[y][x]) != std::end(slopes))
            return std::pair(x, y);
        return {};
    }};
    auto valid_move{[&map, &move](unsigned x, unsigned y, std::tuple<int, int, char> const& m)
        -> std::optional<std::pair<unsigned, unsigned>> {
        auto nm = move(x, y, m);
        if (!nm)
            return {};
        auto[nx, ny] = *nm;
        if (map[ny][nx] == '.' || map[ny][nx] == 'N' || map[ny][nx] == std::get<2>(m))
            return nm;
        return {};
    }};

    for (unsigned x{1}; x != map[0].size() - 1u; ++x)
    {
        for (unsigned y{1}; y != map.size() - 1u; ++y)
        {
            if (map[y][x] != '#')
            {
                auto valid{[x, y, &move](auto const& m){ return bool(move(x, y, m)); }};
                auto c{std::count_if(moves.begin(), moves.end(), valid)};
                if (c != 2)
                {
                    map[y][x] = 'N';
                    nodes[std::pair(x, y)] = node{};
                }
            }
        }
    }

    for (auto&[pos, n]: nodes)
    {
        auto[x, y] = pos;
        map[y][x] = '@';
        for (auto md: moves)
        {
            auto m{valid_move(x, y, md)};
            if (!m)
                continue;
            auto[nx, ny] = *m;

            std::vector<std::pair<unsigned, unsigned>> path;
            if (m)
            {
                auto valid{[&nx, &ny, &valid_move](auto const& m){ return bool(valid_move(nx, ny, m)); }};
                path.emplace_back(nx, ny);
                if (map[ny][nx] == '.') map[ny][nx] = 'P';
                auto it{std::find_if(std::begin(moves), std::end(moves), valid)};
                while (it != std::end(moves))
                {
                    auto[tx, ty] = *move(nx, ny, *it);
                    nx = tx;
                    ny = ty;
                    if (map[ny][nx] == '.') map[ny][nx] = 'P';
                    path.emplace_back(nx, ny);
                    if (map[ny][nx] == 'N') break;
                    it = std::find_if(std::begin(moves), std::end(moves), valid);
                }
                if (map[ny][nx] == 'N')
                {
                    auto nit{nodes.find(std::pair(nx, ny))};
                    if (nit == nodes.end())
                        std::cout << "taget is not a node!\n";
                    nodes[std::pair(x, y)].edges.emplace_back(&nit->second, path);
                }
                for (auto[px, py]: path)
                    if (map[py][px] == 'P')
                        map[py][px] = '.';
            }
        }
        map[y][x] = 'N';
    }

    auto print_paths{[&]{
        for (auto&[pos, n]: nodes)
        {
            for (auto& edge: n.edges)
            {
                for (auto[x, y]: edge.path)
                {
                    if (map[y][x] == '.')
                        map[y][x] = 'p';
                    else if (map[y][x] == 'p')
                        map[y][x] = 'P';
                }
            }
        }
        print();
    }};
    (void)print_paths;

    auto start{nodes.find(std::pair(sit - map[1].begin(), 1))};
    auto end{nodes.find(std::pair(eit - map[map.size() - 2].begin(), map.size() - 2))};
    search(&start->second, &end->second, 0);
    std::cout << max << "\n";
}
