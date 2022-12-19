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
   int   d_value{};

   bool ignore(std::vector<bool> const& open) const;
   int  value(int time, std::vector<bool> const& open) const;

   std::ostream& print(std::ostream& out) const;

   std::string target() const;
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
        out << "valve=" << n.name << " rate=" << n.rate << " [";
        bool comma(false);
        for (auto const& tunnel: n.tunnels) {
            out << (comma? ", ": "") << tunnel;
            comma = true;
        }
        return out << "]";
    }
};

bool tunnel::ignore(std::vector<bool> const& open) const { return d_value && open[valve->index()]; }
int  tunnel::value(int time, std::vector<bool> const& open) const { return open[valve->index()]? 0: (26 - time) * d_value; }
std::string tunnel::target() const { return this->valve->name; }
std::ostream& tunnel::print(std::ostream& out) const {
     return out << "(" << this->valve->name << ", " << this->cost << ", " << this->d_value << ")";
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
    std::vector<node>::const_iterator begin() const { return this->nodes.begin(); }
    std::vector<node>::const_iterator end() const { return this->nodes.end(); }

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

struct state {
    int               value{};
    int               time{};
    node const*       n0;
    node const*       n1;
    std::vector<bool> open;
    std::vector<bool> v0;
    std::vector<bool> v1;

    state(node const* start, std::vector<bool> const& open)
        : n0(start)
        , n1(start)
        , open(open)
        , v0(std::vector<bool>(this->open.size(), false))
        , v1(std::vector<bool>(this->open.size(), false))
    {
    }
private:
    void traverse(node const*& n, std::vector<bool>& visited, tunnel const& t) {
        int val = t.value(this->time, this->open);
        if (val) {
            this->value = val;
            this->open[n->index()] = true;
            visited = std::vector<bool>(visited.size(), false);
        }
        else {
            this->value = 0;
            visited[n->index()] = true;
            n = t.valve;
        }
    }

public:

    state traverse(tunnel const& t0, tunnel const& t1) const {
        state rc(*this);
        ++rc.time;
        rc.traverse(rc.n0, rc.v0, t0);
        rc.traverse(rc.n1, rc.v1, t1);

        return rc;
    }

    int brute() {
        if (25 < this->time) {
            return 0;
        }
        int max{};
    
        for (tunnel const& t0: this->n0->tunnels) {
            for (tunnel const& t1: this->n1->tunnels) {
                if (true
                    && !t0.ignore(this->open)
                    && !t1.ignore(this->open)
                    && !this->v0[t0.valve->index()]
                    && !this->v1[t1.valve->index()]
                    ) {
                    state ns{this->traverse(t0, t1)};
                    //std::cout << space(ns.time) << ns.value << "\n";
                    int value = ns.brute();
                    max = std::max(max, ns.value + value);
                }
            }
        }

        return max;
    }
};

struct point {
   double x{}, y{};
};

void to_xml(graph const& g) {
    std::unordered_map<std::string, point> positions{
        { "IX", point{255.8, 166} },
        { "ZZ", point{222.4, 220.3} },
        { "TN", point{257.9, 207.2} },
        { "IF", point{289.1, 146.8} },
        { "OU", point{322.5, 131.8} },
        { "DS", point{321.7, 173.1} },
        { "UH", point{221.3, 111.5} },
        { "ZQ", point{160.3, 128.7} },
        { "OP", point{184, 98.3} },
        { "FS", point{254.2, 130} },
        { "IS", point{241.7, 301.4} },
        { "RW", point{295, 247.8} },
        { "WO", point{269.8, 275.4} },
        { "SI", point{204.9, 288.9} },
        { "WZ", point{147.3, 314.1} },
        { "KQ", point{181, 327.7} },
        { "EB", point{286.9, 108.8} },
        { "II", point{235.6, 266.6} },
        { "GR", point{234.5, 188.9} },
        { "HA", point{208.1, 178.2} },
        { "BO", point{255.9, 240.7} },
        { "RI", point{307.4, 284.1} },
        { "OW", point{278.4, 314.6} },
        { "FR", point{175.8, 190.3} },
        { "MT", point{169.8, 289} },
        { "DV", point{184.9, 221.1} },
        { "WG", point{142, 154.7} },
        { "VE", point{12.2, 199.7} },
        { "ZK", point{36.1, 96.9} },
        { "HM", point{137.9, 199.3} },
        { "XB", point{117.1, 181.9} },
        { "XC", point{211.7, 322.3} },
        { "NH", point{30.4, 247.1} },
        { "AA", point{41.7, 202.4} },
        { "KO", point{56.7, 146.2} },
        { "RN", point{191.3, 259.5} },
        { "MF", point{146.3, 90.6} },
        { "LK", point{197.3, 134.9} },
        { "BH", point{120.8, 123.1} },
        { "KZ: ", point{332, 249.2} },
        { "GB", point{320.2, 210.4} },
        { "TC", point{216.8, 246.1} },
        { "DF", point{83.9, 239} },
        { "EZ", point{119.9, 269} },
        { "IU", point{106.4, 360.5} },
        { "DU", point{75.2, 289.7} },
        { "RU", point{120.3, 299.3} },
        { "SX", point{225.2, 150.7} },
        { "AO", point{287.2, 218.6} },
        { "MX", point{88.6, 154.6} },
        { "IP", point{109.4, 333.7} },
        { "WN", point{212.3, 72.4} },
        { "NP", point{251, 89.6} },
        { "VX", point{285.7, 183.6} }
        };

    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
              << "<graphml>\n"
              << "  <graph id=\"Graph\" uidGraph=\"4\" uidEdge=\"10006\">\n";

    std::size_t id{};
    std::unordered_map<std::string, std::size_t> ids;

    for (auto const& n: g) {
        point pos = positions[n.name];
        auto nid{ids.insert(std::make_pair(n.name, id))};
        if (nid.second) {
            ++id;
        }
        std::cout << "    <node positionX=\"" << int(3 * pos.x) << "\" positionY=\"" << int(3 * pos.y) << "\" id=\"" << nid.first->second << "\" mainText=\"" << n.name << "\" upText=\"" << n.rate << "\" size=\"30\" ></node>\n";
    }
    std::size_t eid{};
    for (auto const& n: g) {
        for (auto const& e: n) {
            std::cout << "    <edge source=\"" << ids[n.name] << "\" target=\"" << ids[e.target()] << "\" "
                      << "isDirect=\"true\" weight=\"1\" useWeight=\"false\" "
                      << "id=\"" << eid++ << "\" text=\"\" upText=\"\" arrayStyleStart=\"\" arrayStyleFinish=\"\" model_width=\"4\" model_type=\"0\" model_curveValue=\"0.1\" ></edge>\n";
        }
    }
    std::cout << "  </graph>\n"
              << "</graphml>\n";
}

void add_valves(graph& g) {
    for (node& n: g) {
        if (0 < n.rate) {
            n.tunnels.push_back(tunnel{&n, 1, n.rate});
        }
    }
}

int main(int ac, char* av[]) {
    std::cout << std::unitbuf;
    graph g = ac == 2? read(std::ifstream(av[1])): read(std::cin);
    add_valves(g);
    std::cout << g << "\n";

    std::vector<bool> unset(g.size(), false);
    std::vector<bool> open(unset);
    for (node& n: g) {
        if (n.rate == 0) {
            open[n.index()] = true;
        }
    }

    state s(g.get_node("AA"), open);
    auto total = s.brute();

    std::cout << total << "\n";
}
