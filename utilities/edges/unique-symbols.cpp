#include "unique-symbols.h"
#include <string>

/************************/
/* STRUCT UniqueSymbols */
/************************/

// Constructor
UniqueSymbols::UniqueSymbols() {
    _id = 0; // init id to 0
    symbols_map = std::map<std::string, unsigned short>();
}

unsigned short UniqueSymbols::getSymbolID(std::string sym) {
    
    // Check if baseAsset symbol has an ID already
    const std::map<std::string, unsigned short>::iterator it_base = symbols_map.find(std::string(sym));

    // If cannot find in map, add it with a unique ID
    if (it_base == symbols_map.end()) {

        // Insert new pair to map
        symbols_map.insert(std::make_pair(std::string(sym), _id));

        const unsigned short tmp = _id;

        _id++; // increment _id

        return tmp;
    }
    // If in map already, get ID from symbol
    else {
        return it_base->second;
    }
}