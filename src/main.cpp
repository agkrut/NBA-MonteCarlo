#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <json.hpp>
#include <season.hpp>
#include <playoffs.hpp>

using std::string;
using std::map;
using json = nlohmann::json;
using std::cerr;
using std::cout;
using std::endl;

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

void deallocateSeasons(vector<Season*>& seasons) {
    for (size_t i = 0; i < seasons.size(); i++)
        delete seasons.at(i);
}

void deallocatePostseasons(vector<Playoffs*>& playoffs) {
    for (size_t i = 0; i < playoffs.size(); i++)
        delete playoffs.at(i);
}

int K;
int A;
int N;

int main(int argc, char** argv) {
    
    if (argc < 4) {
        cerr << "[Error] Need to supply three arguments: ./exec [K] [A] [N]" << endl;
        exit(-1);
    }
    
    K = atoi(argv[1]);
    A = atoi(argv[2]);
    N = atoi(argv[3]);

    cout << "Parameters: K=" << K << " A=" << A << " N=" << N << endl;

    vector<Season*> seasons;
    vector<Playoffs*> postseasons;

    for (int i = FIRST_SEASON; i <= LAST_SEASON; i++) {
        string dir = "../data/season" + std::to_string(i) + "/clean/";
        json eloJSON = readTeamsJSON(dir, i);
        json playoffsJSON = readPlayoffsJSON(dir, i);

        Season* season = new Season(i, eloJSON, dir);
        Playoffs* playoffs = new Playoffs(playoffsJSON, season->getTeams());

        seasons.push_back(season);
        postseasons.push_back(playoffs);

        playoffs->simulate();
    }

    deallocateSeasons(seasons);
    deallocatePostseasons(postseasons);
}