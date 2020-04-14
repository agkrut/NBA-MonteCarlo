#ifndef SERIES_HPP_
#define SERIES_HPP_

#include <team.hpp>
#include <simgame.hpp>
#include <vector>

class Series
{
public:
    Series(int round, Team* highSeedTeam, Team* lowSeedTeam, int highSeedNum, int lowSeedNum);
    Series(int round);
    ~Series();

    int getRound();
    void setRound(int round);

    int getHighSeedNum();
    void setHighSeedNum(int highSeedNum);

    int getLowSeedNum();
    void setLowSeedNum(int lowSeedNum);

    Team* getHighSeedTeam();
    void setHighSeedTeam(Team* highSeedTeam);

    Team* getLowSeedTeam();
    void setLowSeedTeam(Team* lowSeedTeam);

    Team* getSeriesWinner();
    void setSeriesWinner(Team* seriesWinner);

    Team* getSeriesLoser();
    void setSeriesLoser(Team* seriesLoser);

    int getHighSeedWins();
    void setHighSeedWins(int highSeedWins);

    int getLowSeedWins();
    void setLowSeedWins(int lowSeedWins);

    vector<SimulatedGame*> getGames();
    void setGames(vector<SimulatedGame*> games);

    Series* getAdvance();
    void setAdvance(Series* advance);

private:
    int round;
    int highSeedNum;
    int lowSeedNum;
    Team* highSeedTeam;
    Team* lowSeedTeam;
    Team* seriesWinner;
    Team* seriesLoser;
    int highSeedWins;
    int lowSeedWins;
    vector<SimulatedGame*> games;
    Series* advance;
};

#endif