#include "../Header Files/Pet.h"
#include <iostream>
#include <chrono>
#include <sstream>

#include "../Header Files/Pet.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <atomic>

Pet::Pet() : name(""), type(""), hunger(0), happiness(0), tiredness(0), alive(true), stopThread(false) {
    startPetThread();
}

Pet::Pet(string _type, string _name) : name(_name), type(_type), hunger(0), happiness(10), tiredness(0), alive(true), stopThread(false) {
    startPetThread();
}


Pet::~Pet() {
    stop();
    if (petThread.joinable()) {
        petThread.join();
    }
}

void draw_happy_cat(){
    cout<<( "  /\\____/\\\n( =^ . ^ =)\n (“) (“) _/\n");
}

void draw_happy_dog(){
    cout<<( "\t————\n\tU^ᴥ^U\n     \\_( u   u)\n");

}

void draw_sad_cat(){
    cout<<( "  /\\____/\\\n( =- . - =)\n (“) (“) _/\n");
}

void draw_sad_dog(){
    cout<<( "\t————\n\tU -ᴥ-U\n     \\_( u   u)\n");

}

void draw_dead_cat(){
    cout<<( "  /\\____/\\\n( =x . x =)\n (“) (“) _/\n");
}

void draw_dead_dog(){
    cout<<( "\t————\n\tU xᴥxU\n     \\_( u   u)\n");

}
void Pet::stop() {
    stopThread = true;
}

void Pet::feed() {
    lock_guard<mutex> lock(petMutex);
    if (alive) {
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
    lock_guard<mutex> lock(petMutex);
    if (alive) {
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
    lock_guard<mutex> lock(petMutex);
    if (alive) {
        tiredness -= 100;
        if (tiredness < 0) {
            tiredness = 0;
        }
    }
    else {
        cout << name << " is dead.\n";
    }
}

void Pet::teach(const string& trick) {
    lock_guard<mutex> lock(petMutex);
    tricks.insert(trick);
    cout << name << " has learned to " << trick << "!" << endl;
}

void Pet::perform(const string& trick) {
    lock_guard<mutex> lock(petMutex);
    if (tricks.count(trick) > 0) {
        cout << name << " is performing " << trick << "!" << endl;
        happiness += 5;
        tiredness += 10;
        if (happiness > 100) happiness = 100;
        if (tiredness > 100) tiredness = 100;
    } else {
        cout << name << " doesn't know how to " << trick << "!" << endl;
    }
}

void Pet::status() {
    lock_guard<mutex> lock(petMutex);
    if (alive) {
        cout << "Name: " << name << ", Type: " << type << ", Hunger: " << hunger << ", Happiness: " << happiness
             << ", Tiredness: " << tiredness << endl;

        if(type =="cat"){
            if(happiness >30){
                draw_happy_cat();
            }else{
                draw_sad_cat();
            }
        }
        else if(type == "dog"){
            if(happiness>30){
                draw_happy_dog();
            }else{
                draw_sad_dog();
            }
        }
    }

    else {
        cout << name << " is dead. Bury them.\n";
    }
}

void Pet::startPetThread() {
    petThread = thread([this]() {
        int counter = 0;
        while (!stopThread) {
            this_thread::sleep_for(chrono::seconds(1));
            {
                lock_guard<mutex> lock(petMutex);
                counter++;
                if (counter % 10 == 0) {
                    hunger++;
                    tiredness++;
                }
                checkIsAlive();
            }
        }
    });
}

void Pet::checkIsAlive() {
    if(stopThread) {
        return;
    }
    if (hunger > 50) {
        cout << "Pet " << name << " has died of hunger." << endl;
        alive = false;
        if(type == "cat"){
            draw_dead_cat();
        }else if(type == "dog"){
            draw_dead_dog();
        }
        stop();
        return;
    }
    if (tiredness > 100) {
        cout << "Pet " << name << " has died of tiredness." << endl;
        alive = false;
        if(type == "cat"){
            draw_dead_cat();
        }else if(type == "dog"){
            draw_dead_dog();
        }
        stop();
        return;
    }
}

std::string Pet::saveState() const {
    return name + "," + type + "," + std::to_string(hunger) + "," +
           std::to_string(happiness) + "," + std::to_string(tiredness) + "\n";
}

void Pet::loadState(const string& state) {
    istringstream iss(state);
    string token;

    if (getline(iss, token, ',')) name = token;
    if (getline(iss, token, ',')) type = token;
    if (getline(iss, token, ',')) hunger = stoi(token);
    if (getline(iss, token, ',')) happiness = stoi(token);
    if (getline(iss, token, ',')) tiredness = stoi(token);
}

const string &Pet::getName() const {
    return name;
}

void Pet::setName(const string &name) {
    Pet::name = name;
}

const string &Pet::getType() const {
    return type;
}

void Pet::setType(const string &type) {
    Pet::type = type;
}

int Pet::getHunger() const {
    return hunger;
}

void Pet::setHunger(int hunger) {
    Pet::hunger = hunger;
}

int Pet::getHappiness() const {
    return happiness;
}

void Pet::setHappiness(int happiness) {
    Pet::happiness = happiness;
}

int Pet::getTiredness() const {
    return tiredness;
}

void Pet::setTiredness(int tiredness) {
    Pet::tiredness = tiredness;
}

const unordered_set<std::string> &Pet::getTricks() const {
    return tricks;
}

void Pet::setTricks(const unordered_set<std::string> &tricks) {
    Pet::tricks = tricks;
}

bool Pet::isAlive() const {
    return alive;
}

