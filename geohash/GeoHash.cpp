#include "GeoHash.h"
#include <iostream>
#define WARNING //
double GeoHash::PRECISION = 0.000000000001;

int GeoHash::BITS[5] = { 16, 8, 4, 2, 1 };

std::string GeoHash::BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";

const int GeoHash::MAX_HASH_LENGTH;

const int GeoHash::DEFAULT_MAX_HASHES;


double GeoHash::hashWidthCache[MAX_HASH_LENGTH] = { 0 };
double GeoHash::to180(double d) {
    if (d < 0) {
        return -to180(fabs(d));
    }
    else {
        if (d > 180) {
            double n = round(floor((d + 180) / 360.0));
            return d - n * 360;
        }
        else {
            return d;
        }
    }
}
std::map<std::string, std::map<std::string, std::string> > GeoHash::createBorders() {
    std::map<std::string, std::map<std::string, std::string> > m;
    m["BOTTOM"].insert(std::make_pair("EVEN", "028b"));
    m["TOP"].insert(std::make_pair("EVEN", "prxz"));
    m["LEFT"].insert(std::make_pair("EVEN", "0145hjnp"));
    m["RIGHT"].insert(std::make_pair("EVEN", "bcfguvyz"));
    m["BOTTOM"].insert(std::make_pair("ODD", "0145hjnp"));
    m["TOP"].insert(std::make_pair("ODD", "bcfguvyz"));
    m["LEFT"].insert(std::make_pair("ODD", "028b"));
    m["RIGHT"].insert(std::make_pair("ODD", "prxz"));
    return m;
}
std::map<std::string, std::map<std::string, std::string> > GeoHash::BORDERS = createBorders();
std::map<std::string, std::map<std::string, std::string> > GeoHash::createNeighbours() {
    std::map<std::string, std::map<std::string, std::string> > m;
    m["RIGHT"].insert(std::make_pair("EVEN", "bc01fg45238967deuvhjyznpkmstqrwx"));
    m["LEFT"].insert(std::make_pair("EVEN", "238967debc01fg45kmstqrwxuvhjyznp"));
    m["TOP"].insert(std::make_pair("EVEN", "p0r21436x8zb9dcf5h7kjnmqesgutwvy"));
    m["BOTTOM"].insert(std::make_pair("EVEN", "14365h7k9dcfesgujnmqp0r2twvyx8zb"));
    m["BOTTOM"].insert(std::make_pair("ODD", "238967debc01fg45kmstqrwxuvhjyznp"));
    m["TOP"].insert(std::make_pair("ODD", "bc01fg45238967deuvhjyznpkmstqrwx"));
    m["LEFT"].insert(std::make_pair("ODD", "14365h7k9dcfesgujnmqp0r2twvyx8zb"));
    m["RIGHT"].insert(std::make_pair("ODD", "p0r21436x8zb9dcf5h7kjnmqesgutwvy"));
    return m;
}
std::map<std::string, std::map<std::string, std::string> > GeoHash::NEIGHBOURS = createNeighbours
();

double GeoHash::calculateWidthDegrees(int n) {
    double a;
    if (n % 2 == 0) {
        a = -1;
    }
    else {
        a = -0.5;
    }
    double result = 180 / pow(2, 2.5 * n + a);
    return result;
}

double GeoHash::widthDegrees(int n) {
    if (n > 0 && n <= MAX_HASH_LENGTH) {
        // boost::lock_guard<boost::mutex> lock(cacheMutex);
        if (fabs(hashWidthCache[n - 1] - 0) < PRECISION) {
            hashWidthCache[n - 1] = calculateWidthDegrees(n);
        }
        return hashWidthCache[n - 1];
    }
    else {
        return calculateWidthDegrees(n);
    }
}

std::string GeoHash::adjacentHashAtBorder(const std::string& hash, const std::string& direction) {
    LatLong center = decodeHash(hash);
    if (direction == "RIGHT") {
        if (fabs(center.getLon() + widthDegrees(hash.size()) / 2 - 180) < PRECISION) {
            return encodeHash(center.getLat(), -180, hash.size());
        }
    }
    if (direction == "LEFT") {
        if (fabs(center.getLon() - widthDegrees(hash.size()) / 2 + 180) < PRECISION) {
            return encodeHash(center.getLat(), 180, hash.size());
        }
    }
    if (direction == "TOP") {
        if (fabs(center.getLon() + widthDegrees(hash.size()) / 2 - 90) < PRECISION) {
            return encodeHash(center.getLat(), center.getLon() + 180, hash.size());
        }
    }
    else {
        if (fabs(center.getLat() - widthDegrees(hash.size()) / 2 + 90) < PRECISION) {
            return encodeHash(center.getLat(), center.getLon() + 180, hash.size());
        }
    }
    return "";
}

