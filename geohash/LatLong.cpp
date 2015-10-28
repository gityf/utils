#include "LatLong.h"

double LatLong::getLat() { return lat_; }
double LatLong::getLon() { return lon_; }
LatLong LatLong::add(double lat, double lon) {
    return LatLong(lat_ + lat, lon_ + lon);
}
