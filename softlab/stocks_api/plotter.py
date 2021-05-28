import requests
import json
import numpy as np
import matplotlib.pyplot as plt

def getPlots():
    # Companies whose stock has to be monitered
    name_list = ['AMZN', 'MSFT', 'TSLA', 'GOOG', 'FB']

    # Fetching stock data from API
    data = []
    for name in name_list:
        resp = requests.get(
            "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY_ADJUSTED&symbol={}&outputsize=compact&apikey=6Y6GO80HMK6H92ND".format(name))
        resp = resp.json()  # converting the recieved data to JSON
        data.append(resp)


    # Fetching relevent data and processing it
    vols = []  # to store the sum of volumes for all the companies
    stddevs = []  # to store the standard devaition values for all the companies
    closing_prices = []
    for company_data in data:
        tot_volume = 0
        closing = []
        for day_data in company_data['Time Series (Daily)']:
            vol = int(company_data['Time Series (Daily)']
                      [day_data]['6. volume'])
            tot_volume += vol
            closing.append(
                float(company_data['Time Series (Daily)'][day_data]['4. close']))
        stddevs.append(np.std(closing))
        vols.append(tot_volume)
        closing_prices.append(closing)

    # Closing prices for last 100 days for all stocks
    i = 1
    fig0 = plt.figure(figsize=(35, 2.5))
    for name, vals in zip(name_list, closing_prices):
        plt.subplot(1, 5, i)
        plt.title(name)
        vals_c = vals
        vals_c.reverse()
        plt.plot(list(range(0, 100)), vals_c)
        i += 1

    # plotting Trading Volume
    fig1 = plt.figure(figsize=(9, 2.5))
    plt.subplot(1,2,1)
    plt.title('Trading Volume')
    
    # plotting VAlatility
    plt.bar(name_list, vols)
    plt.subplot(1,2,2)
    plt.title('Volatility')
    plt.bar(name_list, stddevs)

    return fig0, fig1
