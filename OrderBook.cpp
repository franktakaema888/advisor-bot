#include <map>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "OrderBook.hpp"
#include "CSVReader.hpp"
/** construct, reading a CSV data file*/
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}
/** return vector of all known products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;
    
    std::map<std::string,bool> prodMap;
    
    for(OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for(auto const& e : prodMap)
    {
        products.push_back(e.first);
    }
    
    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                  std::string product,
                                  std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for(OrderBookEntry& e : orders)
    {
        if(e.orderType == type &&
           e.product == product &&
           e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e: orders)
    {
        if(e.price > max) max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e: orders)
    {
        if(e.price < min) min = e.price;
    }
    return min;
}

double OrderBook::getVolume(std::vector<OrderBookEntry>& orders)
{
    double volume = orders[0].price;
    for (OrderBookEntry& e: orders)
    {
        volume += e.amount;
    }
    return volume;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for(OrderBookEntry& e : orders)
    {
        if(e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if(next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

std::string OrderBook::getPreviousTime(std::string timestamp)
{
    std::string previous_timestamp = "";
//    for(OrderBookEntry& e : orders)
//    {
//        if(e.timestamp < timestamp && e.timestamp != timestamp)
//        {
//            previous_timestamp = e.timestamp;
//            break;
//        }
//    }
    for(int i = orders.size() - 1; i >= 0; --i)
    {
        OrderBookEntry& e = orders[i];
        if(e.timestamp < timestamp && e.timestamp != timestamp)
        {
            previous_timestamp = e.timestamp;
            break;
        }
    }
    
    if(previous_timestamp == "")
    {
        previous_timestamp = orders[0].timestamp;
//        previous_timestamp = orders[orders.size() - 1].timestamp;
    }

    return previous_timestamp;
}
 
void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);
    // sales = []
    std::vector<OrderBookEntry> sales;
    
    //sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    
    //sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    
    // for asks in ask
    for(OrderBookEntry& ask : asks)
    {
        // for bids in bid
        for(OrderBookEntry& bid : bids)
        {
            //if bid.price >= ask.price then we have a match
            if(bid.price >= ask.price)
            {
                // if the condition above is met then we generate a sale
                OrderBookEntry sale{ask.price, 0, timestamp,
                                product,
                                OrderBookType::asksale};
                
                if(bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if(ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }
                
                
                // bid completely clears asks
                if(bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }
                // ask is completely gone slice the bid
                if(bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }
                //bid is completely gone slice the ask
                if(bid.amount < ask.amount &&
                   bid.amount > 0)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;
}

//Compute the average price of asks/bids
double OrderBook::getAvgAskBid(double total, int count)
{
    double total_price = total;
    int total_count = count;
    
    std::cout << " count "<<count << " total " << total << std::endl;
    
    return total_price/total_count; // divides total sum by number of time frames
}

// computing and returning the total price and count for one timeframe
std::pair<double, int> OrderBook::getTotalAndCount(std::vector<OrderBookEntry>& orders)
{
    double total = 0;
    int count = 0;
    
    // loops through entire order for one tinmeframe
    for (OrderBookEntry& e : orders)
    {
        total += e.price; // adds up all the orders for one timeframe
        count++; // counts number of loops per timeframe
    }
    
    return std::make_pair(total,count); // returns total sum and number of time frames
}
