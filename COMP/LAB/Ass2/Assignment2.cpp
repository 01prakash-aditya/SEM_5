// Name: Aditya Prakash
// Roll No: 23002CS11
// File: Assignment2.c

#include <bits/stdc++.h>
using namespace std;

struct Var {
    string name;
    long long value;
    bool defined;
};
map<string, Var> symTable;

string line;
int pos;

void skipSpaces() {
    while (pos < (int)line.size() && isspace(line[pos])) pos++;
}

bool matchChar(char c) {
    skipSpaces();
    if (pos < (int)line.size() && line[pos] == c) {
        pos++;
        return true;
    }
    return false;
}

long long evalExpression(); 
long long evalTerm();
long long evalFactor();

long long getVariableValue(const string &name) {
    if (symTable.find(name) == symTable.end() || !symTable[name].defined) {
        throw runtime_error("Error : Variable '" + name + "' is not defined");
    }
    return symTable[name].value;
}

long long evalFactor() {
    skipSpaces();
    if (matchChar('(')) {
        long long val = evalExpression();
        if (!matchChar(')')) throw runtime_error("Error : Missing ')'");
        return val;
    }
    if (pos < (int)line.size() && isalpha(line[pos])) {
        string varName(1, line[pos++]);
        return getVariableValue(varName);
    }
    if (pos < (int)line.size() && isdigit(line[pos])) {
        long long num = 0;
        while (pos < (int)line.size() && isdigit(line[pos])) {
            num = num * 10 + (line[pos] - '0');
            pos++;
        }
        return num;
    }
    throw runtime_error("Error : Invalid syntax");
}

long long evalTerm() {
    long long val = evalFactor();
    while (true) {
        skipSpaces();
        if (matchChar('*')) {
            val *= evalFactor();
        }
        else if (matchChar('/')) {
            long long divisor = evalFactor();
            if (divisor == 0) throw runtime_error("Error : Division by zero");
            val /= divisor;
        }
        else if (matchChar('%')) {
            long long divisor = evalFactor();
            if (divisor == 0) throw runtime_error("Error : Division by zero");
            val %= divisor;
        }
        else break;
    }
    return val;
}

long long evalExpression() {
    long long val = evalTerm();
    while (true) {
        skipSpaces();
        if (matchChar('+')) {
            val += evalTerm();
        }
        else if (matchChar('-')) {
            val -= evalTerm();
        }
        else break;
    }
    return val;
}

void parseStatement() {
    skipSpaces();
    if (pos < (int)line.size() && isalpha(line[pos])) {
        string varName(1, line[pos++]);
        skipSpaces();
        if (matchChar('=')) {
            long long value = evalExpression();
            symTable[varName] = {varName, value, true};
            cout << varName << " = " << value << "\n";
            return;
        } else {
            pos = 0; // treat as expression if no '='
        }
    }
    long long result = evalExpression();
    cout << result << "\n";
}

int main() {
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line == "quit" || line == "exit") {
            cout << "Goodbye !\n";
            break;
        }
        try {
            pos = 0;
            parseStatement();
            skipSpaces();
            if (pos != (int)line.size()) {
                throw runtime_error("Error : Invalid syntax");
            }
        }
        catch (const exception &e) {
            cout << e.what() << "\n";
        }
    }
    return 0;
}
