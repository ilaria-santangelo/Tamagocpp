%{
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "../Header Files/Pet.h"
#include "../Header Files/SymbolTable.h"
#include <string>
#include <vector>
void yyerror(const char *s);
extern FILE *yyin;
extern int yylex();
SymbolTable petMap;

char* draw_dog();
char* draw_cat();

%}

%union {
    char* str;
    int num;
}

%token ADOPT FEED PLAY SLEEP STATUS TEACH PERFORM BURY SAVE LOAD NUMBER GIFT USE
%token <str> IDENTIFIER
%token <num> INTEGER

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
       | gift {}
       | use {}
       | load {}
       | bury {}
       | declaration {}
       ;

declaration: NUMBER IDENTIFIER '=' INTEGER {
    int* existingVar = petMap.lookupVariable(std::string($2));
    if (existingVar != nullptr) {
        yyerror("Variable with this name already exists");
    } else {
        petMap.insertVariable(std::string($2), $4);
    }
    delete $2;
}

adopt: ADOPT IDENTIFIER IDENTIFIER {
          Pet* newPet = new Pet(std::string($2), std::string($3));
          if (petMap.lookup(std::string($3)) != nullptr) {
            yyerror("Pet with this name already exists");
          } else {
            petMap.insert(std::string($3), newPet);
            if (std::string($2) == "dog") {
              std::cout << draw_dog();
            }
            else if (std::string($2) == "cat") {
              std::cout << draw_cat();
            }
          }
          delete $2; delete $3;
       }

feed: FEED IDENTIFIER INTEGER {
	    Pet* pet = petMap.lookup(std::string($2));
	    if (pet == nullptr) {
		yyerror("Pet does not exist");
	    } else {
		pet->feed($3);
	    }
	    delete $2;
	}
	| FEED IDENTIFIER IDENTIFIER {
	    Pet* pet = petMap.lookup(std::string($2));
	    int* value = petMap.lookupVariable(std::string($3));
	    if (pet == nullptr) {
		yyerror("Pet does not exist");
	    } else if (value == nullptr) {
		yyerror("Variable does not exist");
	    } else {
		pet->feed(*value);
	    }
	    delete $2; delete $3;
	}
;

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

gift: GIFT IDENTIFIER IDENTIFIER IDENTIFIER {
          Item newItem;
          newItem.type = std::string($3);
          newItem.effect = 10;
          Pet* pet = petMap.lookup(std::string($2));
          if (pet == nullptr) {
            yyerror("Pet does not exist");
          } else {
            pet->addItemToInventory(std::string($4), newItem);
          }
          delete $2; delete $3; delete $4;
       }


use: USE IDENTIFIER IDENTIFIER {
         Pet* pet = petMap.lookup(std::string($2));
         if (pet == nullptr) {
           yyerror("Pet does not exist");
         } else {
           pet->useItemFromInventory(std::string($3));
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

char* draw_dog() {
return "\t————\n\tU・ᴥ・U\n     \\_( u   u)\n";
}

char* draw_cat() {
    return "  /\\____/\\\n( = ・ᴥ・ =)\n (“) (“) _/\n";
}