#include "api-limits.h"
#include "../http/http-scaffold.h"
#include <thread>   // thread sleeping
#include <chrono>   // thread sleeping
#include <iomanip>  // cout time
#include <iostream>

/* STATIC CONSTANTS */

// Binance limit in API calls per minute
const int LimitTracker::SAPI_UID_1M_LIMIT = 180000;
const int LimitTracker::SAPI_IP_1M_LIMIT  = 12000;

// Bincance limit in sAPI calls header names
const std::string LimitTracker::HEADER_SAPI_UID_1M_NAME = "X-SAPI-USED-UID-WEIGHT-1M";
const std::string LimitTracker::HEADER_SAPI_IP_1M_NAME  = "X-SAPI-USED-IP-WEIGHT-1M";

// API calls weights
const int LimitTracker::sendQuoteReqestUID = 200;
const int LimitTracker::acceptQuoteUID     = 200;

/* Constructor */
LimitTracker::LimitTracker() {
    cntSapiUidWeight = 0;
    cntSapiIpWeight = 0;
}

// Funcrtion to ping the binance server and update the current accumulated UID weight
// int LimitTracker::updateUidWeight() {
//     const std::string url = "https://api.binance.com/sapi/v1/asset/convert-transfer/queryByPage";

//     // Object that handles http request
//     HttpScaffold request;

//     // Unimportant paramas
//     const long startTime = 1673530356000;
//     const long endTime   = 1673530423000;

//     // Query parameters (appended to url)
//     std::map<std::string, std::string> params;
//     params.insert(std::make_pair("startTime", std::to_string(startTime)));
//     params.insert(std::make_pair("endTime", std::to_string(endTime)));

//     // Additional header
//     std::map<std::string, std::string> headers;

//     request.get(url, params, headers);

//     std::cout << "updateUidWeight: " << request.getHeader(HEADER_SAPI_UID_1M_NAME) << std::endl;
//     request.printResponseHeader();

//     return 69;

// }

int LimitTracker::updateUidWeight(int w) {
    cntSapiUidWeight = w;
    sapiUidTimestamp = HttpScaffold::timestampEpoch_ms();

    return w;
}


template <typename T>
void LimitTracker::waitTillNext() {
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Get the time point representing the next duration
    auto next_duration = std::chrono::time_point_cast<T>(now) + T(1);

    // Calculate the duration until the next duration
    auto duration = next_duration - now;

    // Print the duration until the next duration in seconds
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    std::cout << "Sleeping for " << seconds.count() << " seconds" << std::endl;

    // Put the thread to sleep until the next duration
    std::this_thread::sleep_until(next_duration);

    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Woken up! Time: " <<std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
}


void LimitTracker::waitTillNextMinute() {
    LimitTracker::waitTillNext<std::chrono::minutes>();
}

void LimitTracker::waitTillNextHour() {
    LimitTracker::waitTillNext<std::chrono::hours>();
}

void LimitTracker::waitTillNextDay() {
    LimitTracker::waitTillNext<std::chrono::duration<int, std::ratio<3600*24>>>();
}


// given the number of edges (each edge == api call)
// calculates and returns bool o
bool LimitTracker::enoughFreeWeight(int num_calls) {
    // TODO use timestamp of last time updated to see if its been over a minute

    if ((sendQuoteReqestUID * num_calls) > SAPI_UID_1M_LIMIT) {
        std::cout << (sendQuoteReqestUID * num_calls) << " > " << SAPI_UID_1M_LIMIT << std::endl;
        return false;
    }
    else {
        std::cout << (sendQuoteReqestUID * num_calls) << " < " << SAPI_UID_1M_LIMIT << std::endl;
        return true;
    }
}