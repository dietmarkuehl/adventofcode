#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <queue>

enum class level { low, high };
std::ostream& operator<< (std::ostream& out, level l)
{
    return out << (l == level::low? "low": "high");
}

struct module
{
    std::string                            name;
    char                                   type{};
    std::unordered_map<std::string, level> inputs;
    level                                  state{};
    std::vector<std::string>               destinations;
    void send(auto& queue, level pulse)
    {
        for (auto const& d: destinations)
        {
            queue.emplace(name, d, pulse);
        }
    }
    void operator()(auto& queue, std::string const& from, level pulse)
    {
        switch (type)
        {
        case '%':
            if (pulse == level::low)
            {
                state = state == level::low? level::high: level::low;
                send(queue, state);
            }
            break;
        case '&':
            {
                inputs[from] = pulse;
                auto it = std::find_if(inputs.begin(), inputs.end(),
                                       [](auto const& p){ return p.second == level::low; });

                send(queue, it == inputs.end()? level::low: level::high);
            }
            break;
        case 'b':
            send(queue, level::low);
            break;
        }
    }
};

int main() {
    using result = std::pair<std::uint64_t, std::uint64_t>;
    result counts{};
    std::unordered_map<std::string, module> modules;
    std::queue<std::tuple<std::string, std::string, level>> queue;
    auto done{[&queue, &counts, &modules]{
        for (int i{}; i != 1000; ++i)
        {
            queue.emplace("push", "broadcaster", level::low);
            while (!queue.empty())
            {
                auto[from, to, pulse] = queue.front();
                queue.pop();
                modules[to](queue, from, pulse);
                ++(pulse == level::high? counts.first: counts.second);
            }
        }
        std::cout << (counts.first * counts.second) << "\n";
        modules.clear();
        counts = result{};
    }};
    for (std::string line; std::getline(std::cin, line); )
    {
        if (line.empty())
        {
            done();
            continue;
        }
        std::string name, arrow;
        std::vector<std::string> destinations;
        std::istringstream lin(line);
        lin >> name >> arrow;
        for (std::string destination; std::getline(lin >> std::ws, destination, ','); )
        {
            destinations.push_back(destination);
        }

        if (name[0] == '%' || name[0] == '&')
        {
            char type{name[0]};
            name = name.substr(1);
            modules[name].type = type;
        }
        else
        {
            modules[name].type = 'b';
        }
        modules[name].name = name;
        modules[name].destinations = destinations;
        for (auto const& d: modules[name].destinations)
        {
            modules[d].inputs[name];
        }
    }
    done();
}
