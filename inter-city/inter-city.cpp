#include "inter-city.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <deque>
#include <map>

const int INF = std::numeric_limits<int>::max();
const int MAX_STATION_ID = 10000;

namespace intercity {

std::map<std::string, int> station_map;
std::vector<std::string> station_names;
std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph;

void init_intercity_graphs() {
    std::cout << "Opening inter-city/stations.txt...\n";
    std::ifstream stations_file("inter-city/stations.txt");
    if (!stations_file.is_open()) {
        std::cerr << "Failed to open inter-city/stations.txt\n";
        return;
    }

    std::string line;
    int max_id = -1;
    while (std::getline(stations_file, line)) {
        if (line.empty() || line[0] == '#') {
            std::cout << "Skipping line: " << line << "\n";
            continue;
        }

        std::cout << "Reading line from stations.txt: " << line << "\n";
        std::istringstream iss(line);
        int id;
        std::string name;
        if (!(iss >> id)) {
            std::cerr << "Error: Failed to parse station ID from line: " << line << "\n";
            continue;
        }
        std::getline(iss, name);
        if (name.empty()) {
            std::cerr << "Error: Empty station name for ID " << id << "\n";
            continue;
        }
        if (name.size() > 1) {
            name = name.substr(1);
        } else {
            std::cerr << "Error: Station name too short for ID " << id << "\n";
            continue;
        }
        if (id < 0) {
            std::cerr << "Error: Negative station ID " << id << " for station " << name << "\n";
            continue;
        }
        if (id > MAX_STATION_ID) {
            std::cerr << "Error: Station ID " << id << " exceeds maximum allowed ID " << MAX_STATION_ID << "\n";
            continue;
        }
        station_map[name] = id;
        max_id = std::max(max_id, id);
        std::cout << "Parsed station: ID=" << id << ", Name=" << name << "\n";
        if (id >= station_names.size()) {
            std::cout << "Resizing station_names to " << (id + 1) << "\n";
            station_names.resize(id + 1);
        }
        station_names[id] = name;
    }
    stations_file.close();
    std::cout << "Finished reading inter-city/stations.txt. Max ID: " << max_id << "\n";

    if (max_id < 0) {
        std::cerr << "Error: No valid stations loaded from inter-city/stations.txt\n";
        return;
    }

    std::cout << "Opening inter-city/connections.txt...\n";
    std::ifstream connections_file("inter-city/connections.txt");
    if (!connections_file.is_open()) {
        std::cerr << "Failed to open inter-city/connections.txt\n";
        return;
    }

    int graph_size = max_id + 1;
    std::cout << "Resizing graph to size " << graph_size << "\n";
    graph.resize(graph_size);

    while (std::getline(connections_file, line)) {
        if (line.empty() || line[0] == '#') {
            std::cout << "Skipping line: " << line << "\n";
            continue;
        }

        std::cout << "Reading line from connections.txt: " << line << "\n";
        std::istringstream iss(line);
        int id1, id2, time;
        float distance;
        char line_color;
        int cost;
        if (!(iss >> id1 >> id2 >> distance >> time >> line_color >> cost)) {
            std::cerr << "Error: Failed to parse connection from line: " << line << "\n";
            continue;
        }
        if (id1 < 0 || id1 >= graph_size || id2 < 0 || id2 >= graph_size) {
            std::cerr << "Error: Invalid station ID in connection: id1=" << id1 << ", id2=" << id2 << " (graph size=" << graph_size << ")\n";
            continue;
        }
        std::cout << "Adding edge: " << id1 << " -> " << id2 << "\n";
        graph[id1].emplace_back(id2, time, distance, line_color, cost);
        graph[id2].emplace_back(id1, time, distance, line_color, cost);
    }
    connections_file.close();
    std::cout << "Finished reading inter-city/connections.txt.\n";
}

int calculate_fare(float total_distance, int total_cost) {
    if (total_cost > 0) return total_cost;
    if (total_distance <= 50) return 100;
    else if (total_distance <= 100) return 200;
    else if (total_distance <= 200) return 400;
    else if (total_distance <= 500) return 800;
    else if (total_distance <= 1000) return 1500;
    else return 2000;
}

std::string get_mode_name(char line_color) {
    switch (line_color) {
        case 't': return "Train";
        case 'f': return "Flight";
        case 'm': return "Metro";
        case 'i': return "Interchange";
        default: return "Unknown Mode";
    }
}

std::vector<int> dijkstra(int src, int dest, bool use_time) {
    int n = graph.size();
    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);
    dist[src] = 0;
    auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> pq(cmp);
    pq.emplace(src, 0);
    while (!pq.empty()) {
        int u = pq.top().first;
        int d = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (const auto& edge : graph[u]) {
            int v = std::get<0>(edge);
            int weight = use_time ? std::get<1>(edge) : static_cast<int>(std::get<2>(edge) * 100);
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.emplace(v, dist[v]);
            }
        }
    }
    std::vector<int> path;
    if (dist[dest] == INF) return path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> bfs_fewest_interchanges(int src, int dest) {
    int n = graph.size();
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);
    std::deque<std::tuple<int, char, int>> q;
    q.push_back({src, ' ', 0});
    while (!q.empty()) {
        auto [u, current_mode, interchanges] = q.front();
        q.pop_front();
        if (visited[u]) continue;
        visited[u] = true;
        if (u == dest) {
            std::vector<int> path;
            for (int at = dest; at != -1; at = prev[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (const auto& edge : graph[u]) {
            int v = std::get<0>(edge);
            char mode = std::get<3>(edge);
            int new_interchanges = interchanges;
            if (current_mode != ' ' && mode != current_mode) {
                new_interchanges++;
            }
            if (!visited[v]) {
                prev[v] = u;
                if (mode == current_mode || current_mode == ' ') {
                    q.push_front({v, mode, new_interchanges});
                } else {
                    q.push_back({v, mode, new_interchanges});
                }
            }
        }
    }
    return {};
}

void find_intercity_route(const std::string& source, const std::string& dest, int choice) {
    std::string src_city, src_station, dest_city, dest_station;
    size_t src_underscore = source.find('_');
    size_t dest_underscore = dest.find('_');
    if (src_underscore == std::string::npos || dest_underscore == std::string::npos) {
        std::cout << "Invalid station format. Use City_Station (e.g., Delhi_New Delhi)" << std::endl;
        return;
    }
    src_city = source.substr(0, src_underscore);
    src_station = source.substr(src_underscore + 1);
    dest_city = dest.substr(0, dest_underscore);
    dest_station = dest.substr(dest_underscore + 1);

    std::string full_src = source;
    std::string full_dest = dest;
    if (station_map.find(full_src) == station_map.end() || station_map.find(full_dest) == station_map.end()) {
        std::cout << "Station not found" << std::endl;
        return;
    }
    int src_id = station_map[full_src];
    int dest_id = station_map[full_dest];

    std::vector<int> path;
    if (choice == 1) {
        path = dijkstra(src_id, dest_id, true);
    } else if (choice == 2) {
        path = dijkstra(src_id, dest_id, false);
    } else if (choice == 3) {
        path = bfs_fewest_interchanges(src_id, dest_id);
    } else {
        std::cout << "Invalid choice" << std::endl;
        return;
    }
    if (path.empty()) {
        std::cout << "No path found" << std::endl;
        return;
    }

    int total_time = 0;
    float total_distance = 0.0;
    int total_cost = 0;
    int interchanges = 0;
    char prev_mode = ' ';
    std::vector<std::pair<std::vector<int>, char>> segments;
    std::vector<int> current_segment = {path[0]};
    char current_mode = ' ';

    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        for (const auto& edge : graph[u]) {
            if (std::get<0>(edge) == v) {
                total_time += std::get<1>(edge);
                total_distance += std::get<2>(edge);
                char mode = std::get<3>(edge);
                total_cost += std::get<4>(edge);
                if (i == 0) {
                    current_mode = mode;
                } else if (mode != current_mode) {
                    segments.emplace_back(current_segment, current_mode);
                    current_segment = {u};
                    if (prev_mode != ' ' && mode != prev_mode) {
                        interchanges++;
                    }
                    current_mode = mode;
                }
                current_segment.push_back(v);
                prev_mode = mode;
                break;
            }
        }
    }
    segments.emplace_back(current_segment, current_mode);

    std::cout << "Inter-city route from " << source << " to " << dest << ":\n";
    for (size_t i = 0; i < segments.size(); ++i) {
        const auto& segment = segments[i].first;
        char mode = segments[i].second;
        for (size_t j = 0; j < segment.size(); ++j) {
            std::cout << station_names[segment[j]];
            if (j < segment.size() - 1) std::cout << " -> ";
        }
        std::cout << " (" << get_mode_name(mode) << ")\n";
        if (i < segments.size() - 1) {
            std::cout << "Change from " << get_mode_name(segments[i].second) << " to " 
                      << get_mode_name(segments[i + 1].second) << " at " 
                      << station_names[segment.back()] << "\n";
        }
    }

    int fare = calculate_fare(total_distance, total_cost);
    std::cout << "Total time: " << total_time << " minutes\n";
    std::cout << "Total distance: " << total_distance << " km\n";
    std::cout << "Fare: Rs. " << fare << "\n";
    std::cout << "Interchanges: " << interchanges << "\n";
} // Added closing brace for find_intercity_route

} // Added closing brace for namespace intercity