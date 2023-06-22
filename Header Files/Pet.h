#ifndef PET_H
#define PET_H

#include <string>
#include <unordered_set>
#include <thread>
#include <mutex>

#include <map>


using namespace std;


struct Item {
    string type;  // toy, treat, accessory
    int effect;   // effect on pet attributes

};

class Pet {
private:
    string name;
    string type;
    int hunger;
    int happiness;
    int tiredness;
    bool alive;
    bool stopThread;
    thread petThread;
    mutex petMutex;

    int experience;
    int level;

    unordered_set<string> tricks;
    std::map<string, Item> inventory;


public:
    Pet();
    Pet(string _type, string _name);
    ~Pet();

    void feed(int amount);
    void play();
    void sleep();
    void teach(const string& trick);
    void perform(const string& trick);
    void status();
    void stop();

    void startPetThread();
    void checkIsAlive();
    string saveState() const;
    void loadState(const string& state);

    const string &getName() const;
    const string &getType() const;
    int getHunger() const;
    int getHappiness() const;
    int getTiredness() const;
    const unordered_set<std::string> &getTricks() const;
    void setTricks(const unordered_set<std::string> &tricks);
    bool isAlive() const;

    double getHungerRate() const;
    double getTirednessRate() const;
    double getExperienceRate() const;
    void calculateLevel();

    void addItemToInventory(const string& item_name, const Item& item);
    void useItemFromInventory(const string& item_name);

};

#endif
