#ifndef PLAYOFFS_HPP_
#define PLAYOFFS_HPP_

#include <series.hpp>

using json = nlohmann::json;
using std::map;
using std::string;

class Playoffs
{
public:
    Playoffs(json playoffsJSON, map<string,Team*> teams);
    ~Playoffs();

    vector<Series*> getWesternConference();
    void setWesternConference(vector<Series*> westernConference);

    vector<Series*> getEasternConference();
    void setEasternConference(vector<Series*> easternConference);

    void simulate();

private:
    vector<Series*> westernConference;
    vector<Series*> easternConference;
    Series* finals;

    map<bool,pair<int,Team*>> calculateSeeding(pair<int,Team*> team1, pair<int,Team*> team2);
    void simulateRound1();
    void simulateRound2();
    void simulateRound3();
    void simulateFinals();
};

#endif