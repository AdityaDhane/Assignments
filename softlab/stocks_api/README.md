Stock market data analysis using the AlphaVantage API.
Submitted by:
    Aditya Dhane(BT18CSE028)
    Kaustubh Kathare(BT18CSE024)

Contribution:
    Fetching data from API
    tkinter gui

To run Application :
    command -> python gui.py

Files included:
    gui.py          - Frontend GUI app(Tkinter)
    plotter.py      - Backend Application, API data retrieval
    ipynb notebook  - Jupyter notebook implementation without GUI
    stock_API - tkinter windows application

Output Format:
    Graph1 : Stock prices of last 100 days for Amazon, Microsoft, Tesla, Google, Facebook (0 indicating 100 days before current day)
    Graph2 : Trading volume bar Graph
    Graph3 : Volatility bar Graph

Constraints:
    API only allows 5 requests per minute

Our API key : 6Y6GO80HMK6H92ND
To generate your key/read the docs : AlphaVantage

Explanation:
-For each company in the list, Send a GET request using the Python's `requests` module,to the API,
 which contains the API key as well as the other parameters.
-Here we have used the function=TIME_SERIES_DAILY_ADJUSTED to specify that we want the daily data 
 and with the outputsize=compact parameter we are getting the daily data for the last 100 trading days.
-We are also specifying the symbol i.e the stock we are looking for using symbol={} 
 where the {} is substituted using the Python formatted strings.


Data sent by the API for Tesla:-

{'Meta Data': {'1. Information': 'Daily Time Series with Splits and Dividend Events',
  '2. Symbol': 'FB',
  '3. Last Refreshed': '2021-03-11',
  '4. Output Size': 'Compact',
  '5. Time Zone': 'US/Eastern'},
 'Time Series (Daily)': {'2020-10-16': {'1. open': '267.375',
   '2. high': '271.37',
   '3. low': '265.3',
   '4. close': '265.93',
   '5. adjusted close': '265.93',
   '6. volume': '16622702',
   '7. dividend amount': '0.0000',
   '8. split coefficient': '1.0'},
  '2020-10-19': {'1. open': '265.53',
   '2. high': '268.55',
   '3. low': '259.88',
   '4. close': '261.4',
   '5. adjusted close': '261.4',
   '6. volume': '13586955',
   '7. dividend amount': '0.0000',
   '8. split coefficient': '1.0'},

    .
    .
    .
    .
    .
    .

  '2021-03-09': {'1. open': '261.175',
   '2. high': '268.53',
   '3. low': '259.81',
   '4. close': '265.74',
   '5. adjusted close': '265.74',
   '6. volume': '22039740',
   '7. dividend amount': '0.0000',
   '8. split coefficient': '1.0'},
  '2021-03-10': {'1. open': '268.64',
   '2. high': '268.97',
   '3. low': '262.87',
   '4. close': '264.9',
   '5. adjusted close': '264.9',
   '6. volume': '14210318',
   '7. dividend amount': '0.0000',
   '8. split coefficient': '1.0'},
  '2021-03-11': {'1. open': '268.11',
   '2. high': '277.9',
   '3. low': '267.82',
   '4. close': '273.88',
   '5. adjusted close': '273.88',
   '6. volume': '21833979',
   '7. dividend amount': '0.0000',
   '8. split coefficient': '1.0'}}}
