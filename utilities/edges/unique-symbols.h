#ifndef UNIQUE_SYMBOLS
#define UNIQUE_SYMBOLS

#include <map>

// Struct for tracking each symbol and giving it a unique ID
struct UniqueSymbols {
    unsigned short _id;
    std::map<std::string, unsigned short> symbols_map; // SYMBOL, ID

    // Constructor, init ID
    UniqueSymbols();

    // If currency symbol does not have an ID assigned create and return one
    // If currency symbol has an ID assigned, return it
    unsigned short getSymbolID(std::string sym);
};

#endif