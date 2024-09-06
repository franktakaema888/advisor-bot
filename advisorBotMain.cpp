#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
//external files
#include "advisorBotMain.hpp"
#include "OrderBookEntry.hpp"
#include "CSVReader.hpp"

advisorBotMain::advisorBotMain()
{
    
}

void advisorBotMain::init()
{
    std::string input; // variable to store user input
    
    currentTime = orderBook.getEarliestTime(); // variable to store the current time from the CSV

    timeStepCount = 1; // variable to count which timeStep the user is in
    
    // this while loop continously executes once the functions are done running
    while(true)
    {
        printMenu();
        // printing menu
        input = getUserOption();
        processUserOption(input);
    }
}

//Start Menu Option
void advisorBotMain::printMenu()
{
    std::cout << "=============" << std::endl;
    std::cout << "Please enter a command, or help for a list of commands" << std::endl;
    std::cout << "=============" << std::endl;
}

//Printing list from "printHelp"
void advisorBotMain::print(std::list<std::string> const &list)
{
    for (auto const &i: list) {
        std::cout << i << std::endl;
    }
}
// C1: help - list all available commands
int advisorBotMain::printHelp()
{
    std::list<std::string> list = { "1.help", "2.help cmd", "3.prod", "4.min", "5.max", "6.avg", "7.predict","8.time","9.step","10. market stats"};
    print(list);
     
    return 0;
}
// C2: help cmd - output help for specific command
int advisorBotMain::printHelpCmd()
{
    std::cout << "Please type 'help [insert specific command]'" << std::endl;
    
    return 0;
}
// Listening for user input to print out help commands
std::string advisorBotMain::helpCmdOptions()
{
    std::string helpOption = "";
    std::string line;
    std::cout << "Type command or help" <<std::endl;
    std::getline(std::cin, line);
    // invalid input would crash out of userOption and be caught. userOption will then stay at 0
    try{
        //string to integer stoi()
        helpOption = line;
    }catch(const std::exception& e)
    {
        //
    }
    std::cout << "=============" << std::endl;
    std::cout << "You chose: " << helpOption <<std::endl;
    
    return helpOption;
}
// help command options
void advisorBotMain::processHelpOption(std::string helpOption)
{
    if(helpOption == "help avg")
    {
        std::cout << "e.g. avg ETH/BTC 10 -> average ETH/BTC bid over the last 10 steps" << std::endl;
    }
    if(helpOption == "help prod")
    {
        std::cout << "e.g. prod -> prints out products available" << std::endl;
    }
    if(helpOption == "help min")
    {
        std::cout << "e.g. min [insert product] ask" << std::endl;
    }
    if(helpOption == "help max")
    {
        std::cout << "e.g. max [insert product] bid/ask" << std::endl;
    }
    if(helpOption == "help predict")
    {
        std::cout << "e.g. predict max/min [insert product] ask/bid" << std::endl;
    }
    if(helpOption == "help time")
    {
        std::cout << "e.g. time -> states current time in the data set" << std::endl;
    }
    if(helpOption == "help step")
    {
        std::cout << "e.g. step -> moves to the next time step" << std::endl;
    }
    if(helpOption == "help market stats")
    {
        std::cout << "e.g. market stats -> shows current asks or bids seen, as well as thier max and min" << std::endl;
    }
    
}

// C3: prod - list all available products
void advisorBotMain::printProd()
{
    std::cout << "BTC/USDT, DOGE/BTC, DOGE/USDT, ETH/BTC, ETH/USDT" << std::endl;
}

// C4: min - find minimum bid or ask for that product in current time step
void advisorBotMain::printMin()
{
    std::cout << "Enter [product],[ask/bid] - eg ETH/BTC,ask" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You Typed: " << input << std::endl;
    
    // tokeniser function to read user input and break it down
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    
    // searching through CSV for available products
    for(std::string const& p : orderBook.getKnownProducts())
    {
        if(tokens[0] == p)
        {
            //print min ask
            if(tokens[1] == "ask")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                          p,
                                                                          currentTime);
                std::cout << "The Min asks for " << p << " is " << OrderBook::getLowPrice(entries) << std::endl;
            }
            //print min bid
            if(tokens[1] == "bid")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                                          p,
                                                                          currentTime);
                std::cout << "The Min bids for " << p << " is " << OrderBook::getLowPrice(entries) << std::endl;
            }
        }
    }
}

// C5: max - find maximum bid or ask for that product in current time step
void advisorBotMain::printMax()
{
    std::cout << "Enter [product],[ask/bid] - eg ETH/BTC,ask" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You Typed: " << input << std::endl;
    
    // tokeniser function to read user input and break it down
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    
    // searching through CSV for available products
    for(std::string const& p : orderBook.getKnownProducts())
    {
        if(tokens[0] == p)
        {
            //print max ask
            if(tokens[1] == "ask")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                          p,
                                                                          currentTime);
                std::cout << "The Max asks for " << p << " is " << OrderBook::getHighPrice(entries) << std::endl;
            }
            //print max bid
            if(tokens[1] == "bid")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                                          p,
                                                                          currentTime);
                std::cout << "The Max bids for " << p << " is " << OrderBook::getHighPrice(entries) << std::endl;
            }
        }
       
    }
}

