#include "delhi.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <deque>
#include <cctype>

const int INF = std::numeric_limits<int>::max();
const int MAX_STATION_ID = 10000;

namespace delhi {

std::map<std::string, int> station_map;
std::vector<std::string> station_names;
std::vector<std::vector<std::tuple<int, int, float, char, int>>> graph;

// Trim leading and trailing whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

void init_delhi_graphs() {
    std::cout << "Opening delhi/stations.txt...\n";
    std::ifstream stations_file("delhi/stations.txt");
    if (!stations_file.is_open()) {
        std::cerr << "Failed to open delhi/stations.txt\n";
        return;
    }

    std::string line;
    int max_id = -1;
    while (std::getline(stations_file, line)) {
        if (line.empty() || line[0] == '#') {
            std::cout << "Skipping line: " << line << "\n";
            continue;
        }
        std::istringstream iss(line);
        int id;
        std::string name;
        if (!(iss >> id)) {
            std::cerr << "Error: Failed to parse station ID from line: " << line << "\n";
            continue;
        }
        // Read station name until '#' or end of line
        std::string token;
        while (iss >> token && token[0] != '#') {
            if (!name.empty()) name += " "; // Add space between words
            name += token;
        }
        name = trim(name);
        if (name.empty() || id < 0 || id > MAX_STATION_ID) {
            std::cerr << "Error: Invalid station data for ID " << id << "\n";
            continue;
        }
        station_map[name] = id;
        max_id = std::max(max_id, id);
        if (id >= station_names.size()) station_names.resize(id + 1);
        station_names[id] = name;
        std::cout << "Parsed station: ID=" << id << ", Name=" << name << "\n";
    }
    stations_file.close();
    std::cout << "Finished reading delhi/stations.txt. Max ID: " << max_id << "\n";

    if (max_id < 0) {
        std::cerr << "Error: No valid stations loaded\n";
        return;
    }

    std::cout << "Opening delhi/connections.txt...\n";
    std::ifstream connections_file("delhi/connections.txt");
    if (!connections_file.is_open()) {
        std::cerr << "Failed to open delhi/connections.txt\n";
        return;
    }

    int graph_size = max_id + 1;
    graph.resize(graph_size);

    while (std::getline(connections_file, line)) {
        if (line.empty() || line[0] == '#') {
            std::cout << "Skipping line: " << line << "\n";
            continue;
        }
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
            std::cerr << "Error: Invalid station ID in connection: " << id1 << " -> " << id2 << "\n";
            continue;
        }
        graph[id1].emplace_back(id2, time, distance, line_color, cost);
        graph[id2].emplace_back(id1, time, distance, line_color, cost);
        std::cout << "Added edge: " << id1 << " (" << station_names[id1] << ") -> " << id2 << " (" << station_names[id2] << ") "
                  << "time=" << time << ", dist=" << distance << ", color=" << line_color << "\n";
    }
    connections_file.close();
    std::cout << "Finished reading delhi/connections.txt\n";
}

int calculate_fare(float total_distance, int total_cost) {
    if (total_cost > 0) return total_cost;
    if (total_distance <= 2) return 10;
    else if (total_distance <= 5) return 20;
    else if (total_distance <= 10) return 30;
    else if (total_distance <= 15) return 40;
    else if (total_distance <= 22) return 50;
    else return 60;
}

std::string get_line_name(char line_color) {
    switch (line_color) {
        case 'r': return "Red Line";
        case 'y': return "Yellow Line";
        case 'b': return "Blue Line 1"; // Line 3
        case 'B': return "Blue Line 2"; // Line 4
        case 'g': return "Green Line";
        case 'v': return "Violet Line";
        case 'p': return "Pink Line";
        case 'm': return "Magenta Line";
        case 'G': return "Grey Line";
        case 'a': return "Airport Express";
        case 'R': return "Rapid Metro Gurgaon";
        case 'i': return "Interchange";
        default: return "Unknown Line";
    }
}

