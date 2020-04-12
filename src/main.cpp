#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <json.hpp>
#include <team.hpp>

using std::string;
using std::cout;
using std::endl;
using json = nlohmann::json;

int main() {

    int currSeason = 2012;
    int lastSeason = 2018;

    for (;currSeason <= lastSeason; currSeason++) {
        string dir = "../data/season" + std::to_string(currSeason) + "/clean/"; 
        string eloPath = dir + "elo.json";
        string playoffsPath = dir + "playoffs.json";

        std::ifstream es(eloPath);
        json eloJSON;
        es >> eloJSON;
        es.close();

        std::ifstream ps(playoffsPath);
        json playoffsJSON;
        ps >> playoffsJSON;
        ps.close();

    }
}