from tkinter import *
from plotter import getPlots
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)

def plot():
    # getting plot data from plotter.py which fetches and processes data from API
    fig0, fig1 = getPlots()


    canvas0 = FigureCanvasTkAgg(fig0, master=window)
    canvas0.draw()
    canvas0.get_tk_widget().pack()

    canvas1 = FigureCanvasTkAgg(fig1, master=window)
    canvas1.draw()
    canvas1.get_tk_widget().pack()

window = Tk()
window.title('')
window.geometry("1200x600")

# Lable
lable1 = Label(window, text="Get Stock Prices for last 100 days for Amazon, Microsoft, Tesla, Google, Facebook",font=("Verdana", 16))
lable1.pack()

# Button
plot_button = Button(master=window, command=plot, height=2, width=10, text="Fetch Data!!")
plot_button.pack()
window.mainloop()
