#include "../Header Files/Pet.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <atomic>

#define BASE_HUNGER_RATE 10 // base hunger rate
#define BASE_TIREDNESS_RATE 20 // base tiredness rate
#define BASE_EXPERIENCE_RATE 10 // base experience rate
#define LEVEL_UP_THRESHOLD 100 // experience points needed to level up

Pet::Pet() : name(""), type(""), hunger(0), happiness(0), tiredness(0), alive(true), stopThread(false), experience(0), level(1) {
    startPetThread();
}

Pet::Pet(string _type, string _name) : name(_name), type(_type), hunger(0), happiness(10), tiredness(0), alive(true), stopThread(false), experience(0), level(1) {
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


void Pet::feed(int amount) {

    lock_guard<mutex> lock(petMutex);
    cout << "Fed " << amount << "\n";
    if (alive) {
        hunger -= amount * getHungerRate();
        if (hunger < 0) {
            hunger = 0;
        }
        happiness += 2 * getExperienceRate();
        if (happiness > 100) {
            happiness = 100;
        }
        experience += BASE_EXPERIENCE_RATE * getExperienceRate();
        calculateLevel();
    }
    else {
        cout << name << " is dead.";
    }
}

void Pet::play() {
    lock_guard<mutex> lock(petMutex);
    if (alive) {
        happiness += 5 * getExperienceRate();
        tiredness += 10 * getTirednessRate();
        if (happiness > 100) {
            happiness = 100;
        }
        experience += BASE_EXPERIENCE_RATE * getExperienceRate();
        calculateLevel();
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
        happiness += 5 * getExperienceRate();
        tiredness += 10 * getTirednessRate();
        if (happiness > 100) happiness = 100;
        if (tiredness > 100) tiredness = 100;
        experience += BASE_EXPERIENCE_RATE * getExperienceRate();
        calculateLevel();
    } else {
        cout << name << " doesn't know how to " << trick << "!" << endl;
    }
}

void Pet::status() {
    lock_guard<mutex> lock(petMutex);
    if (alive) {
        cout << "Name: " << name << ", Type: " << type << ", Hunger: " << hunger << ", Happiness: " << happiness
             << ", Tiredness: " << tiredness << ", Level: " << level << ", Experience: " << experience << endl;

        if (!inventory.empty()) {
            cout << "Inventory: " << endl;
            for (const auto& item : inventory) {
                cout << "- Name: " << item.first << ", Type: " << item.second.type << ", Effect: " << item.second.effect << endl;
            }
        } else {
            cout << "Inventory is empty." << endl;
        }
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
                    hunger += getHungerRate();
                    tiredness += getTirednessRate();
                    happiness -= (hunger / BASE_HUNGER_RATE);
                    happiness -= (tiredness / BASE_TIREDNESS_RATE);
                    checkIsAlive();
                }
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
    if (happiness < 0) {
        cout << "Pet " << name << " has died of sadness." << endl;
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

void Pet::calculateLevel() {
    if (experience >= LEVEL_UP_THRESHOLD * level) {
        level++;
        cout << "Pet " << name << " has leveled up to Level " << level << "!" << endl;
    }
}

std::string Pet::saveState() const {
    return name + "," + type + "," + std::to_string(hunger) + "," +
           std::to_string(happiness) + "," + std::to_string(tiredness) +
           std::to_string(level) + "," + std::to_string(experience) + "\n";
}

void Pet::loadState(const string& state) {
    istringstream iss(state);
    string token;

    if (getline(iss, token, ',')) name = token;
    if (getline(iss, token, ',')) type = token;
    if (getline(iss, token, ',')) hunger = stoi(token);
    if (getline(iss, token, ',')) happiness = stoi(token);
    if (getline(iss, token, ',')) tiredness = stoi(token);
    if (getline(iss, token, ',')) level = stoi(token);
    if (getline(iss, token, ',')) experience = stoi(token);
}

double Pet::getHungerRate() const {
    return BASE_HUNGER_RATE / static_cast<double>(level);
}

double Pet::getTirednessRate() const {
    return BASE_TIREDNESS_RATE / static_cast<double>(level);
}

double Pet::getExperienceRate() const {
    return BASE_EXPERIENCE_RATE / static_cast<double>(level);
}

const string &Pet::getName() const {
    return name;
}

const string &Pet::getType() const {
    return type;
}

int Pet::getHunger() const {
    return hunger;
}

int Pet::getHappiness() const {
    return happiness;
}


int Pet::getTiredness() const {
    return tiredness;
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

void Pet::addItemToInventory(const string& item_name, const Item& item) {
    inventory[item_name] = item;
    cout << name << " has received a new " << item.type << ": " << item_name << "!" << endl;

}


void Pet::useItemFromInventory(const string& item_name) {
    if (inventory.count(item_name) > 0) {
        Item item = inventory[item_name];
        if (item.type == "toy") {
            happiness += item.effect;
            tiredness += item.effect / 2;
            hunger += item.effect / 2;
        } else if (item.type == "treat") {
            happiness += item.effect;
            hunger -= item.effect;
        } else if (item.type == "accessory") {
            happiness += item.effect;
        }
        cout << name << " has used the " << item.type << ": " << item_name << "!" << endl;
        inventory.erase(item_name);  // remove item from inventory after use
    } else {
        cout << name << " doesn't have a " << item_name << " in their inventory." << endl;
    }
}
