// Name: Aditya Prakash
// Roll No: 2302CS11
// File: Assignment1.c

#include <bits/stdc++.h>
using namespace std;

string expr;
int pos = 0;

void skipSpaces() {
    while (pos < (int)expr.size() && isspace(expr[pos])) pos++;
}

bool matchChar(char c) {
    skipSpaces();
    if (pos < (int)expr.size() && expr[pos] == c) {
        pos++;
        return true;
    }
    return false;
}

long long evalsum(); 
long long evalterm();

long long evalfactor() {
    skipSpaces();
    if (matchChar('(')) {
        long long val = evalsum();
        if (!matchChar(')')) {
            throw runtime_error("Invalid expression: missing ')'");
        }
        return val;
    }
    if (pos >= (int)expr.size() || !isdigit(expr[pos])) {
        throw runtime_error("Invalid expression: expected number");
    }
    long long num = 0;
    while (pos < (int)expr.size() && isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    return num;
}

long long evalterm() {
    long long val = evalfactor();
    while (true) {
        skipSpaces();
        if (matchChar('*')) {
            val *= evalfactor();
        } else {
            break;
        }
    }
    return val;
}

long long evalsum() {
    long long val = evalterm();
    while (true) {
        skipSpaces();
        if (matchChar('+')) {
            val += evalterm();
        } else {
            break;
        }
    }
    return val;
}

int main() {
    getline(cin, expr);
    try {
        pos = 0;
        long long result = evalsum();
        skipSpaces();
        if (pos != (int)expr.size()) {
            throw runtime_error("Invalid expression: extra characters");
        }
        cout << result << "\n";
    } catch (const exception &e) {
        cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
