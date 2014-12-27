#include <iostream>
#include "book_std.h"

using namespace std;
const char number = '8'; // t.kind == number means that t is a number Toke
const char quit = 'q'; // t.kind == quit means that t is quit Token
const char print = ';'; // t.kind == print means that t  is a print Token
const string prompt = ">"; //
const string result = "="; // to indicate whatever follows is a calculation result
const char let = 'L'; //  declaration token
const char name = 'a'; // name token
const string declarationKeyword = "let"; // declaration keyword

/**
* ======================================== *
*               Class: Token
* ======================================== *
*/

class Variable {
public:
    string name;
    double value;
    Variable (string n, double v) : name(n), value(v) {}
};

class Token {
public:
    char kind;
    double value;
    string name;
    // constructors
    Token(char ch)
        : kind(ch), value(0) {}
    Token(char ch, double val)
        : kind(ch), value(val) {}
    Token(char ch, string n)
        : kind(ch), name(n){}
};


/**
* ======================================== *
*           Class: Token_stream
*
*/
class Token_stream {
public:
    Token_stream(); // make a Token_stream that reads from cin
    Token get();    // get() will be defined later
    void putback(Token t); // put a tantoken back
    void ignore(char c); // discard characters up to and including a char "c"
private:
    bool full; // is there a token in the buffer
    Token buffer; // here is where we keep a Token put back using putback()
};
Token_stream::Token_stream()
        :full(false), buffer(0){} // no Token in the buffer
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer. The buffer is already full!!!");
    buffer = t; // copy t to buffer
    full = true; // buffer is now full
}

void Token_stream::ignore(char c)
// "c" represents a kind of Token
// e.g. the "print" sybmol
{
    // first take a look a the buffer;
    if (full && c==buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now search input:
    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

Token Token_stream::get()
{
    // use the data from buffer first
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch; // note: ">>" will skip the whitespace characters (space, newline, tab, etc.)
    switch (ch) {
        case print: // for "print"
        case quit:
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return Token(ch); // let each character represent it self
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch); // put digit back into the input stream
            double val;
            cin >> val; // make it a floating-point number
            return Token(number, val); // let '8' represent a number
        }
        default:
        error("You have just typed a bad token");
    }
}
//----------------------------------------
// Initialize a Token_stream object
//----------------------------------------
Token_stream ts;

/**
* ======================================== *
*           Function prototypes
* ======================================== *
*/
double primary();
double term();
double expression();
double declaration();
double statement();
bool is_declared();
double define_name();

/**
* ======================================== *
*           Function: statement()
* ======================================== *
*/
double statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case let:
            return declaration();
        default:
            ts.putback(t);
            return expression();
    }

}
/**
* ======================================== *
*           Function: declaration()
* ======================================== *
*/
// assume we have seen "let"
// handle: name = expression
// declare a variable called "name" with initial value "expression"
double declaration()
{
   Token t = ts.get();
    if (t.kind != name) error ("name expected in declaration");
    string var_name = t.name;

    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", var_name);

    double d = expression();
    define_name(var_name, d);
    return d;
}
/**
* ======================================== *
*           Function: expression()
* ======================================== *
*/
double expression()
{
    double left = term(); // read & eval a Term
    Token t = ts.get(); // get the next token
    while (true)
    {
        switch (t.kind)
        {
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t); // put "t" back to the token stream
                return left;
        }
    }
}

/**
* ======================================== *
*           Fn: term()
* ======================================== *
*/
double term()
{
    double left = primary();
    Token t = ts.get();
    while(true)
    {
        switch(t.kind)
        {
            case '*':
            {
                left *= primary();
                t = ts.get();
                break;
            }
            case '/':
            {
                double d = primary();
                if (d==0) error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            case '%':
            {
                double d = term();
                /*
                int i1 = int(left);
                if (i1 != left) error("left-hand operand of % is not int");
                int i2 = int(d);
                if (i2 != d) error("left-hand operand of % is not int");
                */
                int i1 = narrow_cast<int>(left);
                int i2 = narrow_cast<int>(d);
                if (i2 == 0) error("%: divided by zero");
                left = i1%i2;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t); // put back to token stream
                return left;
        }
    }
}

/**
* ======================================== *
*           Fn: primary()
* ======================================== *
*/
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
        case number:
            return t.value; // return a numeric value
        case '-':
            return -primary();
        case '+':
            return primary();
        default:
            error("primary expected!");
    }
}

/*
Simple calculator

Revision history:
    Revised by Yuhang Wang Dec. 26, 2014
    Originally written by Bjarne Stroustrup August 2004
Thsi program implements a basic expression calculator.
Input from cin; Output to cout.

the grammar for input is:

Statement:
    Expression
    Print
    Quit

Print:
    ;

Quit:
    q

Expression:
    Term
    Expression+Term
    Expression-Term

Term:
    Primary
    Term * Primary
    Term / Primary
    Term % Primary

Primary:
    Number
    ( Expression )

- Primary
+ Primary
Number:
    floating-point-literal

Input comes from cin through the Token_stream called ts.

 */
// a function to clean up the mess caused by bad inputs
// purge the current token stream up unitl the first "print" symbol

vector<Variable> var_table;

// return the value of the Variable named "s"
double get_value(string s)
{
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            return var_table[i].value;
        }
    }
    error("get: undefined variable", s);
}

// set the Variable named "s" to "d"
void set_value(string s, double d)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s) {
            var_table[i].value = d;
            return;
        }
    error("set: underfined variable", s);
}

// check: is var already in var_table?
bool is_declared(string var)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) return true;
    return false;
}

// add (var,val) to the var_table
double define_name(string var, double val)
{
    if (is_declared(var)) error(var, " declared twice");
    var_table.push_back(Variable(var,val));
    return val;
}

void clean_up_mess() { // better version
    ts.ignore(print);
}


void calculate()
{
    while (cin) {
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get(); // disgard all "print" symbols
            if (t.kind == quit) return; // quit if "quit" symbol is encountered
            ts.putback(t);
            cout << result << statement() << endl;
        }
        catch (exception& e) {
            cerr << e.what() << endl;
            clean_up_mess();
        }
    }
}
/**
* ======================================== *
*               Main
* ======================================== *
*/
int main() {
    try {
        calculate();
        keep_window_open();
        return 0;
    }
    catch (exception& e) {
        cerr << e.what() << '\n';
        keep_window_open("~~");
        return 1;
    }
    catch (...) {
        cerr << "exception \n";
        keep_window_open("~~");
        return 2;
    }
}