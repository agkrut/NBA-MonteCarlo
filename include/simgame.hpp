#ifndef SIMULATEDGAME_HPP_
#define SIMULATEDGAME_HPP_

#include <team.hpp>

class SimulatedGame
{
public:
    SimulatedGame(Team* homeTeam, Team* awayTeam);
    SimulatedGame();
    ~SimulatedGame();
    
    Team* getHomeTeam();
    void setHomeTeam(Team* homeTeam);
    
    Team* getRoadTeam();
    void setRoadTeam(Team* roadTeam);

private:
    Team* homeTeam;
    Team* roadTeam;
};

#endif