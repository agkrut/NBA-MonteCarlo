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
private:
    vector<Series*> westernConference;
    vector<Series*> easternConference;
    Series* finals;
};

#endif