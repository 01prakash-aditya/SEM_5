/*Name - Chaitanya Saagar
/*Roll number - 2301CS77*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int line_no = 1;
static long total_tokens = 0;

enum TokenKind {
  TK_KEYWORD=0, TK_IDENTIFIER, TK_INTEGER, TK_FLOAT, TK_HEXINT, TK_STRING,
  TK_OPERATOR, TK_DELIMITER, TK_COMMENT, TK_PREPROC, TK_ERROR, TK__COUNT
};
static const char* TK_NAMES[TK__COUNT] = {
  "KEYWORD","IDENTIFIER","INTEGER","FLOAT","HEX_INTEGER","STRING",
  "OPERATOR","DELIMITER","COMMENT","PREPROCESSOR","ERROR"
};
static long tk_counts[TK__COUNT] = {0};

typedef struct { const char* key; long cnt; } kw_t;
static kw_t kw_freq[] = {
  {"int",0}, {"float",0}, {"string",0}, {"if",0}, {"else",0},
  {"while",0}, {"function",0}, {"return",0}, {"true",0}, {"false",0}
};
static const int KW_N = sizeof(kw_freq)/sizeof(kw_freq[0]);

static void bump(enum TokenKind k) { ++tk_counts[k]; ++total_tokens; }

static char** id_list = NULL;
static int id_cap = 0, id_len = 0;

static int id_exists(const char* s){
  for(int i=0;i<id_len;i++) if(strcmp(id_list[i], s)==0) return 1;
  return 0;
}
static void id_add(const char* s){
  if(id_exists(s)) return;
  if(id_len==id_cap){
    id_cap = id_cap? id_cap*2 : 64;
    id_list = (char**)realloc(id_list, id_cap*sizeof(char*));
  }
  id_list[id_len++] = strdup(s);
}

static void print_kw(const char* k){
  bump(TK_KEYWORD);
  for(int i=0;i<KW_N;i++) if(strcmp(kw_freq[i].key, k)==0){ kw_freq[i].cnt++; break; }
  printf("Line %d: KEYWORD(%s)\n", line_no, k);
}
static void print_tok(enum TokenKind k, const char* label, const char* yy){
  bump(k);
  printf("Line %d: %s(%s)\n", line_no, label, yy);
}
static void print_op(const char* yy){ bump(TK_OPERATOR); printf("Line %d: OPERATOR(%s)\n", line_no, yy); }
static void print_delim(const char* yy){ bump(TK_DELIMITER); printf("Line %d: DELIMITER(%s)\n", line_no, yy); }
static void print_comment_single(const char* yy){ bump(TK_COMMENT); printf("Line %d: COMMENT_SINGLE(%s)\n", line_no, yy); }
static void print_comment_multi(){ bump(TK_COMMENT); printf("Line %d: COMMENT_MULTI\n", line_no); }
static void print_preproc(const char* what){ bump(TK_PREPROC); printf("Line %d: PREPROCESSOR(%s)\n", line_no, what); }
static void print_error(const char* yy){ bump(TK_ERROR); printf("Line %d: ERROR(%s)\n", line_no, yy); }

static void dump_stats(void){
  puts("\n====== Statistics ======");
  printf("Total tokens: %ld\n", total_tokens);
  puts("\n-- Token Distribution --");
  for(int i=0;i<TK__COUNT;i++)
    printf("%-14s : %ld\n", TK_NAMES[i], tk_counts[i]);
  puts("\n-- Keyword Frequency --");
  for(int i=0;i<KW_N;i++)
    printf("%-9s : %ld\n", kw_freq[i].key, kw_freq[i].cnt);
  puts("\n-- Identifier List (unique) --");
  for(int i=0;i<id_len;i++)
    printf("%s\n", id_list[i]);
}

static int unterminated_multicomment = 0;
static int unterminated_string = 0;
%}

%option noyywrap
%option nounput noinput
%option yylineno

%x STR
%x COMMENT
%x PREPROC

WS          [ \t\r]+
NL          \n

DIGIT       [0-9]
HEX         0[xX][0-9a-fA-F]+
EXP         [eE][+-]?{DIGIT}+
FLOAT1      ({DIGIT}*\.{DIGIT}+|{DIGIT}+\.{DIGIT}*)({EXP})?
FLOAT2      {DIGIT}+{EXP}
ID          [A-Za-z][A-Za-z0-9]*

HEADERID    [A-Za-z0-9_.]+

ESC         (\\[\\nt"t])
STRCHAR     ([^\\\"\n]|{ESC})

OP2         (==|!=|&&|\|\|)
OP1         [+\-*/=!<>]
DELIM       [(){};,]

%%

^[ \t]*\#[ \t]*include\b     { print_preproc("# include"); BEGIN(PREPROC); }
^[ \t]*\#[ \t]*define\b      { print_preproc("# define"); }

<PREPROC>"<"                  { print_op(yytext); }
<PREPROC>">"                  { print_op(yytext); }
<PREPROC>{HEADERID}           { print_tok(TK_IDENTIFIER,"IDENTIFIER", yytext); id_add(yytext); }
<PREPROC>\"{HEADERID}\"       { print_tok(TK_STRING,"STRING", yytext); }
<PREPROC>{WS}                 { }
<PREPROC>{NL}                 { line_no++; BEGIN(INITIAL); }
<PREPROC>.                    { print_error(yytext); }

"//".*                        { print_comment_single(yytext); }
"/*"                          { BEGIN(COMMENT); }
<COMMENT>"*/"                 { print_comment_multi(); BEGIN(INITIAL); unterminated_multicomment = 0; }
<COMMENT>{NL}                 { line_no++; }
<COMMENT>.                    { unterminated_multicomment = 1; }

\"                            { BEGIN(STR); unterminated_string = 1; yyless(1); }
<STR>{STRCHAR}+               { }
<STR>\"                       { print_tok(TK_STRING,"STRING", yytext); BEGIN(INITIAL); unterminated_string = 0; }
<STR>{NL}                     { print_error("Unterminated string literal"); line_no++; BEGIN(INITIAL); unterminated_string = 0; }
<STR><<EOF>>                  { print_error("Unterminated string literal at EOF"); unterminated_string = 0; }

"int"                         { print_kw("int"); }
"float"                       { print_kw("float"); }
"string"                      { print_kw("string"); }
"if"                          { print_kw("if"); }
"else"                        { print_kw("else"); }
"while"                       { print_kw("while"); }
"function"                    { print_kw("function"); }
"return"                      { print_kw("return"); }
"true"                        { print_kw("true"); }
"false"                       { print_kw("false"); }

{HEX}                         { print_tok(TK_HEXINT,"HEX_INTEGER", yytext); }
({FLOAT1}|{FLOAT2})           { print_tok(TK_FLOAT,"FLOAT", yytext); }
{DIGIT}+                      { print_tok(TK_INTEGER,"INTEGER", yytext); }

{ID}                          { print_tok(TK_IDENTIFIER,"IDENTIFIER", yytext); id_add(yytext); }

{OP2}                         { print_op(yytext); }
{OP1}                         { print_op(yytext); }
{DELIM}                       { print_delim(yytext); }

{WS}                          { }
{NL}                          { line_no++; }

.                              { print_error(yytext); }

<<EOF>>                        {
                                  if(unterminated_multicomment){
                                    fprintf(stderr, "Line %d: ERROR(Unterminated multi-line comment at EOF)\n", line_no);
                                  }
                                  if(unterminated_string){
                                    fprintf(stderr, "Line %d: ERROR(Unterminated string literal at EOF)\n", line_no);
                                  }
                                  dump_stats();
                                  return 0;
                                }

%%