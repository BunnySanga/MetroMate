#ifndef DELHI_H
#define DELHI_H

#include <vector>
#include <map>
#include <string>
#include <tuple>

namespace delhi {

extern std::map<std::string, int> station_map;  // Maps station names to IDs
extern std::vector<std::string> station_names;  // Maps IDs to station names
extern std::vector<std::vector<std::tuple<int, int, float, char>>> graph;  // Graph: neighbor_id, time, distance, line_color

void init_delhi_graphs();  // Loads data from files
void find_delhi_route(const std::string& source, const std::string& dest, int choice);  // Finds route based on choice (1: time, 2: cost, 3: interchanges)

}  // namespace delhi

#endif  // DELHI_H