std::vector<int> dijkstra(int src, int dest, bool use_time) {
    int n = graph.size();
    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);
    dist[src] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.emplace(0, src);
    std::cout << "Starting Dijkstra from " << station_names[src] << " (ID " << src << ") to " << station_names[dest] << " (ID " << dest << ")\n";
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        std::cout << "Processing " << station_names[u] << " (ID " << u << ") with distance " << d << "\n";
        for (const auto& edge : graph[u]) {
            int v = std::get<0>(edge);
            int weight = use_time ? std::get<1>(edge) : static_cast<int>(std::get<2>(edge) * 100);
            int new_dist = dist[u] + weight;
            std::cout << "  Checking edge to " << station_names[v] << " (ID " << v << "), weight=" << weight << ", new_dist=" << new_dist << "\n";
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                prev[v] = u;
                pq.emplace(dist[v], v);
                std::cout << "  Updated " << station_names[v] << " (ID " << v << ") to distance " << dist[v] << "\n";
            }
        }
    }
    std::vector<int> path;
    if (dist[dest] == INF) {
        std::cout << "No path found\n";
        return path;
    }
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    std::cout << "Computed path: ";
    for (int id : path) std::cout << station_names[id] << " (ID " << id << ") -> ";
    std::cout << "END\n";
    return path;
}

std::vector<int> bfs_fewest_interchanges(int src, int dest) {
    int n = graph.size();
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);
    std::deque<std::pair<int, char>> q;
    q.push_back({src, ' '});
    while (!q.empty()) {
        int u = q.front().first;
        char current_line = q.front().second;
        q.pop_front();
        if (visited[u]) continue;
        visited[u] = true;
        if (u == dest) {
            std::vector<int> path;
            for (int at = dest; at != -1; at = prev[at]) path.push_back(at);
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (const auto& edge : graph[u]) {
            int v = std::get<0>(edge);
            char line = std::get<3>(edge);
            if (!visited[v]) {
                prev[v] = u;
                if (line == current_line || current_line == ' ') {
                    q.push_front({v, line});
                } else {
                    q.push_back({v, line});
                }
            }
        }
    }
    return {};
}

void find_delhi_route(const std::string& source, const std::string& dest, int choice) {
    if (station_map.find(source) == station_map.end() || station_map.find(dest) == station_map.end()) {
        std::cout << "Station not found\n";
        return;
    }
    int src_id = station_map[source];
    int dest_id = station_map[dest];
    std::vector<int> path;
    if (choice == 1) path = dijkstra(src_id, dest_id, true);
    else if (choice == 2) path = dijkstra(src_id, dest_id, false);
    else if (choice == 3) path = bfs_fewest_interchanges(src_id, dest_id);
    else {
        std::cout << "Invalid choice\n";
        return;
    }
    if (path.empty()) {
        std::cout << "No path found\n";
        return;
    }

    int total_time = 0;
    float total_distance = 0.0;
    int total_cost = 0;
    std::vector<char> line_sequence;
    std::vector<std::pair<std::vector<int>, char>> segments;
    std::vector<int> current_segment = {path[0]};
    char current_line = ' ';

    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        char line_color = ' ';
        for (const auto& edge : graph[u]) {
            if (std::get<0>(edge) == v) {
                line_color = std::get<3>(edge);
                total_time += std::get<1>(edge);
                total_distance += std::get<2>(edge);
                total_cost += std::get<4>(edge);
                break;
            }
        }
        if (i == 0) current_line = line_color;
        if (line_color != current_line) {
            segments.emplace_back(current_segment, current_line);
            current_segment = {u};
            current_line = line_color;
        }
        current_segment.push_back(v);
        if (line_color != 'i') line_sequence.push_back(line_color);
    }
    segments.emplace_back(current_segment, current_line);

    int interchanges = 0;
    for (size_t i = 1; i < line_sequence.size(); ++i) {
        if (line_sequence[i] != line_sequence[i - 1] && line_sequence[i] != 'i' && line_sequence[i - 1] != 'i') {
            interchanges++;
        }
    }

    std::cout << "Route from " << source << " to " << dest << ":\n";
    for (size_t i = 0; i < segments.size(); ++i) {
        const auto& segment = segments[i].first;
        char line = segments[i].second;
        if (line == 'i') continue;
        for (size_t j = 0; j < segment.size(); ++j) {
            std::cout << station_names[segment[j]];
            if (j < segment.size() - 1) std::cout << " -> ";
        }
        std::cout << " (" << get_line_name(line) << ")\n";
        if (i < segments.size() - 1 && segments[i + 1].second != 'i' && segments[i].second != segments[i + 1].second) {
            std::cout << "Change from " << get_line_name(segments[i].second) << " to " << get_line_name(segments[i + 1].second) << " at " << station_names[segment.back()] << "\n";
        }
    }

    int fare = calculate_fare(total_distance, total_cost);
    std::cout << "Total time: " << total_time << " minutes\n";
    std::cout << "Total distance: " << total_distance << " km\n";
    std::cout << "Fare: Rs. " << fare << "\n";
    std::cout << "Interchanges: " << interchanges << "\n";
}

} // namespace delhi