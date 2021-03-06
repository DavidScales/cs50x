import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, get_shares

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for user cash
    rows = db.execute("SELECT cash FROM users WHERE id = :id",
                      id = session["user_id"])
    cash = rows[0]["cash"]

    # Count total portfolio value
    total = cash

    # Query user shares
    shares = get_shares(db)

    for share in shares:

        # calculate total & format currency
        share_total = round(share["price"] * share["quantity"], 2)
        share["price"] = usd(share["price"])
        share["total"] = usd(share["total"])
        total += share_total

    return render_template("index.html", shares = shares, total = usd(total), cash = usd(cash))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Cache form values
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)

        # Ensure that shares was submitted
        if not shares:
            return apology("must provide shares", 403)

        # Ensure that shares is a positive integer
        try:
            num_shares = int(shares)
            if num_shares <= 0:
                raise Exception("invalid value of shares")
        except:
            return apology("shares must be a positive integer", 403)

        # Lookup stock via API
        stock = lookup(symbol)

        # Confirm valid API response
        if stock == None:
            return apology("Invalid stock symbol")

        # Calculate purchase total
        cost = stock["price"] * num_shares

        # Query database for user cashe
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id = session["user_id"])
        cash = rows[0]["cash"]

        # Check that the user has enough cash
        if cash < cost:
            return apology("You don't have enough money :(")

        # Update transactions table
        try:
            new_transaction = db.execute("INSERT INTO transactions (user, symbol, price, quantity) VALUES(:user, :symbol, :price, :quantity)",
                                user = session["user_id"], symbol = stock["symbol"], price = stock["price"], quantity = num_shares)
            if new_transaction == None:
                return apology("SQL schema conflict", 500)

        except RuntimeError:
            return apology("invalid SQL command", 500)

        # Update user cash
        updated_cash = cash - cost
        try:
            update = db.execute("UPDATE users SET cash = :updated_cash where id = :id",
                                updated_cash = updated_cash, id = session["user_id"])

            if update == None:
               return apology("SQL schema conflict", 500)

        except RuntimeError:
            return apology("invalid SQL command", 500)


        # Return home
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query user transactions from transactions DB
    transactions = db.execute("SELECT * FROM transactions WHERE user=:user_id",
                              user_id = session["user_id"])

    for share in transactions:

        # calculate total & format currency
        share_total = round(share["price"] * share["quantity"], 2)
        share["price"] = usd(share["price"])
        share["total"] = usd(share_total)

    # Render table with transactions
    return render_template("history.html", transactions = transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Cache form values
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)

        # Lookup stock via API
        stock = lookup(symbol)

        # Confirm valid API response
        if stock == None:
            return apology("Invalid stock symbol")

        # Return quote view with stock data
        return render_template("quoted.html", price=stock["price"], symbol=stock["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Cache form values
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password and confirmation were submitted and match
        elif not password or not confirmation:
            return apology("must provide password and confirmation", 403)
        if password != confirmation:
            return apology("password and confirmation must match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)

        # Ensure username does not already exist
        if len(rows) == 1:
            return apology("username is already taken", 403)

        # Insert new user into users database
        try:
            new_id = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                                username=username, hash = generate_password_hash(password))
            if new_id == None:
                return apology("SQL schema conflict", 500)

        except RuntimeError:
            return apology("invalid SQL command", 500)

        # Remember which user has logged in
        session["user_id"] = new_id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change user password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Cache form values
        old_password = request.form.get("old-password")
        new_password = request.form.get("new-password")
        confirmation = request.form.get("confirmation")

        # Ensure old password was submitted
        if not old_password:
            return apology("must provide existing password", 403)

        # Ensure password and confirmation were submitted and match
        elif not new_password or not confirmation:
            return apology("must provide new password and confirmation", 403)
        if new_password != confirmation:
            return apology("new password and confirmation must match", 403)

        # Query database for user
        user = db.execute("SELECT * FROM users WHERE id = :id",
                          id=session["user_id"])[0]

        # Ensure that existing password is correct
        if not check_password_hash(user["hash"], old_password):
            return apology("Old password is not correct")

        # Update password
        update = db.execute("UPDATE users SET hash = :hash WHERE id = :id",
                            hash = generate_password_hash(new_password), id = session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Cache form values
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)

        # Ensure that shares was submitted
        if not shares:
            return apology("must provide shares", 403)

        # Ensure that shares is a positive integer
        try:
            num_shares = int(shares)
            if num_shares <= 0:
                raise Exception("invalid value of shares")
        except:
            return apology("shares must be a positive integer", 403)

        # Query user shares
        user_shares = get_shares(db)

        # Ensure that the user has the share that they are trying to sell
        try:
            share = [share for share in user_shares if share["symbol"] == symbol][0]
            print(share)
        except:
            return apology("You don't have that stock")

        # Ensure that the user has enough shares to sell
        if share["quantity"] < num_shares:
            return apology("You don't have enough shares to sell")

        # Calculate cash from sale
        profit = share["price"] * num_shares

        # Query database for user cashe
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id = session["user_id"])
        cash = rows[0]["cash"]

        # Update user cash
        updated_cash = cash + profit

        try:
            update = db.execute("UPDATE users SET cash = :updated_cash where id = :id",
                                updated_cash = updated_cash, id = session["user_id"])

            if update == None:
              return apology("SQL schema conflict", 500)

        except RuntimeError:
            return apology("invalid SQL command", 500)

        # Update transactions table
        try:
            new_transaction = db.execute("INSERT INTO transactions (user, symbol, price, quantity) VALUES(:user, :symbol, :price, :quantity)",
                                user = session["user_id"], symbol = share["symbol"], price = share["price"], quantity = -num_shares)
            if new_transaction == None:
                return apology("SQL schema conflict", 500)

        except RuntimeError:
            return apology("invalid SQL command", 500)

        # Return home
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Query user shares
        user_shares = get_shares(db)
        print(user_shares)

        return render_template("sell.html", shares = user_shares)

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
