#ifndef SIMULATEDGAME_HPP_
#define SIMULATEDGAME_HPP_

#include <team.hpp>

class SimulatedGame
{
public:
    SimulatedGame(Team* homeTeam, Team* roadTeam);
    SimulatedGame();
    ~SimulatedGame();
    
    Team* getHomeTeam();
    void setHomeTeam(Team* homeTeam);
    
    Team* getRoadTeam();
    void setRoadTeam(Team* roadTeam);

    Team* getWinner();
    void setWinner(Team* winner);

    Team* getLoser();
    void setLoser(Team* loser);
    
    void simulateGame();

private:
    Team* homeTeam;
    Team* roadTeam;
    Team* winner;
    Team* loser;
};

#endif