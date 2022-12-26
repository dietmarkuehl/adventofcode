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
#include <queue>

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
     return out << this->valve->name;
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

void shortest(node const* n, std::vector<std::size_t>& dist, std::vector<bool> marker) {
    std::queue<std::pair<node const*, std::size_t>> q;
    marker[n->index()] = true;
    q.push({ n, std::size_t{} });
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        dist[p.first->index()] = p.second;
        for (tunnel const& i: *p.first) {
            node const* a = i.valve;
            if (!marker[a->idx]) {
                marker[a->idx] = true;
                q.push({ a, p.second + 1u });
            }
        }
    }
}

using simulation_time = int;

struct sequence {
    int value{};
    std::vector<std::pair<simulation_time, node const*>> seq;
    bool operator< (sequence const& other) const { return this->value < other.value; }

    friend std::ostream& operator<< (std::ostream& out, sequence const& s) {
        out << s.value;
        for (auto const& seq: s.seq) {
            out << " (" << seq.first << ", " << seq.second->name << ")";
        }
        return out;
    }
};

struct state {
    using next_elem = std::pair<simulation_time, node const*>;
    using next_list = std::vector<next_elem>;

    std::vector<std::vector<std::size_t>> const& allpair;
    std::vector<node const*> const&              nodes;
    int*                                         overall_max;

    int                remaining_flow;
    sequence           flow{};
    std::vector<bool>  open;

    std::priority_queue<next_list, std::vector<next_list>,
                        decltype([](auto const& l1, auto const& l2){ return l1.back().first > l2.back().first; })> queue;

    state(auto const& allpair, std::vector<node const*> const& nodes, int* overall_max,
          node const* start, std::size_t size, int total_flow, std::vector<node const*> allNodes)
        : allpair(allpair)
        , nodes(nodes)
        , overall_max(overall_max)
        , remaining_flow(total_flow)
        , open(size, false) {
        for (auto n: allNodes) {
            if (n->rate == 0) {
                open[n->index()] = true;
            }
        }
        this->push(0, start);
        this->push(0, start);
    }
    bool empty() const { return this->queue.empty(); }
    void push(simulation_time time, node const* current) {
        state::next_list list;
        for (auto next: this->nodes) {
            int dist(1u + this->allpair[current->index()][next->index()]);
            if (!this->open[next->index()]
                && (*this->overall_max < this->flow.value + this->remaining_flow * (max_time - (time + dist - 1)))
                ) {
                list.emplace_back(time + dist, next);
            }
        }
        if (!list.empty()) {
            std::sort(list.begin(), list.end(), std::greater<>());
            this->queue.emplace(list);
        }
    }
};

sequence search(state s) {
    if (s.empty()) {
        *s.overall_max = std::max(*s.overall_max, s.flow.value);
        return s.flow;
    }
    auto list = s.queue.top();
    s.queue.pop();
    auto[time, current] = list.back();
    list.pop_back();

    sequence max{s.flow};
    if (!list.empty()) {
        state ts{s};
        ts.queue.emplace(list);
        max = search(ts);
    }

    if (!s.open[current->index()]) {
        s.open[current->index()] = true;
        s.flow.value += (max_time - time) * current->rate;
        s.flow.seq.emplace_back(time, current);
        s.remaining_flow -= current->rate;

        s.push(time, current);

        max = std::max(max, search(s));
    }
    *s.overall_max = std::max(*s.overall_max, s.flow.value);
    return max;
}

int main(int ac, char* av[]) {
    graph g = ac == 2? read(std::ifstream(av[1])): read(std::cin);

    int total_flow{};
    std::vector<node const*> nodes;
    for (auto& n: g) {
        total_flow += n.rate;
        nodes.push_back(&n);
    }

    std::vector<std::vector<std::size_t>> allpair(nodes.size(), std::vector<std::size_t>(nodes.size(), 0));
    std::vector<bool> marker(g.size(), false);
    for (node const* n: nodes) {
        shortest(n, allpair[n->index()], marker);
    }

    int max{};
    sequence total = search(state(allpair, nodes, &max, g.get_node("AA"), nodes.size(), total_flow, nodes));
    std::cout << total << "\n";
}
