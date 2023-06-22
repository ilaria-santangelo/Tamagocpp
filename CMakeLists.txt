cmake_minimum_required(VERSION 3.25)
project(Tamagocpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tamagocpp "Source Files/main.cpp" "Source Files/Pet.cpp" "Grammar Files/parser.tab.cpp" "Grammar Files/lexer.flex.cpp" "Source Files/SymbolTable.cpp")
