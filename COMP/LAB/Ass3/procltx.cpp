#include <bits/stdc++.h>
using namespace std;

extern int yylex();
extern int token_count;
extern int line_num;

int main() {
    cout << "Token Stream:\n";
    yylex();
    cout << "\nTotal tokens: " << token_count << "\n";
    cout << "Total lines: " << line_num << "\n";
    return 0;
}
