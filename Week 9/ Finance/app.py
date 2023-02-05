import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    current_id = session["user_id"]

    # Send data to index
    portfolio = db.execute(
        'SELECT "symbol", "name", SUM("shares"), "price", "total" FROM "portfolio" WHERE "user_id" = ? GROUP BY "symbol"', current_id)

    # Check each symbols's current price, sum total price and make dictionary
    dict_price = {}
    dict_total_price = {}

    sum_total_price = 0
    for port in portfolio:
        if port['SUM("shares")'] > 0:
            symbol = port["symbol"]
            shares = port['SUM("shares")']

            dict_symbol = lookup(symbol)

            # Check current price and sum total price
            current_price = dict_symbol["price"]
            total_price = current_price * shares

            # Make dictionary
            dict_price[symbol] = current_price
            dict_total_price[symbol] = total_price

            # Sum all price
            sum_total_price += total_price

    # Check remain cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", current_id)[0]["cash"]

    # Sum stocks plus cash
    total = cash + sum_total_price

    return render_template("index.html", portfolio=portfolio, price=dict_price, total_price=dict_total_price, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("buy.html")

    # User reached route via POST (as by submitting a form via POST)
    else:
        # Check missing symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        # Check missing shares
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        # Check invalid symbol
        dict_symbol = lookup(symbol)
        if not dict_symbol:
            return apology("invalid symbol", 400)

        # Check invalid shares
        try:
            share = int(shares)
        except:
            return apology("invalid shares", 400)

        if share < 0:
            return apology("invalid shares", 400)

        # Apology if user cannot afford the number of shares at the current price
        price = dict_symbol["price"]
        total_price = price * share

        current_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", current_id)[0]["cash"]

        if cash < total_price:
            return apology("can't afford", 400)

        # Update portfolio
        key_symbol = dict_symbol["symbol"]
        key_name = dict_symbol["name"]
        current_time = datetime.now()

        db.execute('INSERT INTO "portfolio" ("user_id", "symbol", "name", "method", "shares", "price", "total", "time") VALUES(?, ?, ?, ?, ?, ?, ?, ?)',
                   current_id, key_symbol, key_name, "buy", share, price, total_price, current_time)

        # Update cash remain
        cash_remain = cash - total_price
        db.execute('UPDATE users SET cash = ? WHERE id = ?', cash_remain, current_id)

        # Redirect user to home page
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    current_id = session["user_id"]

    portfolio = db.execute('SELECT "symbol", "shares", "price", "time" FROM "portfolio" WHERE "user_id" = ?', current_id)

    return render_template("history.html", portfolio=portfolio)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST (as by submitting a form via POST)
    else:
        # Get symbol dictionary from API
        symbol = request.form.get("symbol")

        # Check missing symbol
        if not symbol:
            return apology("missing symbol", 400)

        # Lookup symbol
        dict_symbol = lookup(symbol)

        # Check invalid symbol
        if not dict_symbol:
            return apology("invalid symbol", 400)

        # Get symbol key
        key_name = dict_symbol["name"]
        key_symbol = dict_symbol["symbol"]
        key_price = usd(dict_symbol["price"])

        # Show the quote result
        return render_template("quoted.html", name=key_name, price=key_price, symbol=key_symbol)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # check missing username
        if not username:
            return apology("missing username", 400)

        # Check username available
        users = db.execute("SELECT username FROM users")
        for user in users:
            if username == user["username"]:
                return apology("username is not available", 400)

        # Check missing password
        if not password or not confirmation:
            return apology("missing password", 400)

        # Check passwords match
        if password != confirmation:
            return apology("passwords don't match", 400)

        # Insert new user into users
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        # Login user
        row = db.execute("SELECT id FROM users WHERE username = ?", username)
        session["user_id"] = row[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Prompt user register
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via GET (as by clicking a link or via redirect)
    current_id = session["user_id"]

    if request.method == "GET":
        symbols = db.execute(
            'SELECT SUM("shares"), "symbol" FROM "portfolio" WHERE "user_id" = ? GROUP BY "symbol" ORDER BY "symbol"', current_id)
        return render_template("sell.html", symbols=symbols)

    # User reached route via POST (as by submitting a form via POST)
    else:
        # Check missing symbol
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("missing symbol", 400)

        # Check missing shares
        shares = request.form.get("shares")

        if not shares:
            return apology("missing shares", 400)

        # Check negative shares
        if int(shares) < 1:
            return apology("shares must be positive", 400)

        # Check too many shares
        shares_holding = db.execute('SELECT SUM("shares") FROM "portfolio" WHERE "user_id" = ? AND "symbol" = ?',
                                    current_id, symbol)[0]['SUM("shares")']

        if int(shares) > shares_holding:
            return apology("too many shares", 400)

        # Update portfolio
        dict_symbol = lookup(symbol)
        key_name = dict_symbol["name"]
        key_price = dict_symbol["price"]
        total_price = key_price * int(shares)
        current_time = datetime.now()
        share = 0 - int(shares)

        db.execute('INSERT INTO "portfolio" ("user_id", "symbol", "name", "method", "shares", "price", "total", "time") VALUES(?, ?, ?, ?, ?, ?, ?, ?)',
                   current_id, symbol, key_name, "sell", share, key_price, total_price, current_time)

        # Update cash remain
        cash = db.execute("SELECT cash FROM users WHERE id = ?", current_id)[0]["cash"]
        cash_remain = cash + total_price

        db.execute('UPDATE users SET cash = ? WHERE id = ?', cash_remain, current_id)

        # Redirect user to home page
        return redirect("/")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow users to change their passwords"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("password.html")

    # User reached route via POST (as by submitting a form via POST)
    else:
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation_new_password = request.form.get("confirmation_new_password")

        # Check missing password
        if not current_password:
            return apology("missing current password", 400)

        if not new_password or not confirmation_new_password:
            return apology("missing new password", 400)

        # Check new password match
        if new_password != confirmation_new_password:
            return apology("passwords don't match", 400)

        # Check current password
        current_id = session["user_id"]
        hash = db.execute('SELECT hash FROM users WHERE id = ?', current_id)

        # Current password wrong
        if not check_password_hash(hash[0]["hash"], current_password):
            return apology("invalid current password", 400)
        # Cureent password correct

        hash_new_password = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash_new_password, current_id)

        # Redirect user to home page
        return redirect("/")