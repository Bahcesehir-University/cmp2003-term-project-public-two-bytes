#include "analyzer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
// Students may use ANY data structure internally
static std::unordered_map<std::string, long long> zoneCounts;
static std::unordered_map<
    std::string,
    std::unordered_map<int, long long>
> slotCounts;

void TripAnalyzer::ingestFile(const std::string& csvPath) {
     zoneCounts.clear();
     slotCounts.clear();
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        return;
    }

    std::string line;

   if( !std::getline(file, line)) return;

    while (std::getline(file, line)) {
       if (line.empty()) continue;

        std::stringstream ss(line);
        std::string tripId, zoneId,drop,time,distance,fare;

        if (!std::getline(ss, tripId, ',')) continue;
        if (!std::getline(ss, zoneId, ',')) continue;
        if (!std::getline(ss, drop, ',')) continue;
        if (!std::getline(ss, time, ',')) continue;
        if (!std::getline(ss, distance, ',')) continue;
        if (!std::getline(ss, fare)) continue;

        if (zoneId.empty()) continue;

        if (time.size() < 13) continue;


        std::string hourStr = time.substr(11, 2);
        if (!std::isdigit(hourStr[0]) || !std::isdigit(hourStr[1])) continue;




        try {
         int hour = std::stoi(hourStr);
        if (hour < 0 || hour > 23) continue;

        zoneCounts[zoneId]++;
        slotCounts[zoneId][hour]++;
        } catch (const std::exception&) {
            continue;
        }


    }
    // TODO:
    // - open file
    // - skip header
    // - skip malformed rows
    // - extract PickupZoneID and pickup hour
    // - aggregate counts
}

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    std::vector<ZoneCount> result;

    for (const auto& p : zoneCounts) {
        ZoneCount z;
        z.zone = p.first;
        z.count = p.second;
        result.push_back(z);
    }
    std::sort(result.begin(), result.end(),
        [](const ZoneCount& a, const ZoneCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            return a.zone < b.zone;
        }
    );
       if (result.size() > k) {
        result.resize(k);
    }


    return result;

    // TODO:
    // - sort by count desc, zone asc
    // - return first k

}

std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
     std::vector<SlotCount> result;

    for (const auto& zonePair : slotCounts) {
        const std::string& zone = zonePair.first;
        const auto& hourMap = zonePair.second;

        for (const auto& hourPair : hourMap) {
            SlotCount s;
            s.zone = zone;
            s.hour = hourPair.first;
            s.count = hourPair.second;
            result.push_back(s);
        }
    }
    std::sort(result.begin(), result.end(),
        [](const SlotCount& a, const SlotCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            if (a.zone != b.zone)
                return a.zone < b.zone;
            return a.hour < b.hour;
        }
    );

    if (static_cast<int>(result.size()) > k)
       result.resize(k);

    return result;
}

    // TODO:
    // - sort by count desc, zone asc, hour asc
    // - return first k
