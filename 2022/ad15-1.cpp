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
     while (std::cin >> ignore >> sensor.x >> ignore >> sensor.y >> ignore >> beacon.x >> ignore >> beacon.y) {
         sensors.push_back(sensor);
         beacons.push_back(beacon);
         maxx = std::max(sensor.x, maxx);
         maxx = std::max(beacon.x, maxx);
         minx = std::min(sensor.x, minx);
         minx = std::min(beacon.x, minx);
         std::cout << "sensor=" << sensor <<" beacon=" << beacon << " "
                   << "distance=" << distance(sensor, beacon) << "\n";
         long dist = distance(sensor, beacon);
         long off = dist - sensor.y;
         long mid = sensor.x + 1;
         minx = std::min(mid - off + 1, minx);
     }
     //std::cout << "minx=" << minx << ", maxx=" << maxx << "\n";
     auto width = maxx - minx;
     std::string line(2 * width, '.');
     for (long target(0); target <= 4000000; ++target) {
         if (target % 100 == 0) { std::cout << target << "\n" << std::flush; }
         std::fill(line.begin(), line.end(), '.');
         for (std::size_t i{0}; i != sensors.size(); ++i) {
             if (true || sensors[i] == point(8, 7)) {
                 long dist = distance(sensors[i], beacons[i]);
                 long off = dist - std::abs(sensors[i].y - target);
                 long mid = sensors[i].x - minx + 1;
                 if (std::abs(sensors[i].y - target) < dist) {
                     std::fill(line.begin() + (mid - off + 1), line.begin() + (mid + 2 + off), '#');
                  }
             }
         }
         for (std::size_t i{0}; i != sensors.size(); ++i) {
              if (beacons[i].y == target) {
                  line[beacons[i].x - minx + 2] = 'B';
              }
         }
         //std::cout << "   " << line << "\n";
         auto start = std::find(line.begin(), line.end(), '#');
         auto cand = std::find(start, line.end(), '.');
         if (std::find(cand, line.end(), '#') != line.end()) {
             std::cout << minx << " " << (cand - line.begin() + minx - 2) << ", " << target << "\n";
             std::cout << ((cand - line.begin() + minx - 2) * 4000000 + target) << "\n";
             break;
         }
     }
     //std::cout << std::count(line.begin(), line.end(), '#') << "\n";
    
}
