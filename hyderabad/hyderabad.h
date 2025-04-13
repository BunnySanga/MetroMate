// #ifndef HYDERABAD_H
// #define HYDERABAD_H

// #include <string>
// #include <vector>
// #include <map>

// namespace hyderabad {

// extern std::map<std::string, int> station_map;
// extern std::vector<std::string> station_names;
// extern std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph; // Added cost to tuple

// void init_hyderabad_graphs();
// float calculate_fare(float total_distance, char line);
// std::string get_line_name(char line_color);
// std::vector<int> dijkstra(int src, int dest, bool use_time);
// std::vector<int> bfs_fewest_interchanges(int src, int dest);
// void find_hyderabad_route(const std::string& source, const std::string& dest, int choice);

// }  // namespace hyderabad

// #endif

#ifndef HYDERABAD_H
#define HYDERABAD_H

#include <string>
#include <vector>
#include <map>

namespace hyderabad {

extern std::map<std::string, int> station_map;
extern std::vector<std::string> station_names;
extern std::vector<std::vector<std::tuple<int, int, float, char>>> graph; // (dest_id, time, distance, line_color)

void init_hyderabad_graphs();
float calculate_fare(float total_distance); // Based on distance only
std::string get_line_name(char line_color);
std::vector<int> dijkstra(int src, int dest, bool use_time);
std::vector<int> bfs_fewest_interchanges(int src, int dest);
void find_hyderabad_route(const std::string& source, const std::string& dest, int choice);

}  // namespace hyderabad

#endif