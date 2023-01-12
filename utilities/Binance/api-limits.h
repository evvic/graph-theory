#ifndef BINANCE_API_LIMITS
#define BINANCE_API_LIMITS

#include <string>

// Binance Convert API limitations
// sapi/ endpoints (the oens used for the convert endpoints)
// Each endpoint with IP limits has an independent 12000 per minute limit.
// Each endpoint with UID limits has an independent 180000 per minute limit.
// Any endpoint has a hard limit of 1200 per minute
// GET /sapi/v1/convert/exchangeInfo  500 Weight(IP)
// POST /sapi/v1/convert/getQuote     200 Weight(UID)

class LimitTracker {
private:
    // boilerplate function for wait until (time period)
    void waitTillNext(std::chrono::duration<int> duration_type)

protected:
    int cntSapiUidWeight;   // count sAPI UID weight
    long sapiUidTimestamp;  // sAPI UID timestamp of last updated count
    
    int cntSapiIpWeight;    // count sAPI IP weight
    long sapiIpTimestamp;   // sAPI IP timestamp of last updated count

public:
    // Constructor
    LimitTracker();

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

    int updateUidWeight();
    int updateUidWeight(int w);
    bool enoughFreeWeight(int num_calls);
};

#endif