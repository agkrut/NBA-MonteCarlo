#include <season.hpp>
#include <iostream>

Season::Season(int year, json eloJSON) {
    this->year = year;
    for (auto& s: eloJSON) {
        string tricode = s["tricode"];
        this->teams[tricode] = new Team((json) s);
    }
}
Season::~Season() {}

int Season::getYear() {
    return this->year;
}
void Season::setYear(int year) {
    this->year = year;
}

map<string,Team*> Season::getTeams() {
    return this->teams;
}
void Season::setTeams(map<string,Team*> teams) {
    this->teams = teams;
}
