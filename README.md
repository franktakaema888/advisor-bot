# Advisor Bot Project

## Overview

This project implements an Advisor Bot for managing and interacting with an order book. The Advisor Bot reads order data from a CSV file, processes the data, and provides various functionalities such as getting minimum and maximum values, average prices, and predicting trends based on historical data.

## Features

- **Order Book Management**: The bot reads and processes order entries from a CSV file.
- **Command-line Interface**: Interact with the Advisor Bot through a simple command-line interface.
- **Historical Data Analysis**: Perform statistical analysis on historical order data such as:
  - Minimum and maximum prices
  - Average prices over time
  - Predictions based on historical trends
- **Real-time Interaction**: Users can input commands to retrieve specific information from the bot, such as the highest bid or lowest ask for a given time period.

## Files Overview

- **`advisorBotMain.cpp`**: The main entry point for the Advisor Bot application. This file initializes the bot and manages user interaction.
- **`advisorBotApp.cpp`**: Contains the logic for processing user commands and coordinating with other modules.
- **`OrderBook.cpp` / `OrderBook.hpp`**: Implements the logic for managing the order book, storing buy/sell orders, and performing operations such as retrieving min/max/average prices.
- **`OrderBookEntry.cpp` / `OrderBookEntry.hpp`**: Defines the structure of an individual order entry (bid/ask) in the order book.
- **`CSVReader.cpp` / `CSVReader.hpp`**: Handles reading of CSV files containing the order data.

## Getting Started

### Prerequisites

- A C++ compiler (e.g., GCC, Clang, etc.)
- C++ Standard Library support (C++11 or later)
- Make (optional, for building via Makefile)

### How to Build

1. Clone or download the project to your local machine.
   
2. Navigate to the project directory and compile the project using a C++ compiler:
   ```bash
   g++ -o advisorBot advisorBotMain.cpp advisorBotApp.cpp OrderBook.cpp OrderBookEntry.cpp CSVReader.cpp
   ```

3. Run the compiled executable:
   ```bash
   ./advisorBot
   ```

### Running the Application

After building the application, the Advisor Bot can be run from the command line. The bot will prompt the user to input commands to retrieve information from the order book. For example:

- `min BTC/USD ask` — retrieves the minimum asking price for the BTC/USD pair.
- `max BTC/USD bid` — retrieves the maximum bid price for the BTC/USD pair.
- `avg BTC/USD bid 10` — retrieves the average bid price for the last 10 entries.

### Command List

- **min** — Retrieves the minimum value for a specified currency pair and order type (ask or bid).
- **max** — Retrieves the maximum value for a specified currency pair and order type.
- **avg** — Calculates the average price over a given number of time periods.
- **predict** — Predicts future trends based on historical data.

### Example Usage

```bash
./advisorBot
> Enter command: min BTC/USD ask
> Minimum ask price for BTC/USD: 30000
```

## Future Enhancements

- Implement machine learning models to improve the prediction accuracy based on historical data.
- Add support for additional file formats (e.g., JSON or XML).
- Create a graphical user interface (GUI) for more intuitive interaction.

## Contributing

Contributions are welcome! Please feel free to open issues or submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under the MIT License.

---

This README format should provide a good structure for the project, making it clear for users or contributors to understand its purpose, features, and how to get started.
