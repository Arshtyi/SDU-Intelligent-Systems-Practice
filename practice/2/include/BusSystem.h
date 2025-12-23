#pragma once
#include <string>
#include <vector>
#include <map>
#include <cmath>

struct Stop
{
    std::string name;
    int x, y;
    int id;
};

struct Route
{
    int id;
    std::vector<int> stopIds;
    int cost;
    int frequency;
    int speed;
};

struct PathStep
{
    std::string stopName;
    int routeId; // The route used to arrive at this stop
    double accumulatedValue;
};

class BusSystem
{
public:
    void loadData(const std::string &filename);

    enum Criteria
    {
        MinCost,
        MinTimeNoWait,
        MinTimeWithWait
    };

    struct PathResult
    {
        std::vector<PathStep> steps;
        double totalValue;
        bool found;
    };

    PathResult findPath(const std::string &startStop, const std::string &endStop, Criteria criteria);

    const std::vector<Stop> &getStops() const { return stops; }
    const std::vector<Route> &getRoutes() const { return routes; }
    const std::map<std::string, int> &getStopNameMap() const { return stopNameToId; }

    // Helper to get stops for a specific route
    const std::vector<int> &getStopsForRoute(int routeId) const;

private:
    std::vector<Stop> stops;
    std::vector<Route> routes;
    std::map<std::string, int> stopNameToId;

    // Map from StopID to list of RouteIDs passing through it
    std::vector<std::vector<int>> stopToRoutes;

    double calculateDistance(int stopId1, int stopId2);
};
