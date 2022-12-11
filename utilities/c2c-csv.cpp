#include "c2c-csv.h"
#include "c2c-edge.h"
#include <string>
#include <vector>
#include <iostream> //test

using namespace std;

// Cosntructor
ParseC2C::ParseC2C(string _fname) : fname(_fname) {}

//
// pass vect of edges by reference
void ParseC2C::readPairsToEdge(std::vector<C2CEdge>& edges) {

    if (FILE *fp = fopen(fname.c_str(), "r")) {


        const unsigned short header_cnt = 6;
        char header_names[6][12];

        // GIVING WARNINGS, CHANGE TO C STRINGS
        // Read past the header in the first row
        fscanf(fp, ",%s,%s,%s,%s,%s,%s", header_names[0], header_names[1], header_names[2], header_names[3],
            header_names[4], header_names[5]);

        cout << "Headers read." << endl;

        for (auto header : header_names) {
            printf("%s, ", header);
        }

        // values for scanning row of csv
        unsigned short id;
        unsigned short to;
        char sym_pair[12];
        char baseAsset[8];
        char toAsset[8];
        double minPrice;
        double maxPrice;
        double rate;

        cout << "Reading rows.." << endl;

        // Scan each row of csv and add C2CEdge to vect of edges
        // while (fscanf(fp, "%hu,%s,%s,%s,%lf,%lf,%lf",
        while (fscanf(fp, "%hu,%[^,],%[^,],%[^,],%lf,%lf,%lf",
                &id, sym_pair, baseAsset, toAsset, &minPrice, &maxPrice, &rate
                ) != EOF) {
            cout << "> Read row " << id << endl;
            edges.emplace_back((C2CEdge(id, to, rate, baseAsset, toAsset)));

        }

        cout << "Rows read." << endl;

        fclose(fp);
    } else {
        cout << "Error opening " << fname << endl;
    }

    cout << "Returning" << endl;

}

// //
