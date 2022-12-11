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
        fscanf(fp, ",%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", header_names[0], header_names[1], header_names[2], header_names[3],
            header_names[4], header_names[5]);

        cout << "Headers read." << endl;

        for (auto header : header_names) {
            printf("%s, ", header);
        }

        // values for scanning row of csv
        unsigned short row;
        unsigned short id;
        unsigned short to;
        char sym_pair[12];
        char baseAsset[8];
        char toAsset[8];
        double minPrice;
        double maxPrice;
        double rate;

        cout << "Reading rows.." << endl;

        // map ID trackers
        unsigned short _id = 0;
        //map<string,int>::iterator _it;

        // Scan each row of csv and add C2CEdge to vect of edges
        // while (fscanf(fp, "%hu,%s,%s,%s,%lf,%lf,%lf",
        while (fscanf(fp, "%hu,%[^,],%[^,],%[^,],%lf,%lf,%lf",
                &row, sym_pair, baseAsset, toAsset, &minPrice, &maxPrice, &rate
                ) != EOF)
        {
            /* BASE ASSET */
            // Check if baseAsset symbol has an ID already
            const map<string,int>::iterator it_base = symbols.find(string(baseAsset));

            // If cannot find in map, add it with a unique ID
            if (it_base == symbols.end()) {

                // Set id to _id
                id = _id;

                cout << "Creating id " << id << " for " << baseAsset << endl;

                // Insert new pair to map
                symbols.insert(make_pair(string(baseAsset), _id));
                _id++; // increment _id
            }
            // If in map already, get ID from baseAsset
            else {

                id = it_base->second;
                cout << "Obtaining id " << id << " for " << baseAsset << endl;
            }

            /* QUOTE/TO ASSET */
            // Check if toAsset symbol has an ID already
            const map<string,int>::iterator it_to = symbols.find(string(toAsset));

            // If cannot find in map, add it with a unique ID
            if (it_to == symbols.end()) {

                // Set id to _id
                to = _id;

                cout << "Creating id " << to << " for " << toAsset << endl;

                // Insert new pair to map
                symbols.insert(make_pair(string(toAsset), _id));
                _id++; // increment _id
            }
            // If in map already, get ID from toAsset
            else {
                to = it_to->second;
                cout << "Obtaining id " << to << " for " << toAsset << endl;
            }

            cout << "Adding id " << id << " to vector" << endl;
            // Add edge
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
