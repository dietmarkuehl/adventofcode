#include <algorithm>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <list>
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

using flags = std::bitset<128>;
static_assert(sizeof(flags) == 16);
int   max_time = 26;

struct node;
struct tunnel {
   node* valve;
   std::function<int(int, flags&)> value{[](int, flags&){ return 0; }};

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

    std::vector<tunnel>::const_iterator begin() const { return this->tunnels.begin(); }
    std::vector<tunnel>::const_iterator end() const { return this->tunnels.end(); }
    int index() const { return this->idx; }
    friend std::ostream& operator<< (std::ostream& out, node const& n) {
        out << "valve=" << n.name << "(" << n.idx << ") rate=" << n.rate << " [";
        bool comma(false);
        for (auto const& tunnel: n.tunnels) {
            out << (comma? ", ": "") << tunnel;
            comma = true;
        }
        return out << "]";
    }
};

std::ostream& tunnel::print(std::ostream& out) const {
     flags open{};
     return out << this->valve->name << "(" << this->value(max_time - 1, open) << ")";
}

struct graph {
    int next{};
    std::unordered_map<std::string, int> name_to_index;
    std::list<node> nodes;

    graph(graph&&) = delete;
    graph(std::istream& in) {
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
        index(name);
        auto it = std::find_if(this->nodes.begin(), this->nodes.end(),
                               [&name](node const& n){ return n.name == name; });
        return &*it;
    }
    void erase(node* n) { n->name.clear(); }
    int index(std::string const& name) {
        auto p = this->name_to_index.insert({ name, next });
        if (p.second) {
            this->nodes.push_back(node{name, 0, {}, next});
            ++next;
        }
        return p.first->second;
    }
    void set(std::string const& name, node n) {
        n.idx = this->index(name);
        *get_node(name) = n;
    }
    std::list<node>::iterator begin() { return this->nodes.begin(); }
    std::list<node>::iterator end() { return this->nodes.end(); }
    std::list<node>::const_iterator begin() const { return this->nodes.begin(); }
    std::list<node>::const_iterator end() const { return this->nodes.end(); }

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

struct state {
    int               value{};
    int               time{};
    node const*       n0;
    node const*       n1;
    flags open;
    flags v0;
    flags v1;

    state(node const* start)
        : n0(start)
        , n1(start)
        , open()
        , v0()
        , v1()
    {
    }
private:
    void traverse(node const*& n, flags& visited, tunnel const& t) {
        int value = t.value(this->time, this->open);
        this->value += value;
        if (value) {
            visited = open;
        }
        else {
            visited[n->index()] = true;
        }
        n = t.valve;
    }

public:

    state traverse(tunnel const& t0, tunnel const& t1) const {
        std::cout << n0->name << ", " << n1->name << " ";
        state rc(*this);
        ++rc.time;
        rc.traverse(rc.n0, rc.v0, t0);
        rc.traverse(rc.n1, rc.v1, t1);
        std::cout << "\n";

        return rc;
    }

    int brute() {
        if (max_time == this->time) {
            return this->value;
        }
        int max{};
        //std::cout << "time=" << this->time << " n0=" << this->n0->name << " n1=" << this->n1->name << " value=" << this->value << "\n";
    
        for (tunnel const& t0: this->n0->tunnels) {
            for (tunnel const& t1: this->n1->tunnels) {
                if (true
                    && !this->v0[t0.valve->index()]
                    && !this->v1[t1.valve->index()]
                    ) {
                    if (this->n0 != this->n1 || t0.valve != t1.valve) {
                        state ns{this->traverse(t0, t1)};
                        //std::cout << space(ns.time) << ns.value << "\n";
                        max = std::max(max, ns.brute());
                    }
                }
            }
        }

        return max;
    }
};

void add_valves(graph& g) {
    std::vector<node*> nodes;
    for (node& n: g) {
        nodes.push_back(&n);
    }
    for (node* n: nodes) {
        if (0 < n->rate) {
            node* vn = g.get_node(n->name + "'");
            vn->tunnels = n->tunnels;
            n->tunnels.push_back(tunnel{vn, [index=vn->index(), rate=n->rate, s=n->name, t=vn->name](int time, flags& open){
                std::cout << "time=" << time << " (" << s << ", " << t << ") -> " << (open[index]? 0: (max_time - time) * rate);
                return open[index]? 0: (open[index] = true, (max_time - time) * rate);
                }});
        }
    }
}

int main(int ac, char* av[]) {
    std::cout << std::unitbuf;
    graph g = ac == 2? read(std::ifstream(av[1])): read(std::cin);
    add_valves(g);
    std::cout << g << "\n";

    state s(g.get_node("AA"));
    auto total = s.brute();

    std::cout << total << "\n";
}
