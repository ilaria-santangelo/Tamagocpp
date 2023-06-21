#include "../Header Files/Pet.h"
#include <iostream>
#include <chrono>

Pet::Pet(string _type, string _name) 
    : name(_name), type(_type), hunger(0), happiness(10), tiredness(0), isAlive(true) {
    petThread = thread([this]() {
        while (isAlive) {
            this_thread::sleep_for(chrono::seconds(1));
            hunger++;
            tiredness++;
            checkIsAlive();
        }
    });
}

Pet::~Pet() {
    if (petThread.joinable()) {
        petThread.join();
    }
}

void Pet::feed() {
    if (isAlive) {
        hunger -= 5;
        if (hunger < 0) {
            hunger = 0;
        }
        happiness += 2;
        if (happiness > 100) {
            happiness = 100;
        }
    }
    else {
        cout << name << " is dead.";
    }
}

void Pet::play() {
    if (isAlive) {
        happiness += 5;
        tiredness += 10;
        if (happiness > 100) {
            happiness = 100;
        }
    }
    else {
        cout << name << " is dead.";
    }
}

void Pet::sleep() {
    if (isAlive) {
        tiredness -= 100;
        if (tiredness < 0) {
            tiredness = 0;
        }
    }
    else {
        cout << name << " is dead.";
    }
}

void Pet::status() {
    cout << "Name: " << name << ", Type: " << type << ", Hunger: " << hunger << ", Happiness: " << happiness << ", Tiredness: " << tiredness << endl;
}

void Pet::checkIsAlive() {
    if (hunger > 50) {
        cout << name << " has died of hunger." << endl;
        isAlive = false;
    }
    if (tiredness > 100) {
       cout << name << " has died of tiredness." << endl;
        isAlive = false;
    }
}


