#ifndef _DISTANCE_H
#define _DISTANCE_H

#include <math.h>
#include <string>
#include "LatLong.h"
#include "GeoHash.h"

class Distance {
    const static double EARTH_RADIUS = 6371.009;
    const static double TO_RADIANS = 3.1415926 / 180.0;

public:
    static double distance(double lat1, double lng1, double lat2, double lng2);
    static double distance(const std::string& hash1, const std::string& hash2);
};
#endif
