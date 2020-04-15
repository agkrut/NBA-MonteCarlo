#include <season.hpp>

Season::Season(int year, json eloJSON) {
    this->year = year;
    
    for (json::iterator it = eloJSON.begin(); it != eloJSON.end(); ++it) {
        string tricode = (string) it.key();
        this->teams[tricode] = new Team(*it);
    }
}
Season::~Season() {
    map<string,Team*>::iterator it = this->teams.begin();
    for (; it != this->teams.end(); ++it)
        delete it->second;
}

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
