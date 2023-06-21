#include <string>
#include "Pet.h"

using namespace std;

struct SymbolTableEntry {
    string name;
    Pet* pet;
    SymbolTableEntry* next;
};

class SymbolTable {
private:
    SymbolTableEntry* head;
public:
    SymbolTable() : head(nullptr) {}

    void insert(string name, Pet* pet) {
        SymbolTableEntry* newEntry = new SymbolTableEntry;
        newEntry->name = name;
        newEntry->pet = pet;
        newEntry->next = head;
        head = newEntry;
    }

    Pet* lookup(string name) {
        SymbolTableEntry* entry = head;
        while (entry != nullptr) {
            if (entry->name == name) {
                return entry->pet;
            }
            entry = entry->next;
        }
        return nullptr;
    }

    ~SymbolTable() {
        while (head != nullptr) {
            SymbolTableEntry* next = head->next;
            delete head;
            head = next;
        }
    }
};
