#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// Structures
struct Station
{
    string name;
    int x, y;
    int id;
};

struct Route
{
    int id;
    int cost;
    int wait_time;
    int speed;
    vector<int> station_ids;
};

struct Edge
{
    int to_station;
    int route_id; // 0 if alighting/boarding (handled implicitly in state), or actual route for travel
    double weight;
    int action; // 0: Board, 1: Travel, 2: Alight
};

map<string, int> name_to_id;
map<int, string> id_to_name;
vector<Station> stations;
vector<Route> routes;
int station_counter = 0;

int get_station_id(string name, int x, int y)
{
    if (name_to_id.find(name) == name_to_id.end())
    {
        name_to_id[name] = station_counter;
        id_to_name[station_counter] = name;
        stations.push_back({name, x, y, station_counter});
        station_counter++;
    }
    return name_to_id[name];
}

int get_station_id_only(string name)
{
    if (name_to_id.find(name) != name_to_id.end())
    {
        return name_to_id[name];
    }
    return -1;
}

double dist(int u_id, int v_id)
{
    long long dx = stations[u_id].x - stations[v_id].x;
    long long dy = stations[u_id].y - stations[v_id].y;
    return sqrt(dx * dx + dy * dy);
}

// Dijkstra State
struct State
{
    int u;    // Current Station
    int r;    // Current Route (0 = at station, >0 = on bus)
    double d; // Cost/Time

    bool operator>(const State &other) const
    {
        return d > other.d;
    }
};

// Predecessor for path reconstruction
struct Parent
{
    int u;
    int r;
    int action;     // 0: Start, 1: Board, 2: Travel, 3: Alight
    int route_used; // For board/travel
};

void solve(string start_name, string end_name, int type)
{
    int start_node = get_station_id_only(start_name);
    int end_node = get_station_id_only(end_name);

    if (start_node == -1 || end_node == -1)
    {
        cout << "Station not found." << endl;
        return;
    }

    map<int, int> route_id_map;
    int route_count = 0;
    for (const auto &rt : routes)
    {
        route_id_map[rt.id] = ++route_count;
    }

    map<pair<int, int>, double> min_dist;
    map<pair<int, int>, Parent> parent;

    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial State: At start station, not on any bus
    min_dist[{start_node, 0}] = 0;
    pq.push({start_node, 0, 0});
    parent[{start_node, 0}] = {-1, -1, 0, 0};

    double final_cost = -1;

    while (!pq.empty())
    {
        State current = pq.top();
        pq.pop();

        int u = current.u;
        int r = current.r;
        double d = current.d;

        if (min_dist.count({u, r}) && min_dist[{u, r}] < d)
            continue;

        if (u == end_node && r == 0)
        {
            final_cost = d;
            break;
        }

        // Transitions

        // 1. If not on bus (r == 0), can Board any route passing through u
        if (r == 0)
        {
            for (const auto &rt : routes)
            {
                // Check if u is in this route
                bool on_route = false;
                int idx_in_route = -1;
                for (size_t i = 0; i < rt.station_ids.size(); ++i)
                {
                    if (rt.station_ids[i] == u)
                    {
                        on_route = true;
                        idx_in_route = i;
                        break;
                    }
                }

                if (on_route && idx_in_route < rt.station_ids.size() - 1)
                { // Can't board if it's the last station
                    // Cost to board
                    double weight = 0;
                    if (type == 0)
                        weight = rt.cost; // Min Cost
                    else if (type == 1)
                        weight = 0; // Min Time (no wait)
                    else if (type == 2)
                        weight = rt.wait_time; // Min Time (with wait)

                    double new_dist = d + weight;
                    if (!min_dist.count({u, rt.id}) || new_dist < min_dist[{u, rt.id}])
                    {
                        min_dist[{u, rt.id}] = new_dist;
                        pq.push({u, rt.id, new_dist});
                        parent[{u, rt.id}] = {u, 0, 1, rt.id}; // Action 1: Board
                    }
                }
            }
        }
        // 2. If on bus (r != 0), can Travel to next station OR Alight
        else
        {
            // Find the route object
            // Optimization: Could store route pointers/indices
            const Route *rt_ptr = nullptr;
            for (const auto &rt : routes)
            {
                if (rt.id == r)
                {
                    rt_ptr = &rt;
                    break;
                }
            }

            if (rt_ptr)
            {
                // Alight
                // Cost to alight is 0
                if (!min_dist.count({u, 0}) || d < min_dist[{u, 0}])
                {
                    min_dist[{u, 0}] = d;
                    pq.push({u, 0, d});
                    parent[{u, 0}] = {u, r, 3, r}; // Action 3: Alight
                }

                // Travel to next
                int next_station = -1;
                for (size_t i = 0; i < rt_ptr->station_ids.size() - 1; ++i)
                {
                    if (rt_ptr->station_ids[i] == u)
                    {
                        next_station = rt_ptr->station_ids[i + 1];
                        break;
                    }
                }

                if (next_station != -1)
                {
                    double travel_dist = dist(u, next_station);
                    double weight = 0;
                    if (type == 0)
                        weight = 0; // Already paid
                    else
                        weight = travel_dist / rt_ptr->speed; // Time

                    double new_dist = d + weight;
                    if (!min_dist.count({next_station, r}) || new_dist < min_dist[{next_station, r}])
                    {
                        min_dist[{next_station, r}] = new_dist;
                        pq.push({next_station, r, new_dist});
                        parent[{next_station, r}] = {u, r, 2, r}; // Action 2: Travel
                    }
                }
            }
        }
    }

    // Output
    if (final_cost == -1)
    {
        cout << "No path found." << endl;
    }
    else
    {
        // Reconstruct path
        vector<string> path_desc;
        int curr_u = end_node;
        int curr_r = 0;

        // Backtrack
        // We need to collect segments: Route R: S1 -> S2 ...
        // The parent map gives us the previous state.

        struct Event
        {
            int action; // 1: Board, 2: Travel, 3: Alight
            int u;
            int r;
        };

        vector<Event> events;
        while (true)
        {
            if (parent.find({curr_u, curr_r}) == parent.end())
                break;
            Parent p = parent[{curr_u, curr_r}];
            if (p.action == 0)
                break; // Start

            events.push_back({p.action, curr_u, p.route_used}); // Store where we arrived/what we did

            curr_u = p.u;
            curr_r = p.r;
        }
        reverse(events.begin(), events.end());

        // Format output
        // Group by route
        bool first = true;
        int current_route = -1;

        for (const auto &e : events)
        {
            if (e.action == 1)
            { // Board
                if (!first)
                    cout << " 换乘";
                else
                    cout << "线路";
                cout << e.r << " " << id_to_name[e.u]; // Start station of this segment
                current_route = e.r;
                first = false;
            }
            else if (e.action == 2)
            {                                   // Travel
                cout << " " << id_to_name[e.u]; // Arrived station
            }
            else if (e.action == 3)
            {
                // Alight
                // Do nothing, just state change
            }
        }

        cout << " 花费" << final_cost << endl;
    }
}

