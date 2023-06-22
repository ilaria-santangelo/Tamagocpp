#pragma once

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include "Pet.h"

struct SymbolTableEntry {
    std::string name;
    Pet* pet;
    SymbolTableEntry* next;
};

class SymbolTable {
private:
    SymbolTableEntry* head;

public:
    SymbolTable();
    void insert(std::string name, Pet* pet);
    Pet* lookup(std::string name);
    void save();
    void load(const std::string& filename);
    void clear();
    void bury(std::string name);
    ~SymbolTable();
};

#endif  // SYMBOLTABLE_H
