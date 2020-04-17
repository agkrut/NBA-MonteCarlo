#ifndef SIMULATEDGAME_HPP_
#define SIMULATEDGAME_HPP_

#include <team.hpp>
#include <random>

// #define N 100001 // Number of MC iterations (must be odd)
// #define K 30   // ELO weight of each game
// #define A 20   // Home-court advantage constant

extern int K;
extern int A;
extern int N;

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
    
    double getWinTeamOldELO();
    void setWinTeamOldELO(double winTeamOldELO);

    double getWinTeamNewELO();
    void setWinTeamNewELO(double winTeamNewELO);

    double getLoseTeamOldELO();
    void setLoseTeamOldELO(double loseTeamOldELO);

    double getLoseTeamNewELO();
    void setLoseTeamNewELO(double loseTeamNewELO);

    int getWinTeamSimWins();
    void setWinTeamSimWins(int winTeamSimWins);

    int getWinTeamSimLosses();
    void setWinTeamSimLosses(int winTeamSimLosses);

    void simulateGame();

private:
    Team* homeTeam;
    Team* roadTeam;
    Team* winner;
    Team* loser;
    double winTeamOldELO;
    double winTeamNewELO;
    double loseTeamOldELO;
    double loseTeamNewELO;
    int winTeamSimWins;
    int winTeamSimLosses;
};

#endif