#include "api-limits.h"
#include "../http/http-scaffold.h"
#include <thread>   // thread sleeping
#include <chrono>   // thread sleeping
#include <iomanip>  // cout time
#include <iostream>

/* STATIC CONSTANTS */

// Binance limit in API calls per minute
const int LimitTracker::SAPI_UID_1M_WEIGHT_LIMIT = 180000;
const int LimitTracker::SAPI_IP_1M_WEIGHT_LIMIT  = 12000;

// Binance api calls limit per hour
const int LimitTracker::SAPI_UID_1H_CALLS_LIMIT = 360;

// Binance error codes
const int LimitTracker::ERR_HOUR_LIMIT_CODE = 345103;
const int LimitTracker::ERR_DAY_LIMIT_CODE  = 345239;
const int LimitTracker::ERR_FAILED_TO_QUOTE = 345166;

// Binance limit in sAPI calls header names
const std::string LimitTracker::HEADER_SAPI_UID_1M_NAME = "X-SAPI-USED-UID-WEIGHT-1M";
const std::string LimitTracker::HEADER_SAPI_IP_1M_NAME  = "X-SAPI-USED-IP-WEIGHT-1M";

// API calls weights
const int LimitTracker::sendQuoteReqestUID = 200;
const int LimitTracker::acceptQuoteUID     = 500;
const int LimitTracker::checkOrderStatus   = 100;

// Calculate ideal max circular arbitrage length (how much weight in 1 minute to accept all quoted trades)
//const int LimitTracker::MAX_CIRCULAR_TRADE_SIZE = LimitTracker::SAPI_UID_1M_WEIGHT_LIMIT / (LimitTracker::acceptQuoteUID + (2 * LimitTracker::checkOrderStatus));
const int LimitTracker::MAX_CIRCULAR_TRADE_SIZE = 10;

/* Constructor */
LimitTracker::LimitTracker() {
    cntSapiUidWeight = 0;
    cntSapiIpWeight = 0;
    num_calls = 0;
}


int LimitTracker::incrememntCalls() {
    num_calls++;
    return num_calls;
}

int LimitTracker::getNumCalls() {
    return num_calls;
}

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

void LimitTracker::waitForSeconds(double seconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds * 1000)));
}


bool LimitTracker::is1mLimitReached(int weight) {
    if (weight >= SAPI_UID_1M_WEIGHT_LIMIT) return true;
    else return false;
}



// given the number of edges (each edge == api call)
// calculates and returns bool o
bool LimitTracker::enough1mFreeWeight(int numCalls) {
    // TODO use timestamp of last time updated to see if its been over a minute


    // Check enough weight for the minute
    if (is1mLimitReached(cntSapiUidWeight * numCalls)) {
        std::cout << (cntSapiUidWeight * numCalls) << " > " << SAPI_UID_1M_WEIGHT_LIMIT << std::endl;
        return false;
    }
    else {
        std::cout << (cntSapiUidWeight * numCalls) << " < " << SAPI_UID_1M_WEIGHT_LIMIT << std::endl;
        return true;
    }
}

// given the number of edges (each edge == api call)
// calculates and returns bool o
bool LimitTracker::enoughFreeCalls1h(int numCalls) {
    // TODO use timestamp of last time updated to see if its been over a minute

    // Check enough calls left in the hour
    if (((numCalls % SAPI_UID_1H_CALLS_LIMIT) + num_calls) <= SAPI_UID_1H_CALLS_LIMIT) {
        //std::cout << (numCalls + num_calls) << " < " << SAPI_UID_1H_CALLS_LIMIT << std::endl;
        return true;
    }
    else {
        //std::cout << (numCalls + num_calls) << " > " << SAPI_UID_1H_CALLS_LIMIT << std::endl;
        return false;
    }
}