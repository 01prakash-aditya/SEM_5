%{
#include <bits/stdc++.h>
using namespace std;

int line_num = 1;
int token_count = 0;

void print_token(const string &type, const string &lexeme) {
    cout << type << "(" << lexeme << ") [Line " << line_num << "]\n";
    token_count++;
}
%}

DIGIT   [0-9]
LETTER  [a-zA-Z]
ID      {LETTER}
NUMBER  {DIGIT}+
WS      [ \t]+
NEWLINE \n
ASSIGN  "="
PLUS    \+
MINUS   \-
MULTIPLY \*
DIVIDE  /
MODULO  %
LPAREN  \(
RPAREN  \)
COMMENT_START  "/\*"
COMMENT_END    "\*/"
LINE_COMMENT   "//".*

%%

"quit"              { print_token("QUIT", yytext); }
"exit"              { print_token("EXIT", yytext); }

{ID}                { print_token("IDENTIFIER", yytext); }
{NUMBER}            { print_token("NUMBER", yytext); }
{ASSIGN}            { print_token("ASSIGN", yytext); }
{PLUS}              { print_token("PLUS", yytext); }
{MINUS}             { print_token("MINUS", yytext); }
{MULTIPLY}          { print_token("MULTIPLY", yytext); }
{DIVIDE}            { print_token("DIVIDE", yytext); }
{MODULO}            { print_token("MODULO", yytext); }
{LPAREN}            { print_token("LPAREN", yytext); }
{RPAREN}            { print_token("RPAREN", yytext); }

{WS}                { /* ignore spaces/tabs */ }
{NEWLINE}           { print_token("NEWLINE", "\\n"); line_num++; }

/\*([^*]|\*+[^*/])*\*+/   { /* multi-line comment */ }
{LINE_COMMENT}            { /* single-line comment */ }

"/\*"                     { cerr << "Error: Unterminated comment at line " << line_num << "\n"; return 1; }

.                   { cerr << "Error: Unknown symbol '" << yytext << "' at line " << line_num << "\n"; }

%%

int yywrap() { return 1; }
