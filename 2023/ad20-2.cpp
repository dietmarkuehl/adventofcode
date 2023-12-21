#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <queue>
#include <cstdlib>

enum class level { low, high };
std::ostream& operator<< (std::ostream& out, level l)
{
    return out << (l == level::low? "low": "high");
}

struct module
{
    std::string                            name;
    int                                    count{};
    char                                   type{};
    std::vector<level>                     inputs;
    level                                  state{};
    std::vector<std::pair<module*, int>>   destinations;
    void send(auto& queue, level pulse)
    {
        for (auto[d, index]: destinations)
        {
            queue.emplace(index, d, pulse);
        }
    }
    void operator()(auto& queue, int from, level pulse)
    {
        switch (type)
        {
        case '=':
            if (pulse == level::low)
            {
                ++count;
            }
            break;
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
                auto it = std::find(inputs.begin(), inputs.end(), level::low);
                send(queue, it == inputs.end()? level::low: level::high);
            }
            break;
        case 'b':
            send(queue, level::low);
            break;
        }
    }
};

int main()
{
    std::unordered_map<std::string, module> modules;
    std::queue<std::tuple<int, module*, level>> queue;
    std::unordered_map<std::string, int> rx_pulses;
    auto done{[&queue, &modules, &rx_pulses]{
        modules["rx"].type = '=';
        for (int i{}; i != 1000000; ++i)
        {
            queue.emplace(0, &modules["broadcaster"], level::low);
            while (!queue.empty())
            {
                auto[from, to, pulse] = queue.front();
                queue.pop();
                (*to)(queue, from, pulse);
            }
            unsigned done_count{0};
            for (auto& rx: rx_pulses)
            {
                if (0 < modules[rx.first].count)
                {
                    ++done_count;
                    if (rx.second == 0)
                    {
                        rx.second = i + 1;
                    }
                }
            }
            if (done_count == rx_pulses.size())
            {
                std::uint64_t factor{1};
                for (auto const& rx: rx_pulses)
                {
                    factor *= rx.second;
                }
                std::cout << factor << "\n";
                std::exit(0);
            }
        }
        modules = {};
    }};
    int lscount{};
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
        for (auto const& d: destinations)
        {
            if (d == "ls")
            {
                auto ls{std::string("ls") + "0123456789"[lscount]};
                auto rx{std::string("rx") + "0123456789"[lscount]};
                ++lscount;
                modules[ls].type = '&';
                modules[ls].name = ls;
                modules[name].destinations.emplace_back(&modules[ls], modules[ls].inputs.size());
                modules[ls].inputs.push_back(level::low);
                modules[rx].type = '=';
                modules[rx].name = rx;
                rx_pulses[rx] = 0;
                modules[ls].destinations.emplace_back(&modules[rx], 0);
            }
            else
            {
                int index = modules[d].inputs.size();
                modules[d].inputs.push_back(level::low);
                modules[name].destinations.emplace_back(&modules[d], index);
            }
        }
    }
    done();
}
