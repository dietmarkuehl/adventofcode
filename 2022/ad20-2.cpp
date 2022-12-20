#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

struct node {
    std::int64_t value{};
    node*        next{this};
    node*        prev{this};
    node(std::int64_t value): value(value) {}
    node* erase() {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        return this;
    }
    node* append(node* n) {
        this->next->prev = n;
        n->next = this->next;
        this->next = n;
        n->prev = this;
        return n;
    }
    node* find(std::int64_t value) {
        for (node* it = this; ; it = it->next) {
            if (it->value == value) {
                return it;
            }
        }
    }
};

void apply_mix(std::vector<node*> const& nodes) {
    auto print = [size=nodes.size(), front=nodes.front()]{
        if (size < 8) {
            auto it = front;
            for (std::size_t i{}; i != size; ++i) {
                std::cout << it->value << ", ";
                it = it->next;
            }
            std::cout << "\n";
        }
    };
    for (node* it: nodes) {
        std::int64_t i = it->value;
        if (i < 0) {
            auto pos = it->prev;
            it->erase();
            for (i = (-i) % (nodes.size() - 1); i--; ) {
                pos = pos->prev;
            }
            pos->append(it);
        }
        else {
            i = i % (nodes.size() - 1);
            if (i != 0) {
                auto pos = it->next;
                it->erase();
                while (--i) {
                    pos = pos->next;
                }
                pos->append(it);
            }
        }
    }
    print();
}

int main() {
    constexpr std::int64_t key{811589153};
    std::cout << std::unitbuf;
    std::vector<std::int64_t> input(std::istream_iterator<std::int64_t>(std::cin), std::istream_iterator<std::int64_t>{});
    std::vector<node*> nodes;
    node* start = new node(0);
    node* mix = start;
    for (std::int64_t i: input) {
       nodes.push_back(new node(i * key));
       mix = mix->append(nodes.back());
    }
    delete start->erase();

    for (int i{}; i != 10; ++i) {
        apply_mix(nodes);
    }
    node* it = nodes.front()->find(0);

    std::cout << std::numeric_limits<std::int64_t>::max() << "\n";
    std::int64_t total{};
    for (int i{}; i != 3; ++i) {
        for (int j{}; j != 1000; ++j) {
            it = it->next;
        }
        total += it->value;
    }
    std::cout << total << "\n";
}
