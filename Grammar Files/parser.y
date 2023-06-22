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

char* draw_dog();
char* draw_cat();

void executeCommand(string commandStr) {
    vector<string> commandParts = splitString(commandStr, ' ');  // split commandStr by space

    if (commandParts[0] == "adopt") {
        Pet* newPet = new Pet(commandParts[1], commandParts[2]);
        if (petMap.lookup(commandParts[2]) != nullptr) {
            yyerror("Pet with this name already exists");
        } else {
            petMap.insert(commandParts[2], newPet);
        }
    }
    else if (commandParts[0] == "feed") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            int amount = parseValue(commandParts[2]);  // this function should convert commandParts[2] to an integer (if it's a number) or fetch the variable value (if it's a variable name)
            pet->feed(amount);
        }
    }
    else if (commandParts[0] == "play") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            pet->play();
        }
    }
    else if (commandParts[0] == "sleep") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            pet->sleep();
        }
    }
    else if (commandParts[0] == "status") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            pet->status();
        }
    }
    else if (commandParts[0] == "teach") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            pet->teach(commandParts[2]);
        }
    }
    else if (commandParts[0] == "perform") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            pet->perform(commandParts[2]);
        }
    }
    else if (commandParts[0] == "bury") {
        Pet* pet = petMap.lookup(commandParts[1]);
        if (pet == nullptr) {
            yyerror("Pet does not exist");
        } else {
            if (pet->isAlive() == false) {
                petMap.bury(commandParts[1]);
            } else {
                std::cout << "Pet " << commandParts[1] << " is still alive. Can't bury it." << std::endl;
            }
        }
    }
    else {
        yyerror("Invalid command");
    }
}

int parseValue(string str) {
    // if str is a number, convert it to integer and return
    if (isdigit(str[0])) {
        return atoi(str.c_str());
    }
    // if str is a variable name, look it up in the variable map and return its value
    else {
        int* value = varMap.lookup(str);
        if (value == nullptr) {
            yyerror("Variable does not exist");
            return 0;
        }
        return *value;
    }
}


%}

%union {
    char* str;
    int num;
}

%token ADOPT FEED PLAY SLEEP STATUS TEACH PERFORM BURY SAVE LOAD NUMBER
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
       | load {}
       | bury {}
       | declaration {}
       | loop {}
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

loop: LOOP '(' INTEGER ',' '"' command_string '"' ')' {
     int loopCount = $3;
     string commandStr = $6;
     for(int i = 0; i < loopCount; i++) {
         executeCommand(commandStr);
     }
}

command_string: IDENTIFIER {
                    $$ = $1;
                }
              | IDENTIFIER IDENTIFIER {
                    $$ = (std::string($1) + " " + std::string($2)).c_str();
                    delete $1; delete $2;
                }
              | IDENTIFIER IDENTIFIER INTEGER {
                    $$ = (std::string($1) + " " + std::string($2) + " " + std::to_string($3)).c_str();
                    delete $1; delete $2;
                }
              | IDENTIFIER IDENTIFIER IDENTIFIER {
                    $$ = (std::string($1) + " " + std::string($2) + " " + std::string($3)).c_str();
                    delete $1; delete $2; delete $3;
                }
     ;

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