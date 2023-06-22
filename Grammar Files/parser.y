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

%token ADOPT FEED PLAY SLEEP STATUS TEACH PERFORM BURY SAVE LOAD
%token <str> IDENTIFIER

%%

program: command_list {}
        ;

command_list: command ';' command_list {}
            | command ';' {}
            ;

command: adopt { }
       | feed { }
       | play {}
       | sleep {}
       | status {}
       | teach {}
       | perform {}
       | save {}
       | load {}
       | bury {}
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

bury: BURY IDENTIFIER {
          Pet* pet = petMap.lookup(std::string($2));
          if (pet == nullptr) {
              yyerror("Pet does not exist");
          } else {
              if(pet->isAlive() == false) {
                  petMap.bury(std::string($2));
              } else {
                  std::cout << "Pet " << std::string($2) << " is still alive. Can't bury it." << std::endl;
              }
          }
          delete $2;
      }

save: SAVE {
    petMap.save();
        }

load: LOAD {
    petMap.load("pets.txt");
}




%%

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
}

int parser_main(int argc, char **argv) {
      if (argc > 1) {
              FILE *file = fopen(argv[1], "r");
              if (!file) {
                  std::cerr << "Error: could not open file " << std::endl;
                  return 1;
              }
              yyin = file;
      }
      yyparse();
      return 0;
}