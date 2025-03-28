#ifndef INTER_CITY_H
#define INTER_CITY_H

#include <vector>
#include <map>
#include <string>
#include <tuple>

namespace intercity {

extern std::map<std::string, int> station_map;  // Maps station names (e.g., Delhi_New Delhi) to IDs
extern std::vector<std::string> station_names;  // Maps IDs to station names
extern std::vector<std::vector<std::tuple<int, int, float, char>>> graph;  // Graph: neighbor_id, time, distance, line_color

void init_intercity_graphs();  // Loads data from files
void find_intercity_route(const std::string& source, const std::string& dest, int choice);  // Finds route based on choice (1: time, 2: cost, 3: interchanges)

}  // namespace intercity

#endif  // INTER_CITY_H