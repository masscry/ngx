%{

#include <stdio.h>
#include <stdlib.h>
#include <config.tab.h>

#define YY_USER_ACTION \
  yylloc->first_line = yylloc->last_line = yylineno;\
  yylloc->first_column = yycolumn; yylloc->last_column = yycolumn + yyleng - 1;\
  yycolumn += yyleng;

#define YY_NO_UNISTD 

%}

%option noyywrap reentrant
%option bison-bridge bison-locations
%option yylineno 8bit

%%

[[:digit:]]+              { yylval->num = atof(yytext); return NUMBER;    }
[_[:alpha:]][_[:alnum:]]* { yylval->str = strdup(yytext); return STRING;  }

.                         { /* DO NOTHING */ }

%%

void yyerror(YYLTYPE* loc, void* scanner, const char* msg) {
  fprintf(stderr, "%d: %d: %s: %s\n", loc->first_line, loc->first_column, "ERROR", msg );
  exit(-1);
}
