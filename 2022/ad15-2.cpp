#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::istream& ignore(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), '=');
}

struct point {
   long x{}, y{};
   point() = default;
   point(long x, long y): x(x), y(y) {}
   friend std::ostream& operator<< (std::ostream& out, point const& p) {
      return out << "(" << p.x << ", " << p.y << ")";
   }
   bool operator== (point const&) const = default;
};

struct segment {
    long min{}, max{};
    segment() = default;
    segment(long min, long max): min(min), max(max) {}
    bool operator< (segment const& other) const { return this->min < other.min; }
    friend std::ostream& operator<< (std::ostream& out, segment const& s) {
        return out << "(" << s.min << ", " << s.max << ")";
    }
};

long distance(point const& a, point const& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main() {
     long maxx = std::numeric_limits<long>::min();
     long minx = std::numeric_limits<long>::max();

     std::vector<point> sensors;
     std::vector<point> beacons;

     point sensor;
     point beacon;
     long max_target{};
     std::cin >> max_target;
     while (std::cin >> ignore >> sensor.x >> ignore >> sensor.y >> ignore >> beacon.x >> ignore >> beacon.y) {
         sensors.push_back(sensor);
         beacons.push_back(beacon);
         maxx = std::max(sensor.x, maxx);
         maxx = std::max(beacon.x, maxx);
         minx = std::min(sensor.x, minx);
         minx = std::min(beacon.x, minx);
         long dist = distance(sensor, beacon);
         long off = dist - sensor.y;
         long mid = sensor.x + 1;
         minx = std::min(mid - off + 1, minx);
     }
     auto width = maxx - minx;
     for (long target(0); target <= max_target; ++target) {
         //if (target % 1000 == 0) { std::cout << "target=" << target << "\n" << std::flush; }
         std::vector<segment> segments;
         for (std::size_t i{0}; i != sensors.size(); ++i) {
             if (true || sensors[i] == point(8, 7)) {
                 long dist = distance(sensors[i], beacons[i]);
                 long off = dist - std::abs(sensors[i].y - target);
                 long mid = sensors[i].x - 1;
                 if (std::abs(sensors[i].y - target) < dist) {
                     segments.emplace_back(mid - off + 1, mid + 2 + off);
                  }
             }
         }
         std::sort(segments.begin(), segments.end());
         if (2u < segments.size()) {
             for (std::size_t i{}, j{}; i != segments.size(); i = j) {
                 j = i + 1;
                 while (j != segments.size() && segments[j].max < segments[i].max) {
                    ++j;
                 }
                 if  (segments[i].max < segments[j].min) {
                     std::cout << (4000000 * segments[i].max + target) << "\n";
                     break;
                 }
             }
         }
     }
    
}