// C6: avg - compute average ask or bid for the sent product over the sent number of time steps.
void advisorBotMain::printAskBidAvg()
{
    // avg product ask/bid timesteps
    std::cout << "Enter [product],[ask/bid],[number of time steps] - eg ETH/BTC,ask,10" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You Typed: " << input << std::endl;
    
    // tokeniser function to read user input and break it down
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    
    std::string currentTimeStamp = currentTime; //local variable so that currentTime will not be over-written for the entire program
        
    // searching through CSV for available products
    for(std::string const& p : orderBook.getKnownProducts())
    {
        
        int numTimeSteps = stoi(tokens[2]); // converting string to integer for number of time step input

        if(tokens[0] == p) // checking if product input matches the products in CSV, getKnownProducts
        {
            if(tokens[1] == "ask" ) // checking if input is ask or bid, if bid, code will continue to the next condition
            {
                double avg = 0; // variable to save computation from getAvgAskBid
                std::pair<double, int> price_timeframe; // variable to save total count from getTotalAndCount
                double total_price = 0; // variable to store sum of total price
                int total_count = 0; // variable to store total count
                
                for( int i = 0; i < numTimeSteps; i++)
                {
                    
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                          p,
                                                          currentTimeStamp);
                    // sending entries into calculateAvgAskBid() to be computed and sent back to be stored in a vecotr
                    price_timeframe = OrderBook::getTotalAndCount(entries);
                    total_price += price_timeframe.first; // adding up the total price for different timeframes
                    total_count += price_timeframe.second; // adding up the total count
                    
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                        
                }
                //storing the computed average into avg
                avg = OrderBook::getAvgAskBid(total_price, total_count);
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The average " << p << " " << tokens[1] << " over the last " << numTimeSteps << " timesteps was " <<  avg << std::endl;
            }
            if(tokens[1] == "bid")
            {
                double avg = 0; // variable to save computation from getAvgAskBid
                std::pair<double, int> price_timeframe; // variable to save total count from getTotalAndCount
                double total_price = 0; // variable to store sum of total price
                int total_count = 0; // variable to store total count
                
                for( int i = 0; i < numTimeSteps; i++)
                {
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                          p,
                                                          currentTimeStamp);
                    // sending entries into calculateAvgAskBid() to be computed and sent back to be stored in a vecotr
                    price_timeframe = OrderBook::getTotalAndCount(entries);
                    total_price += price_timeframe.first;
                    total_count += price_timeframe.second;
                    
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                }
                avg = OrderBook::getAvgAskBid(total_price, total_count);
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The average " << p << " " << tokens[1] << " over the last " << numTimeSteps << " timesteps was " <<  avg << std::endl;
            }
            if(tokens[1] == " ")
            {
                std::cout << "Enter [product],[ask/bid],[number of time steps] - eg ETH/BTC,ask,10" << std::endl;
            }
        }
    }
}

//C7: predict
void advisorBotMain::printPredictMinMax()
{
    // predict min/max product ask or bid
    std::cout << "Enter [min/max],[product],[ask/bid] - eg max,ETH/BTC,bid" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You Typed: " << input << std::endl;
    
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    
    std::string currentTimeStamp = currentTime; //local variable so that currentTime will not be over-written for the entire program
    
    for(std::string const& p : orderBook.getKnownProducts())
    {
        if(tokens[1] == p) // checking if product input matches the products in CSV, getKnownProducts
        {
            if(tokens[2] == "ask" && tokens[0] == "max") // checking if input is ask or bid, if bid, code will continue to the next condition
            {
                double avg = 0;
                double totalMaxPrice = 0;
                
                
                for( int i = 0; i < timeStepCount; i++)
                {
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                          p,
                                                          currentTimeStamp);
                    totalMaxPrice += OrderBook::getHighPrice(entries); //max value
                    std::cout << "Current time is: " << currentTimeStamp << std::endl;
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                }
                
                avg = totalMaxPrice / timeStepCount;
                
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The predicted max ask price for " << p << " is " <<  avg << std::endl;
            }
            if(tokens[2] == "ask" && tokens[0] == "min") // checking if input is ask or bid, if bid, code will continue to the next condition
            {
                double avg = 0;
                double totalMaxPrice = 0;
                
                for( int i = 0; i < timeStepCount; i++)
                {
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                          p,
                                                          currentTimeStamp);
                    totalMaxPrice += OrderBook::getLowPrice(entries); //max value
                    
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                }
                
                avg = totalMaxPrice / timeStepCount;
                
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The predicted min ask price for " << p << " is " <<  avg << std::endl;
            }
            if(tokens[2] == "bid" && tokens[0] == "max")
            {
                double avg = 0;
                double totalMaxPrice = 0;
                
                for( int i = 0; i < timeStepCount; i++)
                {
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                          p,
                                                          currentTimeStamp);
                    totalMaxPrice += OrderBook::getHighPrice(entries); //max value
                    
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                }
                
                avg = totalMaxPrice / timeStepCount;
                
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The predicted max bid price for " << p << " is " <<  avg << std::endl;
            }
            if(tokens[2] == "bid" && tokens[0] == "min")
            {
                double avg = 0;
                double totalMaxPrice = 0;
                
                for( int i = 0; i < timeStepCount; i++)
                {
                    // Finding the orders from the CSV file
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                          p,
                                                          currentTimeStamp);
                    totalMaxPrice += OrderBook::getLowPrice(entries); //max value
                    
                    // updating the current time based on the numbers of iterations
                    currentTimeStamp = orderBook.getPreviousTime(currentTimeStamp);
                }
                
                avg = totalMaxPrice / timeStepCount;
                
                // The average ETH/BTC ask price over the last 10 timesteps was 1.0
                std::cout << "The predicted min bid price for " << p << " is " <<  avg << std::endl;
            }
            if(tokens[2] == " ")
            {
                std::cout << "Enter [min/max],[product],[ask/bid] - eg max ETH/BTC bid" << std::endl;
            }
        }
    }
}

