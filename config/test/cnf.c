#include <stdio.h>

#include <config.tab.h>
#include <config.h>

int main(int argc, char* argv[]){
  yyscan_t scanner;
  yylex_init(&scanner);
  yyparse(scanner);
  yylex_destroy(&scanner);
  return 0;
}
