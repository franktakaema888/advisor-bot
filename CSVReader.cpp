#include <stdio.h>
#include <iostream>
#include <fstream>

#include "CSVReader.hpp"

CSVReader::CSVReader()
{
    
}
//this function returns a vector of OrderBookEntry Objects: "returning the whole book shelf"
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;
    
    std::ifstream csvFile{csvFilename};
    std::string line;
    if(csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            try{
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }catch(const std::exception& e)
            {
                std::cout<< "CSVReader::readCSV bad data" << std::endl;
            }
        } //end of while
    }
    
    std::cout<< "CSVReader::readCSV read" << entries.size() << "entires" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine,char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    //Start from pos 0, find first char that is not separator
    start = csvLine.find_first_not_of(separator, 0);
    do{
        // find first of separator from the string
        end = csvLine.find_first_of(separator,start);
        // if start is already end of line
        if(start == csvLine.length() || start == end) break;
        // if there are still more char, then extract out the next string
        if(end >= 0) token = csvLine.substr(start, end - start);
        // if last string left
        else token = csvLine.substr(start, csvLine.length() - start);
        
        token.erase(0, token.find_first_not_of(" "));
        token.erase(token.find_last_not_of(" ") + 1);
        tokens.push_back(token);
        
        start = end + 1;
    }while(end > 0);
    
    return tokens;
}

// Checking for the length of the token
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price,amount;
    
    if(tokens.size() != 5) //bad
    {
        std::cout << "Bad line" <<std::endl;
        throw std::exception{};
    }
    
    try {
        // we have 5 tokens
        // stod meansz: string to decimal
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad Float!" << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad Float!" << tokens[4] << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                        amount,
                        tokens[0],
                        tokens[1],
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                               std::string amountString,
                                               std::string timestamp,
                                               std::string product,
                                               OrderBookType orderType)
{
    double price,amount;
    try {
        // we have 5 tokens
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad Float!" << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad Float!" << amountString << std::endl;
        throw;
    }
    
    OrderBookEntry obe{price,
                        amount,
                        timestamp,
                        product,
                        orderType};
    
    return obe;
}