std::string GeoHash::adjacentHash(const std::string& hash, const std::string& direction) {
    if (hash.size() == 0) return "";
    std::string adjacentHashAtBorder_str = adjacentHashAtBorder(hash, direction);
    if (adjacentHashAtBorder_str.size() != 0) {
        return adjacentHashAtBorder_str;
    }
    std::string base;
    for (int i = 0; i < hash.size() - 1; ++i) {
        base += std::tolower(hash[i]);
    }
    char lastChar = std::tolower(hash[hash.size() - 1]);
    std::string parity = (hash.size() % 2 == 0) ? "EVEN" : "ODD";

    if (BORDERS[direction][parity].find(lastChar) != std::string::npos) {
        base = adjacentHash(base, direction);
    }
    return base + BASE32[NEIGHBOURS[direction][parity].find(lastChar)];
}

std::vector<std::string> GeoHash::neighbours(const std::string& hash) {
    std::vector<std::string> list;
    std::string left = adjacentHash(hash, "LEFT");
    std::string right = adjacentHash(hash, "RIGHT");
    list.push_back(left);
    list.push_back(right);
    list.push_back(adjacentHash(hash, "TOP"));
    list.push_back(adjacentHash(hash, "BOTTOM"));
    list.push_back(adjacentHash(left, "TOP"));
    list.push_back(adjacentHash(left, "BOTTOM"));
    list.push_back(adjacentHash(right, "TOP"));
    list.push_back(adjacentHash(right, "BOTTOM"));
    return list;
}

void GeoHash::refineInterval(double interval[], int cd, int mask) {
    if ((cd & mask) != 0) {
        interval[0] = (interval[0] + interval[1]) / 2;
    }
    else {
        interval[1] = (interval[0] + interval[1]) / 2;
    }
}

LatLong GeoHash::decodeHash(const std::string& geohash) {
    if (geohash.size() == 0) return LatLong(-360, -360);
    bool isEven = true;
    double lat[2];
    double lon[2];
    lat[0] = -90.0;
    lat[1] = 90.0;
    lon[0] = -180.0;
    lon[1] = 180.0;
    for (int i = 0; i < geohash.size(); ++i) {
        std::string::size_type cd = BASE32.find(geohash[i]);
        for (int j = 0; j < 5; ++j) {
            int mask = BITS[j];
            if (isEven) {
                refineInterval(lon, cd, mask);
            }
            else {
                refineInterval(lat, cd, mask);
            }
            isEven = !isEven;
        }
    }
    double resultLat = (lat[0] + lat[1]) / 2;
    double resultLon = (lon[0] + lon[1]) / 2;
    return LatLong(resultLat, resultLon);
}


std::string GeoHash::encodeHash(double latitude, double longitude, int length) {
    if (length <= 0) {
        WARNING("length must be greater than zero");
        return "";
    }
    if (!(latitude >= -90 && latitude <= 90)) {
        WARNING("latitude: %f must be between -90 and 90 inclusive", latitude);
        return "";
    }
    longitude = to180(longitude);
    bool isEven = true;
    double lat[2];
    double lon[2];
    int bit = 0;
    int ch = 0;
    std::string geohash;
    lat[0] = -90.0;
    lat[1] = 90.0;
    lon[0] = -180.0;
    lon[1] = 180.0;

    while (geohash.size() < length) {
        if (isEven) {
            double mid = (lon[0] + lon[1]) / 2;
            if (longitude > mid) {
                ch |= BITS[bit];
                lon[0] = mid;
            }
            else {
                lon[1] = mid;
            }
        }
        else {
            double mid = (lat[0] + lat[1]) / 2;
            if (latitude > mid) {
                ch |= BITS[bit];
                lat[0] = mid;
            }
            else {
                lat[1] = mid;
            }
        }
        isEven = !isEven;
        if (bit < 4) {
            ++bit;
        }
        else {
            geohash += (BASE32[ch]);
            bit = 0;
            ch = 0;
        }
    }
    return geohash;
}
