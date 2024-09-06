#pragma once
#include <vector>
#include <list>

#include "OrderBookEntry.hpp"
#include "OrderBook.hpp"


class advisorBotMain
{
    public:
        advisorBotMain();
    
        void init();
    private:
        void printMenu();
        void print(std::list<std::string> const &list);
        int printHelp();
        int printHelpCmd();
        std::string helpCmdOptions();
        void processHelpOption(std::string helpOption);
        std::string getUserOption();
        void processUserOption(std::string userOption);
        void printProd();
        void printMin();
        void printMax();
        void printAskBidAvg();
        void printPredictMinMax();
        void showCurrentTime();
        void gotoNextTimeStep();
        void printMarketStats();
    
        std::string currentTime;
        
        int timeStepCount;
    
        OrderBook orderBook{"/Users/markcheong/Desktop/SIM-UOL/Object Oriented Programming CM 2005/advisorbot_proj/20200601.csv"};
        
        
};

