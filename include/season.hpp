#ifndef SEASON_HPP_
#define SEASON_HPP_

#include <map>
#include <team.hpp>
#include <json.hpp>

using std::map;
using json = nlohmann::json;

class Season
{
public:
    Season(int year, json eloJSON);
    ~Season();
    
    int getYear();
    void setYear(int year);

    map<string,Team*> getTeams();
    void setTeams(map<string,Team*> teams);

private:
    int year;
    map<string,Team*> teams;
};

#endif