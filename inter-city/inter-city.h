#ifndef INTER_CITY_H
#define INTER_CITY_H

#include <vector>
#include <map>
#include <string>
#include <tuple>

namespace intercity {

extern std::map<std::string, int> station_map;
extern std::vector<std::string> station_names;
extern std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph;

void init_intercity_graphs();
void find_intercity_route(const std::string& source, const std::string& dest, int choice);

}  // namespace intercity

#endif