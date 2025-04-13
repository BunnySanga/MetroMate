// #ifndef INTER_CITY_H
// #define INTER_CITY_H

// #include <vector>
// #include <map>
// #include <string>
// #include <tuple>

// namespace intercity {

// extern std::map<std::string, int> station_map;
// extern std::vector<std::string> station_names;
// extern std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph;

// void init_intercity_graphs();
// void find_intercity_route(const std::string& source, const std::string& dest, int choice);

// }  // namespace intercity

// #endif

#ifndef INTERCITY_H
#define INTERCITY_H

#include <string>
#include <vector>
#include <map>

namespace intercity {

extern std::map<std::string, int> station_map;
extern std::vector<std::string> station_names;
extern std::vector<std::vector<std::tuple<int, int, float, char>>> graph; // (dest_id, time, distance, line_color)

void init_intercity_graphs();
float calculate_fare(float total_distance); // Based on distance only
std::string get_line_name(char line_color);
std::vector<int> dijkstra(int src, int dest, bool use_time);
std::vector<int> bfs_fewest_interchanges(int src, int dest);
void find_intercity_route(const std::string& source, const std::string& dest, int choice);

}  // namespace intercity

#endif