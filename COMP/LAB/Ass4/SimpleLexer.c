/*Name - Chaitanya Saagar
Roll number - 2301CS77*/

#include <stdio.h>

extern int yylex(void);
extern FILE* yyin;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening input file");
            return 1;
        }
    } else {
        yyin = stdin;
    }
    yylex();
    if (yyin && yyin != stdin) fclose(yyin);
    return 0;
}