#include "bangalore.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <deque>

const int INF = std::numeric_limits<int>::max();  // Infinity for Dijkstra's

namespace bangalore {

std::map<std::string, int> station_map;
std::vector<std::string> station_names;
std::vector<std::vector<std::tuple<int, int, float, char>>> graph;

void init_bangalore_graphs() {
    std::ifstream stations_file("bangalore/stations.txt");
    if (!stations_file.is_open()) {
        std::cerr << "Error opening bangalore/stations.txt" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(stations_file, line)) {
        std::istringstream iss(line);
        int id;
        std::string name;
        iss >> id;
        std::getline(iss, name);
        name = name.substr(1);  // Remove leading space
        station_map[name] = id;
        if (id >= station_names.size()) {
            station_names.resize(id + 1);
        }
        station_names[id] = name;
    }
    stations_file.close();

    std::ifstream connections_file("bangalore/connections.txt");
    if (!connections_file.is_open()) {
        std::cerr << "Error opening bangalore/connections.txt" << std::endl;
        return;
    }
    int max_id = station_names.size();
    graph.resize(max_id);
    while (std::getline(connections_file, line)) {
        std::istringstream iss(line);
        int id1, id2, time;
        float distance;
        char line_color;
        iss >> id1 >> id2 >> distance >> time >> line_color;
        graph[id1].emplace_back(id2, time, distance, line_color);
        graph[id2].emplace_back(id1, time, distance, line_color);  // Bidirectional
    }
    connections_file.close();
}

int calculate_fare(float total_distance) {
    // Bangalore Metro fare slabs (approximate, adjust as per official rates)
    if (total_distance <= 2) return 10;
    else if (total_distance <= 4) return 15;
    else if (total_distance <= 6) return 20;
    else if (total_distance <= 9) return 25;
    else if (total_distance <= 12) return 30;
    else if (total_distance <= 15) return 35;
    else if (total_distance <= 18) return 40;
    else if (total_distance <= 22) return 45;
    else if (total_distance <= 27) return 50;
    else return 60;  // Max fare for longer distances
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
    std::deque<std::tuple<int, char, int>> q;  // station, line, interchanges
    q.push_back({src, ' ', 0});
    while (!q.empty()) {
        auto [u, current_line, interchanges] = q.front();
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
            char line = std::get<3>(edge);
            int new_interchanges = interchanges;
            if (current_line != ' ' && line != current_line) {
                new_interchanges++;
            }
            if (!visited[v]) {
                prev[v] = u;
                if (line == current_line || current_line == ' ') {
                    q.push_front({v, line, new_interchanges});
                } else {
                    q.push_back({v, line, new_interchanges});
                }
            }
        }
    }
    return {};
}

void find_bangalore_route(const std::string& source, const std::string& dest, int choice) {
    if (station_map.find(source) == station_map.end() || station_map.find(dest) == station_map.end()) {
        std::cout << "Station not found" << std::endl;
        return;
    }
    int src_id = station_map[source];
    int dest_id = station_map[dest];
    std::vector<int> path;
    if (choice == 1) {  // Shortest time
        path = dijkstra(src_id, dest_id, true);
    } else if (choice == 2) {  // Minimum cost
        path = dijkstra(src_id, dest_id, false);
    } else if (choice == 3) {  // Fewest interchanges
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
    int interchanges = 0;
    char prev_line = ' ';
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        for (const auto& edge : graph[u]) {
            if (std::get<0>(edge) == v) {
                total_time += std::get<1>(edge);
                total_distance += std::get<2>(edge);
                char current_line = std::get<3>(edge);
                if (i > 0 && current_line != prev_line) {
                    interchanges++;
                }
                prev_line = current_line;
                break;
            }
        }
    }
    int fare = calculate_fare(total_distance);
    std::cout << "Route from " << source << " to " << dest << ":" << std::endl;
    for (int id : path) {
        std::cout << station_names[id] << " ";
    }
    std::cout << std::endl;
    std::cout << "Total time: " << total_time << " minutes" << std::endl;
    std::cout << "Total distance: " << total_distance << " km" << std::endl;
    std::cout << "Fare: Rs. " << fare << std::endl;
    std::cout << "Interchanges: " << interchanges << std::endl;
}

}  // namespace bangalore