#ifndef _LATLONG_H
#define _LATLONG_H

class LatLong {
    double lat_;
    double lon_;
public:
    LatLong(double lat, double lon) :
        lat_(lat), lon_(lon){}
    double getLat();
    double getLon();
    LatLong add(double lat, double lon);
};
#endif
