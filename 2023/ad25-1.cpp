#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <queue>

struct edge
{
    std::string node;
    bool        used{};
};

int main() {
    std::cout << std::unitbuf;
    std::unordered_map<std::string, std::unordered_map<std::string, bool*>> graph;
    std::deque<bool> edges;
    for (std::string line; std::getline(std::cin, line); )
    {
        std::string s, t;
        std::istringstream lin(line);
        std::getline(lin, s, ':');
        while (lin >> t)
        {
            bool*& edge(graph[s][t]);
            if (edge == nullptr)
            {
                edges.push_back(false);
                edge = &edges.back();
            }
            graph[t][s] = edge;
        }
    }

    auto it = graph.begin();
    auto source = (it++)->first;
    for (; it != graph.end(); ++it)
    {
        auto traverse{[&graph, source, target=it->first](bool mark){
            std::unordered_map<std::string, std::string> parent;
            std::queue<std::string> q;
            q.push(source);
            parent[source] = {};
            int count{};
            while (not q.empty() && q.front() != target)
            {
                ++count;
                for (auto& a: graph[q.front()]) {
                   if (not *a.second && not parent.contains(a.first))
                   {
                       q.push(a.first);
                       parent[a.first] = q.front();
                   }
                }
                q.pop();
            }
            if (mark && !q.empty())
            {
                for (std::string c{q.front()}; not parent[c].empty(); c = parent[c])
                {
                    *graph[c][parent[c]] = true;
                }
            }
            return std::pair(not q.empty(), count);
        }};
        std::fill(edges.begin(), edges.end(), false);
        for (int i{}; i != 3; ++i)
        {
            traverse(true);
        }
        auto[reached, count] = traverse(false);
        std::cout << it->first << " reached=" << std::boolalpha << reached << " count=" << count << "/" << graph.size() << "\n";
        if (!reached)
        {
            std::cout << (count * (graph.size() - count)) << "\n";
            break;
        }
    }
}
