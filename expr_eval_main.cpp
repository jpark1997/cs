#include <iostream>
#include <string>
#include "expr_eval.h"
#include "math.h"
#include "stdlib.h"
#include "mystack.h"
#include <iomanip>

using namespace std;

double expr_eval::eval(string exp, double x) {
	mystack<string> ops;
	mystack<double> val;
/*
	string mul = "*";
	string minus = "-";
	string plus = "+";
	string divide = "/";
	string power = "^";
	string sinS = "s";
	string cosS = "c";
	string tanS = "t";
	string logS = "l";
	string openP = "(";
	string closeP = ")";
*/
	int prt = 0;// 1 if there is ( after * or /
	int i = 0;
	while(i < exp.length()) {
		if(exp.at(i) == '+') {//when +, check for *,/^ and push
			if(ops.empty() == 0) {
				if(ops.top() == "*" || ops.top() == "/" || ops.top() == "^") {
					double a2 = val.pop();
					double a1 = val.pop();
					string opp;
					opp = ops.pop();
					double ar;
					if(opp == "*") {
						ar = a1*a2;
						val.push(ar);
					} else if(opp == "/") {
						ar = a1/a2;
						val.push(ar);
					} else if(opp == "^") {
						ar = pow(a1,a2);
						val.push(ar);
					}// do *,/,^ first
	
					ops.push("+");
					i++;
				} else {
					ops.push("+");
					i++;
				}
			} else {
				ops.push("+");
				i++;
			}
		} else if(exp.at(i) == '-') {//when -, check for *,/^ and push
			if(ops.empty() == 0) {
				if(ops.top() == "*" || ops.top() == "/" || ops.top() == "^") {
					double a2 = val.pop();
					double a1 = val.pop();
					string opp;
					opp = ops.pop();
					double ar;
					if(opp == "*") {
						ar = a1*a2;
						val.push(ar);
					} else if(opp == "/") {
						ar = a1/a2;
						val.push(ar);
					} else if(opp == "^") {
						ar = pow(a1,a2);
						val.push(ar);
					}
	
					ops.push("-");
					i++;
				} else {
					ops.push("-");
					i++;
				}
			} else {
				ops.push("-");
				i++;
			}

		} else if(exp.at(i) == 's') {
			ops.push("s");
			i = i+3;
		} else if(exp.at(i) == 'c') {
			ops.push("c");
			i = i+3;
		} else if(exp.at(i) == 't') {
			ops.push("t");
			i = i+3;
		} else if(exp.at(i) == 'l') {
			ops.push("l");
			i = i+3;
			/////////////////////////when sin,cos,tan,log
		} else if(exp.at(i) == '*') {
			ops.push("*");
			i++;
		} else if(exp.at(i) == '/') {
			ops.push("/");
			i++;
		} else if(exp.at(i) == '^') {
			ops.push("^");
			i++;
			/////////////////////when *,/,^, push
		} else if(exp.at(i) == 'x') {
			val.push(x);
			i++;
		} else if(exp.at(i) == '(') {
			ops.push("(");
			i++;
		} else if(exp.at(i) == ')') {
			//calculation
			
			while(ops.top() != "(") { // until the input is (
				string inPs;
				inPs = ops.pop();
				double inPd;
				inPd = val.pop();

				if(inPs == "+") {
					inPd = val.pop() + inPd;
				} else if(inPs == "-") {
					inPd = val.pop() - inPd;
				} else if(inPs == "*") {
					inPd = val.pop() * inPd;
				} else if(inPs == "/") {
					inPd = val.pop()/inPd;
				} else if(inPs == "^") {
					inPd = pow(val.pop(),inPd);
				}

				val.push(inPd);
			}
			ops.pop();
			// check for trig function and log after ()
			if(ops.empty() == 0) {
				if(ops.top() == "s") {
					ops.pop();
					double sinD;
					sinD = val.pop();
					double sinDResult;
					sinDResult = sin(sinD*3.14159265/180.0);
					val.push(sinDResult);
				} else if(ops.top() == "c") {
					ops.pop();
					double cosD;
					cosD = val.pop();
					double cosDResult;
					cosDResult = cos(cosD*3.14159265/180.0);
					val.push(cosDResult);
				} else if(ops.top() == "t") {
					ops.pop();
					double tanD;
					tanD = val.pop();
					double tanDResult;
					tanDResult = tan(tanD*3.14159265/180.0);
					val.push(tanDResult);
				} else if(ops.top() == "l") {
					ops.pop();
					double logD;
					logD = val.pop();
					double logDResult;
					logDResult = log10(logD);
					val.push(logDResult);
				} else if(ops.top() == "/") {
					ops.pop();
					double a2 = val.pop();
					double a1 = val.pop();
					//string opp = ops.pop();
					double ar = a1/a2;
					val.push(ar);
				} else if(ops.top() == "*") {
					ops.pop();
					double a2 = val.pop();
					double a1 = val.pop();
					//string opp = ops.pop();
					double ar = a1*a2;
					val.push(ar);
				} else if(ops.top() == "^") {
					ops.pop();
					double a2 = val.pop();
					double a1 = val.pop();
					//string opp = ops.pop();
					double ar = pow(a1, a2);
					val.push(ar);
				}
			}

			i++;

		} else if(exp.at(i) >= 48 && exp.at(i) <= 57){ // if input is number, push to val
			int num = 0;
			char charNum[10000];
			while((exp[i] >= 48 && exp[i] <= 57) || exp[i] == '.') {
				charNum[num] = exp.at(i);
				num++;
				i++;
			}
			//string nn = exp.substr(i, num);
			charNum[num] = '\0';

			double dd;
			dd = atof(charNum);
			val.push(dd);
		}
	}

	//after input is done, check for there is other operations to be done
	if(ops.size() == 0) {//if there is no more ops
		return val.pop();
	} else {//there are more ops
		while(ops.size() != 0) {
			string inPs1;
			inPs1 = ops.pop();
			double inPd1;
			inPd1 = val.pop();
			
			if(inPs1 == "+") {
				if(ops.size() != 0) {
					if(ops.top() == "-") {
						ops.pop();
						ops.push("+");
						inPd1 = inPd1 - val.pop();
					} else {
						inPd1 = val.pop() + inPd1;
					}
				} else {	
					inPd1 = val.pop() + inPd1;
				}
			} else if(inPs1 == "-") {
				inPd1 = val.pop() - inPd1;
			} else if(inPs1 == "*") {
				inPd1 = val.pop() * inPd1;
			} else if(inPs1 == "/") {
				inPd1 = val.pop() / inPd1;
			} else if(inPs1 == "^") {
				inPd1 = pow(val.pop(), inPd1);
			}
			
			val.push(inPd1);
		}

		return val.pop();
	}

	
	
}

int main(){
	string equation;
	//double xVal;

	expr_eval ee;
	while(cin >> equation) {
		size_t found = equation.find("x");
		if(found == std::string::npos) { // if there is no x
			double result = ee.eval(equation, 0.00);
//			cout.precision(2);
			cout << fixed << setprecision(2) << result << endl;
		}
		else { // there is x
			double xVal;
			cin >> xVal;
			double result = ee.eval(equation, xVal);
			cout.precision(2);
			cout << fixed << setprecision(2) << result << endl;
		}
	}

	cout << "" << endl;
  return 0;
}