//C8: time
void advisorBotMain::showCurrentTime()
{
    std::cout << "Current time is: " << currentTime << std::endl;
    std::cout << "Current count is: " << timeStepCount << std::endl;
}
// C9: step
void advisorBotMain::gotoNextTimeStep()
{
    std::cout << "Moving on to next time step" << std::endl;
    
    currentTime = orderBook.getNextTime(currentTime);
}

//Task 2: Print Market Stats
void advisorBotMain::printMarketStats()
{
    // avg product ask/bid timesteps
    std::cout << "Enter [product],[ask/bid] - eg: ETH/BTC,ask" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You Typed: " << input << std::endl;
    
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    for(std::string const& p : orderBook.getKnownProducts())
    {
        if(tokens[0] == p) // checking if product input matches the products in CSV, getKnownProducts
        {
            if(tokens[1] == "ask" ) // checking if input is ask or bid, if bid, code will continue to the next condition
            {
                
                std::cout << "=============" << std::endl;
                std::cout << "Product: " << p <<std::endl;
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                      p,
                                                      currentTime);
                //Printing out order entries
                std::cout << "Asks seen: " << entries.size() << std::endl;
                std::cout << "Max asks:  " << OrderBook::getHighPrice(entries) << std::endl;
                std::cout << "Min asks:  " << OrderBook::getLowPrice(entries) << std::endl;
                std::cout << "Volume:  " << OrderBook::getVolume(entries) << std::endl;
            }
            if(tokens[1] == "bid")
            {
                    std::cout << "=============" << std::endl;
                    std::cout << "Product: " << p <<std::endl;
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                          p,
                                                          currentTime);
                    //Printing out order entries
                    std::cout << "Bids seen: " << entries.size() << std::endl;
                    std::cout << "Max asks:  " << OrderBook::getHighPrice(entries) << std::endl;
                    std::cout << "Min asks:  " << OrderBook::getLowPrice(entries) << std::endl;
                    std::cout << "Volume:  " << OrderBook::getVolume(entries) << std::endl;
            }
            if(tokens[1] == " ")
            {
                std::cout << "Enter [product],[ask/bid] - eg: ETH/BTC,ask" << std::endl;
            }
        }
    }
    
}

// Listening for user input for menu
std::string advisorBotMain::getUserOption()
{
    std::string userOption = "";
    std::string line;
    std::cout << "Type command or help" <<std::endl;
    std::getline(std::cin, line);
    // invalid input would crash out of userOption and be caught. userOption will then stay at 0
    try{
        //string to integer stoi()
        userOption = line;
    }catch(const std::exception& e)
    {
        //
    }
    std::cout << "=============" << std::endl;
    std::cout << "You chose: " << userOption <<std::endl;
    
    return userOption;
}

// menu selection options
void advisorBotMain::processUserOption(std::string userOption)
{
    static int userInputCount = 0;
    
    if(userOption == "") //bad input
    {
        std::cout << "Invalid choice please type help for more options" << std::endl;
    }
    if(userOption != "help" && userOption != "help cmd")
    {
        userInputCount++; // takes note of the number of times user inputs function
    }
    if(userOption == "help") //Print help
    {
        printHelp();
    }
    if(userOption == "help cmd") //Print help cmd
    {
        std::string helpCmd;

        printHelpCmd();
        //printing options for help cmd assist
        helpCmd = helpCmdOptions();
        processHelpOption(helpCmd);
    }
    if(userOption == "prod") //Print list of products
    {
        printProd();
    }
    if(userOption == "min") //Print minimum
    {
        printMin();
    }
    if(userOption == "max") //Print maximum
    {
        printMax();
    }
    if(userOption == "avg") //Print average
    {
        printAskBidAvg();
    }
    if(userOption == "predict") //Print predict
    {
        printPredictMinMax();
    }
    if(userOption == "time") //Print current time
    {
        showCurrentTime();
    }
    if(userOption == "step") //Move to next time step
    {
        gotoNextTimeStep();
        timeStepCount++; // every time step is called we will increment global step counter by 1
    }
    if(userOption == "market stats") //Print market statistics
    {
        printMarketStats();
    }
}

