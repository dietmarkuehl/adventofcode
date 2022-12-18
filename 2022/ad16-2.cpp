#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>

// ----------------------------------------------------------------------------

std::istream& ignore_to_eq(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), '=');
}
std::istream& ignore_comma(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
}
template <int I>
std::istream& ignore_word(std::istream& in) {
    std::string word;
    for (int i{}; i != I; ++i) {
        in >> word;
    }
    return in;
}

struct space {
    int s;
    space(int s): s(s) {}
    friend std::ostream& operator<< (std::ostream& out, space const& x) {
        for (int i{}; i != x.s; ++i) {
            out << "   ";
        }
        return out;
    }
};

// ----------------------------------------------------------------------------

struct node;
struct tunnel {
   node* valve;
   int   cost{1};
   std::ostream& print(std::ostream& out) const;

   friend std::ostream& operator<< (std::ostream& out, tunnel const& t) {
       return t.print(out);
   }
};
struct node {
    std::string         name;
    int                 rate;
    std::vector<tunnel> tunnels;
    int                 idx{-1};

    int index() const { return this->idx; }
    friend std::ostream& operator<< (std::ostream& out, node const& n) {
        out << "valve=" << n.name << " rate=" << n.rate << " [";
        bool comma(false);
        for (auto const& tunnel: n.tunnels) {
            out << (comma? ", ": "") << tunnel;
            comma = true;
        }
        return out << "]";
    }
};

std::ostream& tunnel::print(std::ostream& out) const {
     return out << "(" << this->valve->name << ", " << this->cost << ")";
}

struct graph {
    int next{};
    std::unordered_map<std::string, int> name_to_index;
    std::vector<node> nodes;

    graph() { nodes.reserve(100); }
    graph(graph&&) = delete;
    graph(std::istream& in)
        :graph() {
        std::string valve;
        int         rate;
        std::vector<tunnel> tunnels;
        for (std::string line; std::getline(in, line); ) {
            if (std::istringstream lin(line); lin >> ignore_word<1> >> valve >> ignore_to_eq >> rate >> ignore_word<5>) {
                for (std::string tun; std::getline(lin >> std::ws, tun, ','); ) {
                    tunnels.push_back({this->get_node(tun)});
                }
            }
            this->set(valve, {valve, rate, tunnels});
            tunnels.clear();
        }
    }
    std::size_t size() const { return this->nodes.size(); }
    node* get_node(std::string const& name) {
        return &this->nodes[this->index(name)];
    }
    void erase(node* n) { n->name.clear(); }
    int index(std::string const& v) {
        auto p = this->name_to_index.insert({ v, next });
        if (p.second) {
            ++next;
            this->nodes.resize(this->next);
        }
        return p.first->second;
    }
    void set(std::string const& v, node n) {
        int index = this->index(v);
        n.idx = index;
        this->nodes[index] = n;
    }
    std::vector<node>::iterator begin() { return this->nodes.begin(); }
    std::vector<node>::iterator end() { return this->nodes.end(); }

    friend std::ostream& operator<< (std::ostream& out, graph const& g) {
        for (node const& n: g.nodes) {
            if (!n.name.empty()) {
                out << n << "\n";
            }
        }
        return out;
    }
};

auto read(auto&& in) {
    return graph(in);
}

void compact(graph& g) {
    std::vector<node*> rm;
    for (auto& n: g) {
        if (n.rate == 0 && n.tunnels.size() == 2) {
            rm.push_back(&n);
            tunnel t0(n.tunnels[0]);
            tunnel t1(n.tunnels[1]);
            node& n0 = *t0.valve;
            node& n1 = *t1.valve;
            auto it0 = std::find_if(n0.tunnels.begin(), n0.tunnels.end(), [&n](auto& t){ return &n == t.valve; });
            auto it1 = std::find_if(n1.tunnels.begin(), n1.tunnels.end(), [&n](auto& t){ return &n == t.valve; });
            n0.tunnels.erase(it0);
            n1.tunnels.erase(it1);
            n0.tunnels.push_back({ t1.valve, t0.cost + t1.cost });
            n1.tunnels.push_back({ t0.valve, t0.cost + t1.cost });
        }
    }
    for (auto const& n: rm) {
        g.erase(n);
    }
}

struct fmt {
    std::vector<bool> v;
    friend std::ostream& operator<< (std::ostream& out, fmt const& v) {
        for (std::size_t i{0}; i != v.v.size(); ++i) {
           out << (v.v[i]? '@': '-');
        }
        return out;
    }
};

int brute(node const& n, node const& en, int time, std::vector<bool> open, std::vector<bool> visited, std::vector<bool> evisited, int np, int limit);
int brute_step(node const& n, node const& en, int time, std::vector<bool> open, std::vector<bool> visited, std::vector<bool> evisited, int np, int limit) {
    if (limit <= np) {
        return 0;
    }
    int max{};
    for (auto const& t: n.tunnels) {
        if (!visited[t.valve->index()]) {
            for (auto const& et: en.tunnels) {
                if (!evisited[et.valve->index()]) {
                    max = std::max(max, brute(*t.valve, *et.valve, time + t.cost, open, visited, evisited, np + 1, limit));
                }
            }
        }
    }
    return max;
}

int brute(node const& n, node const& en, int time, std::vector<bool> open, std::vector<bool> visited, std::vector<bool> evisited, int np, int limit) {
    if (25 < time) {
        return 0;
    }
    visited[n.index()] = true;
    evisited[en.index()] = true;

    int max{};
    max = std::max(max, brute_step(n, en, time, open, visited, evisited, np + 1, limit));
    if (!open[n.index()]) {
        int flow(n.rate * (26 - time));
        open[n.index()] = true;
        max = std::max(max, flow + brute_step(n, en, time + 1, open, std::vector<bool>(visited.size(), false), evisited, 0, limit+3));
        open[n.index()] = false;
    }
    if (!open[en.index()]) {
        int flow(en.rate * (26 - time));
        open[en.index()] = true;
        max = std::max(max, flow + brute_step(n, en, time + 1, open, evisited, std::vector<bool>(visited.size(), false), 0, limit+3));
        open[en.index()] = false;
    }
    if (&n != &en && !open[n.index()] && !open[en.index()]) {
        int flow(n.rate * (26 - time) + en.rate * (26 - time));
        open[n.index()] = true;
        open[en.index()] = true;
        max = std::max(max, flow + brute_step(n, en, time + 1, open, std::vector<bool>(visited.size(), false), std::vector<bool>(visited.size(), false), 0, limit+6));
        open[n.index()] = false;
        open[en.index()] = false;
    }

    return max;
}

int main(int ac, char* av[]) {
    std::cout << std::unitbuf;
    graph g = ac == 2? read(std::ifstream(av[1])): read(std::cin);
    std::cout << g << "\n";
    //compact(g);

    std::vector<bool> unset(g.size(), false);
    std::vector<bool> open(unset);
    for (node& n: g) {
        if (n.rate == 0) {
            open[n.index()] = true;
        }
    }
    auto total = brute(*g.get_node("AA"), *g.get_node("AA"), 1, open, unset, unset, 0, 4);

    std::cout << total << "\n";
}
