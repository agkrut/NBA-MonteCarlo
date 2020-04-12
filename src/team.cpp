#include <team.hpp>

Team::Team(nlohmann::json j) {
    this->city = j["city"];
    this->tricode = j["tricode"];
    this->name = j["nickname"];
    this->teamID = j["teamId"];
    for (auto& s: j["elo"])
        this->rsELO.push_back(s);
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