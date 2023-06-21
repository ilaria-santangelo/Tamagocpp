#pragma once

#include <string>
#include <unordered_map>
#include <thread>
#include <unordered_set>

using namespace std;

class Pet {
public:
    Pet(string name, string type);
    ~Pet();

    void feed();
    void play();
    void sleep();
    void status();
    void teach(const std::string& trick);
    void perform(const std::string& trick);

private:
    string name;
    string type;
    int hunger;
    int happiness;
    int tiredness;
    bool isAlive;
    thread petThread;
    std::unordered_set<std::string> tricks;

    void checkIsAlive();
};
