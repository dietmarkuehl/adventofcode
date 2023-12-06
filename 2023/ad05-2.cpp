#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct range
{
    std::uint64_t start;
    std::uint64_t end;
    bool operator< (range const& other) const { return start < other.start; }
    friend std::istream& operator>> (std::istream& in, range& r)
    {
        std::uint64_t size;
        in >> r.start >> size;
        r.end = r.start + size;
        return in;
    }
    friend std::ostream& operator<< (std::ostream& out, range const& r)
    {
        return out << '[' << r.start << ", " << r.end << ")";
    }
};

auto read_seeds(std::istream& in)
{
    std::string line;
    std::getline(in.ignore(10, ':'), line) >> std::ws;
    std::istringstream sin(line);
    return std::vector<range>{std::istream_iterator<range>(sin), {}};
}

int main(int ac, char* av[])
{
    std::istream  in(std::cin.rdbuf());
    std::ifstream fin;
    if (1 < ac) {
        fin.open(av[1]);
        in.rdbuf(fin.rdbuf());
    }
    auto values = read_seeds(in);
    while (in)
    {
        std::string head;
        std::getline(in, head);
        std::vector<range> tmp;
        std::sort(values.begin(), values.end());
        for (std::string line; std::getline(in, line); )
        {
            std::uint64_t target, source, size;
            if (line.empty())
                break;
            std::istringstream(line) >> target >> source >> size;
            auto offset = [target, source](auto v){ return target + (v - source); };
            values.reserve(2 * values.size());
            for (auto it = values.begin(); it != values.end(); )
            {
                if (source <= it->start && it->start < source + size)
                {
                    // it=             |------|
                    // source= |---------...
                    if (it->end <= source + size)
                    {
                        // it=             |------|
                        // source= |-----------------|
                        tmp.emplace_back(offset(it->start), offset(it->end));
                        it = values.erase(it);
                        continue;
                    }
                    else
                    {
                        // it=             |------|
                        // source= |------------|
                        tmp.emplace_back(offset(it->start), offset(source + size));
                        it->start = source + size;
                    }
                }
                else if (it->start < source && source <= it->end)
                {
                    // it=      |------|
                    // source=     |--...
                    if (it->end <= source + size)
                    {
                        // it=      |------|
                        // source=     |------------|
                        tmp.emplace_back(offset(source), offset(it->end));
                        it->end = source;
                    }
                    else
                    {
                        // it=      |-------|
                        // source=     |--|
                        tmp.emplace_back(offset(source), offset(source + size));
                        values.emplace_back(source + size, it->end);
                        it->end = source;
                    }
                }
                ++it;
            }
        }
        tmp.insert(tmp.end(), values.begin(), values.end());
        values = std::move(tmp);
    }
    std::sort(values.begin(), values.end());
    std::cout << values.front().start << "\n";
}
