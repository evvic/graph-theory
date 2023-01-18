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

// HOUR LIMIT 
// For getQuote api call there were 360 calls allowed to make in one hour
// this call has a weight of 200, therefore 72000 hourly weight
// 2nd test yielded 363 api calls in an hour limit

// DAY LIMIT
class LimitTracker {
private:
    

protected:
    int cntSapiUidWeight;   // count sAPI UID weight
    long sapiUidTimestamp;  // sAPI UID timestamp of last updated count
    
    int cntSapiIpWeight;    // count sAPI IP weight
    long sapiIpTimestamp;   // sAPI IP timestamp of last updated count

    int num_calls;          // Count number of api calls made to server

public:
    /* Static members */    

    // Binance limit in API calls per minute
    static const int SAPI_UID_1M_WEIGHT_LIMIT ;
    static const int SAPI_IP_1M_WEIGHT_LIMIT ;

    // Binance api calls limit per hour
    static const int SAPI_UID_1H_CALLS_LIMIT;

    // Bincance limit in sAPI calls header names
    static const std::string HEADER_SAPI_UID_1M_NAME;
    static const std::string HEADER_SAPI_IP_1M_NAME;

    // Binance error codes
    static const int ERR_HOUR_LIMIT_CODE;
    static const int ERR_DAY_LIMIT_CODE;

    // API calls weights
    static const int sendQuoteReqestUID;
    static const int acceptQuoteUID;

    // HELPER FUNCTIONS
    static void waitTillNextMinute();
    static void waitTillNextHour();
    static void waitTillNextDay();

    // boilerplate function for wait until (time period)
    template <typename T>
    static void waitTillNext();

    /* Class members */

    // Constructor
    LimitTracker();

    int getNumCalls();                  // Returns num_calls
    int incrememntCalls();              // Increments num_calls
    int updateUidWeight();
    int updateUidWeight(int w);
    bool is1mLimitReached(int weight);
    bool enough1mFreeWeight(int calls);
    bool enoughFreeCalls1h(int calls);
};

#endif