int main()
{
    string line;
    while (getline(cin, line))
    {
        if (line.empty())
            continue;
        if (line.substr(0, 5) == "QUERY")
        {
            stringstream ss(line);
            string temp, start, end;
            ss >> temp >> start >> end;

            cout << "Query: " << start << " -> " << end << endl;

            cout << "1. Min Cost: ";
            solve(start, end, 0);

            cout << "2. Min Time (No Wait): ";
            solve(start, end, 1);

            cout << "3. Min Time (With Wait): ";
            solve(start, end, 2);

            cout << "--------------------------------------------------" << endl;
            continue;
        }

        // Parse Route
        // r_i s_{i1}(x_{i1} y_{i1}) ... c f v
        stringstream ss(line);
        int id;
        ss >> id;

        vector<string> tokens;
        string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }

        if (tokens.size() < 4)
            continue; // Invalid

        int v = stoi(tokens.back());
        tokens.pop_back();
        int f = stoi(tokens.back());
        tokens.pop_back();
        int c = stoi(tokens.back());
        tokens.pop_back();

        Route rt;
        rt.id = id;
        rt.cost = c;
        rt.wait_time = f;
        rt.speed = v;

        // Parse stations
        // Tokens are now pairs: "Name(x", "y)"
        for (size_t i = 0; i < tokens.size(); i += 2)
        {
            string part1 = tokens[i];     // Name(x
            string part2 = tokens[i + 1]; // y)

            size_t open_paren = part1.find('(');
            string name = part1.substr(0, open_paren);
            string x_str = part1.substr(open_paren + 1);
            string y_str = part2.substr(0, part2.size() - 1);

            int x = stoi(x_str);
            int y = stoi(y_str);

            int s_id = get_station_id(name, x, y);
            rt.station_ids.push_back(s_id);
        }
        routes.push_back(rt);
    }
    return 0;
}
