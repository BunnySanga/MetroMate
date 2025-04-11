#ifndef DELHI_H
#define DELHI_H

#include <string>
#include <vector>
#include <map>

namespace delhi {

extern std::map<std::string, int> station_map;
extern std::vector<std::string> station_names;
extern std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph; // Added cost to tuple

void init_delhi_graphs();
float calculate_fare(float total_distance, char line);
std::string get_line_name(char line_color);
std::vector<int> dijkstra(int src, int dest, bool use_time);
std::vector<int> bfs_fewest_interchanges(int src, int dest);
void find_delhi_route(const std::string& source, const std::string& dest, int choice);

}  // namespace delhi

#endif