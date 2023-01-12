#include "api-limits.h"
#include <thread>   // thread sleeping
#include <chrono>   // thread sleeping
#include <iomanip>  // cout time
#include <iostream>


/* CONSTANTS */
// Binance limit in API calls per minute
const int LimitTracker::SAPI_UID_1M_LIMIT = 180000;
const int LimitTracker::SAPI_IP_1M_LIMIT  = 12000;

// Bincance limit in sAPI calls header names
const std::string LimitTracker::HEADER_SAPI_UID_1M_NAME = "X-SAPI-USED-UID-WEIGHT-1M";
const std::string LimitTracker::HEADER_SAPI_IP_1M_NAME  = "X-SAPI-USED-IP-WEIGHT-1M";

// API calls weights
const int LimitTracker::sendQuoteReqestUID = 200;
const int LimitTracker::acceptQuoteUID     = 200;

void LimitTracker::waitTillNextMinute() {
    // implement when required
}


void LimitTracker::waitTillNextHour() {
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Get the time point representing the next hour
    auto next_hour = std::chrono::time_point_cast<std::chrono::hours>(now) + std::chrono::hours(1);

    // Calculate the duration until the next hour
    auto duration = next_hour - now;

    // Print the duration until the next hour
    std::cout << "Sleeping for " << duration.count() << " seconds" << std::endl;

    // Put the thread to sleep until the next hour
    std::this_thread::sleep_until(next_hour);

    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Woken up! Time: " << std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
}

void LimitTracker::waitTillNextDay() {
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Get the time point representing the next day
    auto next_day_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::hours(24));
    auto next_day = now + next_day_seconds;
    next_day = std::chrono::time_point_cast<std::chrono::seconds>(next_day);

    // Calculate the duration until the next day
    auto duration = next_day - now;

    // Print the duration until the next day
    std::cout << "Sleeping for " << duration.count() << " seconds" << std::endl;

    // Put the thread to sleep until the next day
    std::this_thread::sleep_until(next_day);

    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Woken up! Time: " <<std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
}