#pragma once

#include <string>
#include <thread>
#include <unordered_set>
#include <mutex>

using namespace std;

class Pet {
public:
    Pet();
    Pet(string name, string type);
    ~Pet();

    void feed(int amount);
    void play();
    void sleep();
    void status();
    void teach(const std::string& trick);
    void perform(const std::string& trick);
    string saveState() const;
    void loadState(const string& state);

    const string &getName() const;

    void setName(const string &name);

    const string &getType() const;

    void setType(const string &type);

    int getHunger() const;

    void setHunger(int hunger);

    int getHappiness() const;

    void setHappiness(int happiness);

    int getTiredness() const;

    void setTiredness(int tiredness);

    const unordered_set<std::string> &getTricks() const;

    void setTricks(const unordered_set<std::string> &tricks);

    bool isAlive() const;

private:
    string name;
    string type;
    int hunger;
    int happiness;
    int tiredness;
    bool alive;
    thread petThread;
    unordered_set<std::string> tricks;
    mutex petMutex;
    atomic<bool> stopThread;

    void startPetThread();
    void checkIsAlive();

    void stop();
};
