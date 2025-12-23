#include "BusSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>
#include <cmath>

void BusSystem::loadData(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);

        int routeId;
        ss >> routeId;

        Route route;
        route.id = routeId;

        std::string token;
        while (ss >> token)
        {
            if (token.empty())
                continue;
            if (token[0] == 'S' && token.length() > 1 && std::isdigit(token[1]))
            {
                // It's a stop
                std::string stopName = token;
                int x, y;
                ss >> x >> y;

                int stopId;
                if (stopNameToId.find(stopName) == stopNameToId.end())
                {
                    stopId = stops.size();
                    stops.push_back({stopName, x, y, stopId});
                    stopNameToId[stopName] = stopId;
                    stopToRoutes.resize(stops.size());
                }
                else
                {
                    stopId = stopNameToId[stopName];
                }

                route.stopIds.push_back(stopId);
                // We will add to stopToRoutes after we push the route to vector to get correct index
            }
            else
            {
                // It's the cost
                try
                {
                    route.cost = std::stoi(token);
                    ss >> route.frequency >> route.speed;
                }
                catch (...)
                {
                    // Handle parsing error if any
                }
                break;
            }
        }

        int routeIdx = routes.size();
        routes.push_back(route);

        // Update stopToRoutes
        for (int stopId : route.stopIds)
        {
            stopToRoutes[stopId].push_back(routeIdx);
        }
    }
}

double BusSystem::calculateDistance(int stopId1, int stopId2)
{
    const Stop &s1 = stops[stopId1];
    const Stop &s2 = stops[stopId2];
    return std::sqrt(std::pow(s1.x - s2.x, 2) + std::pow(s1.y - s2.y, 2));
}

const std::vector<int> &BusSystem::getStopsForRoute(int routeId) const
{
    // routeId is the internal index in routes vector
    if (routeId >= 0 && routeId < routes.size())
    {
        return routes[routeId].stopIds;
    }
    static std::vector<int> empty;
    return empty;
}

BusSystem::PathResult BusSystem::findPath(const std::string &startStop, const std::string &endStop, Criteria criteria)
{
    if (stopNameToId.find(startStop) == stopNameToId.end() || stopNameToId.find(endStop) == stopNameToId.end())
    {
        return {{}, 0.0, false};
    }

    int startNode = stopNameToId[startStop];
    int endNode = stopNameToId[endStop];

    int numRoutes = routes.size();
    int numStops = stops.size();

    // dist[stop][route_idx + 1]
    // route_idx: -1 (start) -> 0, 0 -> 1, ...
    std::vector<std::vector<double>> dist(numStops, std::vector<double>(numRoutes + 1, std::numeric_limits<double>::max()));

    struct State
    {
        double cost;
        int u;
        int lastRouteIdx; // -1 for start

        bool operator>(const State &other) const
        {
            return cost > other.cost;
        }
    };

    struct Parent
    {
        int u;
        int routeIdx;
    };

    std::vector<std::vector<Parent>> parent(numStops, std::vector<Parent>(numRoutes + 1, {-1, -1}));

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    // Initial state
    dist[startNode][0] = 0;
    pq.push({0, startNode, -1});

    double finalCost = std::numeric_limits<double>::max();
    int finalRouteIdx = -1;

    while (!pq.empty())
    {
        State current = pq.top();
        pq.pop();

        double d = current.cost;
        int u = current.u;
        int lastR = current.lastRouteIdx;

        if (d > dist[u][lastR + 1])
            continue;
        if (u == endNode)
        {
            if (d < finalCost)
            {
                finalCost = d;
                finalRouteIdx = lastR;
            }
            break;
        }

        for (int rIdx : stopToRoutes[u])
        {
            const Route &route = routes[rIdx];

            // Find u in route.stopIds
            // Since a stop can appear multiple times in a route (loops), we need to handle that.
            // But usually simple routes.
            std::vector<int> indices;
            for (int i = 0; i < route.stopIds.size(); ++i)
            {
                if (route.stopIds[i] == u)
                    indices.push_back(i);
            }

            for (int idxInRoute : indices)
            {
                // Try moving to next and prev
                int neighbors[] = {idxInRoute - 1, idxInRoute + 1};
                for (int nextIdx : neighbors)
                {
                    if (nextIdx >= 0 && nextIdx < route.stopIds.size())
                    {
                        int v = route.stopIds[nextIdx];

                        double weight = 0;

                        // Boarding / Switching Cost
                        if (rIdx != lastR)
                        {
                            if (criteria == MinCost)
                            {
                                weight += route.cost;
                            }
                            else if (criteria == MinTimeWithWait)
                            {
                                weight += route.frequency; // Conservative wait time
                            }
                            // MinTimeNoWait: 0 switching cost
                        }

                        // Travel Cost
                        double distVal = calculateDistance(u, v);
                        if (criteria == MinCost)
                        {
                            // Travel cost is 0 (covered by ticket)
                        }
                        else
                        {
                            // Time = Dist / Speed
                            weight += distVal / route.speed;
                        }

                        if (dist[u][lastR + 1] + weight < dist[v][rIdx + 1])
                        {
                            dist[v][rIdx + 1] = dist[u][lastR + 1] + weight;
                            parent[v][rIdx + 1] = {u, lastR};
                            pq.push({dist[v][rIdx + 1], v, rIdx});
                        }
                    }
                }
            }
        }
    }

    if (finalCost == std::numeric_limits<double>::max())
    {
        return {{}, 0.0, false};
    }

    // Reconstruct path
    std::vector<PathStep> steps;
    int currU = endNode;
    int currR = finalRouteIdx;

    while (currU != -1)
    {
        // We push to front.
        steps.push_back({stops[currU].name, (currR == -1 ? -1 : routes[currR].id), 0.0}); // Value not needed for display per step?

        Parent p = parent[currU][currR + 1];
        currU = p.u;
        currR = p.routeIdx;
    }
    std::reverse(steps.begin(), steps.end());

    return {steps, finalCost, true};
}
