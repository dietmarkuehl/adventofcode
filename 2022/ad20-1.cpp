#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct node {
    int   value{};
    node* next{this};
    node* prev{this};
    node(int value): value(value) {}
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
    node* find(int value) {
        for (node* it = this; ; it = it->next) {
            if (it->value == value) {
                return it;
            }
        }
    }
};

int main() {
    std::cout << std::unitbuf;
    std::vector<int> input(std::istream_iterator<int>(std::cin), std::istream_iterator<int>{});
    std::vector<node*> nodes;
    node* start = new node(0);
    node* mix = start;
    for (int i: input) {
       nodes.push_back(new node(i));
       mix = mix->append(nodes.back());
    }
    delete start->erase();

    auto print = [size=input.size(), front=input.front(), &mix]{
        if (size < 8) {
            auto it = mix->find(front);
            for (std::size_t i{}; i != size; ++i) {
                std::cout << it->value << ", ";
                it = it->next;
            }
            std::cout << "\n";
        }
    };
    print();
    for (node* it: nodes) {
        int i = it->value;
        if (i < 0) {
            auto pos = it->prev;
            it->erase();
            for (i = -i; i--; ) {
                pos = pos->prev;
            }
            pos->append(it);
        }
        else if (0 < i) {
            auto pos = it->next;
            it->erase();
            while (--i) {
                pos = pos->next;
            }
            pos->append(it);
        }
        print();
    }
    node* it = mix->find(0);

    int total{};
    for (int i{}; i != 3; ++i) {
        for (int j{}; j != 1000; ++j) {
            it = it->next;
        }
        std::cout << it->value << " ";
        total += it->value;
    }
    std::cout << "total=" << total << "\n";
}
