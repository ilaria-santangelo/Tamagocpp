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

struct VariableTableEntry {
    std::string name;
    int value;
    VariableTableEntry* next;
};

class SymbolTable {
private:
    SymbolTableEntry* head;
    VariableTableEntry* varHead = nullptr;

public:
    SymbolTable();
    void insert(std::string name, Pet* pet);
    Pet* lookup(std::string name);
    void save();
    void load(const std::string& filename);
    void clear();
    void bury(std::string name);
    void insertVariable(std::string name, int value);
    int* lookupVariable(std::string name);
    ~SymbolTable();
};

#endif  // SYMBOLTABLE_H
