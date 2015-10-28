#include <iostream>
#include <string>
#include <vector>
#include "GeoHash.h"
using namespace std;

int main() {
    string destinationGeoHash = GeoHash::encodeHash(40.137432, 116.643129,5);
    std::vector<std::string> geohashNeighbour = GeoHash::neighbours(destinationGeoHash);
    cout << "destinationGeoHash:" << destinationGeoHash << endl;
    for (int i = 0; i < geohashNeighbour.size(); ++i) {
        cout << "i:" << i << ": " << geohashNeighbour[i] << endl;
    }
    return 0;
}
