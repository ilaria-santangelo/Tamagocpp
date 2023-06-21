%{
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "../Header Files/Pet.h"
#include "../Header Files/SymbolTable.h"
void yyerror(const char *s);
extern FILE *yyin;
extern int yylex();
SymbolTable petMap;
%}

%union {
  char* str;
}

%token ADOPT FEED PLAY SLEEP STATUS TEACH PERFORM
%token <str> IDENTIFIER

%%

program: command_list { /* do nothing */ }
        ;

command_list: command ';' command_list { /* do nothing */ }
            | command ';' { /* do nothing */ }
            ;

command: adopt { /* do nothing */ }
       | feed { /* do nothing */ }
       | play { /* do nothing */ }
       | sleep { /* do nothing */ }
       | status { /* do nothing */ }
       | teach { /* do nothing */ }
       | perform { /* do nothing */ }
       ;

adopt: ADOPT IDENTIFIER IDENTIFIER {
          Pet* newPet = new Pet(std::string($2), std::string($3));
          if (petMap.lookup(std::string($3)) != nullptr) {
            yyerror("Pet with this name already exists");
          } else {
            petMap.insert(std::string($3), newPet);
          }
          delete $2; delete $3;
       }

feed: FEED IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->feed();
         }
         delete $2;
      }
play: PLAY IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->play();
         }
         delete $2;
      }

sleep: SLEEP IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->sleep();
         }
         delete $2;
      }

status: STATUS IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->status();
         }
         delete $2;
      }


teach: TEACH IDENTIFIER IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->teach(std::string($3));
         }
         delete $2; delete $3;
      }

perform: PERFORM IDENTIFIER IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->perform(std::string($3));
         }
         delete $2; delete $3;
      }


%%

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
}

void parser_main() {
  yyparse();
}