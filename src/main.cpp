#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <json.hpp>
#include <season.hpp>
#include <playoffs.hpp>

using std::string;
using std::cout;
using std::endl;
using std::map;
using json = nlohmann::json;

#define FIRST_SEASON 2012
#define LAST_SEASON  2012

json readTeamsJSON(string dir, int season) {
    string eloPath = dir + "elo.json";
    std::ifstream es(eloPath);
    json eloJSON;
    es >> eloJSON;
    es.close();
    return eloJSON;
}

json readPlayoffsJSON(string dir, int season) {
    string playoffsPath = dir + "playoffs.json";
    std::ifstream ps(playoffsPath);
    json playoffsJSON;
    ps >> playoffsJSON;
    ps.close();
    return playoffsJSON;
}

void deallocateSeasons(vector<Season*>& seasons) {
    for (size_t i = 0; i < seasons.size(); i++)
        delete seasons.at(i);
}

int main() {

    vector<Season*> seasons;

    for (int i = FIRST_SEASON; i <= LAST_SEASON; i++) {
        string dir = "../data/season" + std::to_string(i) + "/clean/";
        json eloJSON = readTeamsJSON(dir, i);
        json playoffsJSON = readPlayoffsJSON(dir, i);

        Season* season = new Season(i, eloJSON);
        Playoffs* playoffs = new Playoffs(playoffsJSON, season->getTeams());
    }

    deallocateSeasons(seasons);
}