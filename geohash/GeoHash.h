#ifndef _GEO_HASH_H
#define _GEO_HASH_H

#include <math.h>
#include <cctype>
#include <string>
#include <map>
#include <vector>
#include <utility>  

#include "LatLong.h"
#include "Distance.h"

class GeoHash {
public:
    static const int MAX_HASH_LENGTH = 12;
    static const int DEFAULT_MAX_HASHES = 12;

    static double PRECISION;
    static int BITS[5];
    static std::string BASE32;
    static std::map<std::string, std::map<std::string, std::string> > NEIGHBOURS;
    static std::map<std::string, std::map<std::string, std::string> > BORDERS;

    //static boost::mutex cacheMutex;
    static double hashWidthCache[MAX_HASH_LENGTH];


    static std::map<std::string, std::map<std::string, std::string> > createBorders();

    static std::map<std::string, std::map<std::string, std::string> > createNeighbours();

    static std::string adjacentHashAtBorder(const std::string& hash, const std::string& direction);

    static void refineInterval(double interval[], int cd, int mask);

    static double calculateWidthDegrees(int n);



public:
    static double widthDegrees(int n);

    static std::string adjacentHash(const std::string& hash, const std::string& direction);

    static std::vector<std::string> neighbours(const std::string& hash);

    static LatLong decodeHash(const std::string& geohash);

    static double to180(double d);

    static std::string encodeHash(double latitude, double longitude, int length);
};
#endif


