#include "mod_std_lib_facilities.h"
using namespace std;

class Token
{
  public:
  char kind;
  double value;
};




Token get_token()
{
  
  
}; // function to read a token from cin
vector<Token> tok; // we'll put tokens here

double expression()
{
  double left = term();
  Token t = get_token();
  while (true) {
    switch (t.kind) {
    case '+':
      left += term(); // evaluate Term and add
      t = get_token();
      break
    case '-':
      left -= term(); // evaluate Term and subtract
      t =  get_token();
    default:
      return left; // finally no more + or -; return answer
    t = get_token();
  }
}


double term()
{
  double left = primary();
  Token t = get_token();
  while (true) {
    switch (t.kind) {
      case '*':
        left *= primary();
	t = get_token();
	break;
      case '/':
      {
        double d = primary();
	if (d==0) error("divide by zero");
        left /= primary();
	t = get_token();
	break;
      }
      default:
        return left;
    }
  }
}


double primary()
{
  Token t = get_token();
  switch (t.kind) {
  case '(': // handle '(' expression ')'
    {
      double d = expression();
      t = get_token();
      if (t.kind !=')') error("')' expected");
      return d;
    }
  case '8': // use '8' ot represent a number 
    return t.value;  // return the number's value
  default:
    error("primary expected");
  }
}

int main() 
{
  try {
    while (cin)
      cout << expression() << '\n';
      keep_window_open();
  }
  catch (exception& e) {
    cerr << e.what() << '\n';
    keep_window_open();
    return 1;
  }
  catch (...) {
    cerr << "exception\n";
    keep_window_open();
    return 2
  }
}
