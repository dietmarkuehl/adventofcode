#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stack>
#include <optional>

static int indent_index = std::ios::xalloc();
std::ostream& indent(std::ostream& out) {
    for (int i = out.iword(indent_index); i--; ) {
        out << "    ";
    }
    return out;
}
struct increase {
    std::ostream& out;
    increase(std::ostream& out): out(out) {
        ++out.iword(indent_index);
    }
    ~increase() {
        --out.iword(indent_index);
    }
};

struct node {
    int                d_size{};
    mutable std::optional<int> d_total;
    std::unordered_map<std::string, node> entries;

    int total() const {
        if (!d_total) {
            int rc{this->d_size};
            for (auto& n: entries) {
                rc += n.second.total();
            }
            d_total = rc;
        }
        return *d_total;
    }

    friend std::ostream& operator<< (std::ostream& out, node const& n) {
        out << " " << n.d_size << " total=" << n.total() << '\n';
        increase inc(out);

        for (auto const& entry: n.entries) {
             out << indent << entry.first << entry.second << '\n';
        }
        return out;
    }
};

void find(node const& n, auto apply) {
    apply(n);
    for (auto const& n: n.entries) {
        find(n.second, apply);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    node root;
    std::stack<node*> current;

    for (std::string line; std::getline(std::cin, line); ) {
        if (5 < line.size() && line.substr(0, 5) == "$ cd ") {
            std::string dir;
            if (std::istringstream(line.substr(5)) >> dir) {
                if (dir == "/") {
                    while (!current.empty()) {
                         current.pop();
                    }
                    current.push(&root);
                }
                else if (dir == "..") {
                    current.pop();
                }
                else {
                    current.push(&current.top()->entries[dir]);
                }
            }
        }
        else if (4 <= line.size() && line.substr(0, 4) == "$ ls") {
        }
        else {
            int size{};
            std::string name;
            if (std::istringstream(line) >> size >> name) {
                current.top()->entries[name].d_size = size;
            }
        }
    }

    const int total_disk = 70000000;
    const int total_need = 30000000;
    const int total_avail = total_disk - root.total();
    const int total_miss = total_need - total_avail;
    int min{root.total()};
    find(root, [&](auto& n){ if (n.d_size == 0 && total_miss <= n.total() ) min = std::min(min, n.total()); });
    std::cout << min << "\n";
}
