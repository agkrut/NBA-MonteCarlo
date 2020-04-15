#include <team.hpp>
#include <iostream>
Team::Team(json j, string dataDir) {
    this->city = j["city"];
    this->tricode = j["tricode"];
    this->name = j["nickname"];
    this->teamID = j["teamId"];
    this->rsWins = 0;
    this->rsLosses = 0;
    for (auto& s: j["elo"])
        this->rsELO.push_back(s);
    json sched = this->readScheduleJSON(dataDir)["games"];
    for (json::iterator it = sched.begin(); it != sched.end(); ++it) {
        if ((*it)["home_team_score"] > (*it)["visitor_team_score"]) {
            if ((*it)["home_team"]["abbreviation"] == this->tricode)
                this->rsWins++;
            else
                this->rsLosses++;
        }
        else {
            if ((*it)["visitor_team"]["abbreviation"] == this->tricode)
                this->rsWins++;
            else
                this->rsLosses++;            
        }
    }
}
Team::~Team() {}

string Team::getCity() {
    return this->city;
}
void Team::setCity(string city) {
    this->city = city;
}

string Team::getName() {
    return this->name;
}
void Team::setName(string name) {
    this->name = name;
}

string Team::getTricode() {
    return this->tricode;
}
void Team::setTricode(string tricode) {
    this->tricode = tricode;
}

vector<double> Team::getRsELO() {
    return this->rsELO;
}
void Team::setRsELO(vector<double> rsELO) {
    this->rsELO = rsELO;
}

double Team::getCurrELO() {
    return this->currELO;
}
void Team::setCurrELO(double currELO) {
    this->currELO = currELO;
}

string Team::getTeamID() {
    return this->teamID;
}
void Team::setTeamID(string teamID) {
    this->teamID = teamID;
}

int Team::getRsWins() {
    return this->rsWins;
}
void Team::setRsWins(int rsWins) {
    this->rsWins = rsWins;
}

int Team::getRsLosses() {
    return this->rsLosses;
}
void Team::setRsLosses(int rsLosses) {
    this->rsLosses = rsLosses;
}

json Team::readScheduleJSON(string dir) {
    string playoffsPath = dir + "schedules/" + this->tricode + ".json";
    std::ifstream ss(playoffsPath);
    json scheduleJSON;
    ss >> scheduleJSON;
    ss.close();
    return scheduleJSON;
}