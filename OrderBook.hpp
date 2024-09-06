#include <iostream>
#include <vector>
#include <string>

#include "OrderBookEntry.hpp"
#include "CSVReader.hpp"
#pragma once

class OrderBook
{
    public:
    /** construct, reading a CSV data file*/
        OrderBook(std::string filename);
    /** return vector of all known products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product,
                                              std::string timestamp);
    /**returns the earliest time in the order book**/
        std::string getEarliestTime();
    /**returns the next time after the sent time in the order book**/
    /**if the is no next timestamp, wraps around to the start**/
        std::string getNextTime(std::string timestamp);
        std::string getPreviousTime(std::string timestamp);
    
        void insertOrder(OrderBookEntry& order);
    // matching engine
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
    
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
    static double getVolume(std::vector<OrderBookEntry>& orders);
    
        static double getAvgAskBid(double total, int count);
        static std::pair<double, int> getTotalAndCount(std::vector<OrderBookEntry>& orders);
    
    private:
        std::vector<OrderBookEntry> orders;
};


