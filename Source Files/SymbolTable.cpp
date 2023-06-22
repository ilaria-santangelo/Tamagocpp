#include "../Header Files/SymbolTable.h"
#include <fstream>
#include <iostream>

SymbolTable::SymbolTable() : head(nullptr) {}

void SymbolTable::insert(std::string name, Pet* pet) {
    SymbolTableEntry* newEntry = new SymbolTableEntry;
    newEntry->name = name;
    newEntry->pet = pet;
    newEntry->next = head;
    head = newEntry;
}

Pet* SymbolTable::lookup(std::string name) {
    SymbolTableEntry* entry = head;
    while (entry != nullptr) {
        if (entry->name == name) {
            return entry->pet;
        }
        entry = entry->next;
    }
    return nullptr;
}

void SymbolTable::save() {
    std::ofstream file("pets.txt");
    if (file.is_open()) {
        SymbolTableEntry* entry = head;
        while (entry != nullptr) {
            file << entry->name << "\n";
            file << entry->pet->saveState();
            entry = entry->next;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for saving";
    }
}

void SymbolTable::load(const std::string& filename) {
    clear();
    std::cout << "Loading...\n";
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::string petName = line;
            if(std::getline(file, line)) {
                Pet* pet = new Pet();
                pet->loadState(line);
                insert(petName, pet);
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for loading";
    }
}

void SymbolTable::clear() {
    SymbolTableEntry* current = head;
    while (current != nullptr) {
        SymbolTableEntry* toDelete = current;
        current = current->next;
        delete toDelete->pet;
        delete toDelete;
    }
    head = nullptr;
}

void SymbolTable::bury(std::string name) {
    SymbolTableEntry* entry = head;
    SymbolTableEntry* prev = nullptr;
    while (entry != nullptr) {
        if (entry->name == name) {
            if (entry->pet->isAlive() == false) {
                if (prev == nullptr) {
                    head = entry->next;
                } else {
                    prev->next = entry->next;
                }

                // Delete the entry
                delete entry->pet;
                delete entry;
                std::cout << "The pet " << name << " has been buried.\n";
                return;
            } else {
                std::cout << "The pet " << name << " is still alive and cannot be buried.\n";
                return;
            }
        }
        prev = entry;
        entry = entry->next;
    }
    std::cout << "The pet " << name << " does not exist.\n";
}

SymbolTable::~SymbolTable() {
    while (head != nullptr) {
        SymbolTableEntry* next = head->next;
        delete head->pet;
        delete head;
        head = next;
    }
}

void SymbolTable::insertVariable(std::string name, int value) {
    VariableTableEntry* newEntry = new VariableTableEntry;
    newEntry->name = name;
    newEntry->value = value;
    newEntry->next = varHead;
    varHead = newEntry;
}

int *SymbolTable::lookupVariable(std::string name) {
    VariableTableEntry* entry = varHead;
    while (entry != nullptr) {
        if (entry->name == name) {
            return &(entry->value);
        }
        entry = entry->next;
    }
    return nullptr;
}


