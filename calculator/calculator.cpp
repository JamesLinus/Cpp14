/*
 Chapter 6. Design a calculator
 Author: Yuhang Wang
 Date: 2014-09-14
*/

#include "mod_std_lib_facilities.h"
//#include <iostream>
//using namespace std;

class Token 
{
  public:
    char kind;
    double value;
};

int main()
{
  cout << "Hello" << endl;
  cout << "Please enter an expression:";
  int lval = 0;
  int rval;

  cin>>lval; // read leftmost operand;
  cout << "lval: " << lval << endl;
  
  if (!cin) error("no first operand");

  for (char op; cin>>op;) 
  { // read operator and right-hand operand repeatedly
    cout << "op: " <<op <<endl;
    if (op != 'x') cin>>rval;
    if (!cin) error("no second operand");
    switch(op) {
      case '+':
        lval += rval; 
	break;
      case '-':
        lval -= rval;
	break;
      case '*':
        lval *= rval;
	break;
      case '/':
        lval /= rval;
	break;
      default:
        cout << "Result: "<<lval<<'\n';
	//keep_window_open();
	return 0;
      }
   }
   error("bad expression");
}
