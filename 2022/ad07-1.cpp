#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

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
    int         size{};
    std::unordered_map<std::string, node> entries;

    int total() const {
        int rc{this->size};
	for (auto& n: entries) {
	    rc += n.second.total();
	}
	return rc;
    }

    friend std::ostream& operator<< (std::ostream& out, node const& n) {
        out << " " << n.size << " total=" << n.total() << '\n';
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
	        current.top()->entries[name].size = size;
	    }
	}
    }

    const int limit = 100000;
    int total{};
    find(root, [&total](auto& n){ if (n.size == 0 && n.total() < limit) total += n.total(); });
    std::cout << total << "\n";
}
