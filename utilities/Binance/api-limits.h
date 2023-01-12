#ifndef BINANCE_API_LIMITS
#define BINANCE_API_LIMITS

#include <string>

class LimitTracker {
protected:

public:
    // Binance limit in API calls per minute
    static const int SAPI_UID_1M_LIMIT ;
    static const int SAPI_IP_1M_LIMIT ;
    
    // Bincance limit in sAPI calls header names
    static const std::string HEADER_SAPI_UID_1M_NAME;
    static const std::string HEADER_SAPI_IP_1M_NAME;

    // API calls weights
    static const int sendQuoteReqestUID;
    static const int acceptQuoteUID;


    // HELPER FUNCTIONS
    static void waitTillNextMinute();
    static void waitTillNextHour();
    static void waitTillNextDay();

};

#endif