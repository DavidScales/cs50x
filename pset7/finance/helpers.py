import csv
import os
import urllib.request

from flask import redirect, render_template, request, session
from functools import wraps
from random import random


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # API is unreliable, using placeholder for testing
    return round(random() * 100, 2)

    # # Reject symbol if it starts with caret
    # if symbol.startswith("^"):
    #     return None

    # # Reject symbol if it contains comma
    # if "," in symbol:
    #     return None

    # # Query Alpha Vantage for quote
    # # https://www.alphavantage.co/documentation/
    # try:

    #     # GET CSV
    #     url = f"https://www.alphavantage.co/query?apikey={os.getenv('API_KEY')}&datatype=csv&function=TIME_SERIES_INTRADAY&interval=1min&symbol={symbol}"
    #     webpage = urllib.request.urlopen(url)

    #     # Parse CSV
    #     datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

    #     # Ignore first row
    #     next(datareader)

    #     # Parse second row
    #     row = next(datareader)

    #     # Ensure stock exists
    #     try:
    #         price = float(row[4])
    #     except:
    #         return None

    #     # Return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
    #     return {
    #         "price": price,
    #         "symbol": symbol.upper()
    #     }

    # except:
    #     return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

def get_shares(db):
    """Return formatted share data from DB and API"""
    try:
        # Query database for user shares
        shares = db.execute("SELECT symbol, SUM(quantity) FROM transactions WHERE user = :user GROUP BY symbol",
                          user = session["user_id"])

        for share in shares:
            # Reformat from [{'symbol': 'GOOG', 'SUM(quantity)': 5}, {'symbol': 'NFLX', 'SUM(quantity)': 19}]
            # to            [{'symbol': 'GOOG', 'quantity':      5}, {'symbol': 'NFLX', 'quantity':      19}]
            share["quantity"] = share.pop("SUM(quantity)")

            # add current price & total value
            current_price = lookup(share["symbol"])
            if current_price == None:
                share["price"] = "NA"
                share["total"] = "NA"
            else:
                share_total = round(current_price * share["quantity"], 2)
                share["price"] = current_price
                share["total"] = share_total

        return shares

    except:
        raise
        # return None
