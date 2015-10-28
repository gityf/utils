#include "Distance.h"


double Distance::distance(double lat1, double lng1, double lat2, double lng2) {
    double dLat = (lat2 - lat1) * TO_RADIANS;
    double dLng = (lng2 - lng1) * TO_RADIANS;
    double aLat = sin(dLat / 2.0);
    double aLng = sin(dLng / 2.0);
    aLat *= aLat;
    aLng *= aLng;
    double a = aLat + cos(lat1 * TO_RADIANS) * cos(lat2 * TO_RADIANS) * aLng;
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return c * EARTH_RADIUS;
}

/*
double Distance::distance(double lat1, double lng1, double lat2, double lng2) {
    double radLat1 = lat1 * TO_RADIANS;
    double radLat2 = lat2 * TO_RADIANS;
    double a = radLat1 - radLat2;
    double b = (lng1 - lng2) * TO_RADIANS;

    double s = 2
        * asin(
        sqrt(
        pow(sin(a / 2), 2)
        + cos(radLat1) * cos(radLat2)
        * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS;

    return s;
}
//*/

double Distance::distance(const std::string& hash1, const std::string& hash2) {
    LatLong latlong1 = GeoHash::decodeHash(hash1);
    LatLong latlong2 = GeoHash::decodeHash(hash2);
    return distance(latlong1.getLat(), latlong1.getLon(), latlong2.getLat(), latlong2.getLon());
}

