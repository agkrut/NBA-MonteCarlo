#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <json.hpp>
#include <team.hpp>
#include <season.hpp>

using std::string;
using std::cout;
using std::endl;
using std::map;
using json = nlohmann::json;

#define FIRST_SEASON 2012
#define LAST_SEASON  2018

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

int main() {

    vector<Season*> seasons;

    for (int i = FIRST_SEASON; i < LAST_SEASON; i++) {
        string dir = "../data/season" + std::to_string(i) + "/clean/";
        json eloJSON = readTeamsJSON(dir, i);
        json playoffsJSON = readPlayoffsJSON(dir, i);

        Season* newSeason = new Season(i, eloJSON, playoffsJSON);
    }

}