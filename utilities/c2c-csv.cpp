#include "c2c-csv.h"
#include "c2c-edge.h"
#include <string>
#include <vector>

using namespace std;

// Cosntructor
ParseC2C::ParseC2C(string _fname) : fname(_fname) {}

//
// pass vect of edges by reference
void ParseC2C::readPairsToEdge(std::vector<C2CEdge>& edges) {

    if (FILE *fp = fopen(fname.c_str(), "r")) {

    // Read past the header in the first row
    fscanf(fp, ",%s,%s,%s,%s,%s,%s");

    // values for scanning row of csv
    C2CEdge tmp;
    char sym_pair[12];
    double minPrice;
    double maxPrice;

    // Scan each row of csv and add C2CEdge to vect of edges
    while (fscanf(fp, "%hu,%s,%s,%s,%lf,%lf,%lf",
            &tmp.id, sym_pair, tmp.baseAsset, tmp.toAsset, &minPrice, &maxPrice, &tmp.rate
            ) == 7) {
        edges.push_back(tmp);
    }

    fclose(fp);
}

}

// //
