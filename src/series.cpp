#include <series.hpp>

Series::Series(int round, Team* highSeedTeam, Team* lowSeedTeam, int highSeedNum, int lowSeedNum){
    this->round = round;
    
    this->highSeedTeam = highSeedTeam;
    this->lowSeedTeam = lowSeedTeam;

    this->highSeedNum = highSeedNum;
    this->lowSeedNum = lowSeedNum;

    this->seriesWinner = nullptr;
    this->seriesLoser = nullptr;

    this->highSeedWins = 0;
    this->lowSeedWins = 0;

    this->games.push_back(new SimulatedGame(highSeedTeam, lowSeedTeam));
    this->games.push_back(new SimulatedGame(highSeedTeam, lowSeedTeam));
    this->games.push_back(new SimulatedGame(lowSeedTeam, highSeedTeam));
    this->games.push_back(new SimulatedGame(lowSeedTeam, highSeedTeam));
}
Series::Series(int round) {
    this->round = round;

    this->highSeedTeam = nullptr;
    this->lowSeedTeam = nullptr;

    this->highSeedNum = -1;
    this->lowSeedNum = -1;

    this->seriesWinner = nullptr;
    this->seriesLoser = nullptr;

    this->highSeedWins = 0;
    this->lowSeedWins = 0;

    for (int i = 0; i < 4; i++)
        this->games.push_back(new SimulatedGame);
}
Series::~Series() {
    for (int i = 0; i < 7; i++)
        delete this->games.at(i);
}

int Series::getRound() {
    return this->round;
}
void Series::setRound(int round) {
    this->round = round;
}

int Series::getHighSeedNum() {
    return this->highSeedNum;
}
void Series::setHighSeedNum(int highSeedNum) {
    this->highSeedNum = highSeedNum;
}

int Series::getLowSeedNum() {
    return this->lowSeedNum;
}
void Series::setLowSeedNum(int lowSeedNum) {
    this->lowSeedNum = lowSeedNum;
}

Team* Series::getHighSeedTeam() {
    return this->highSeedTeam;
}
void Series::setHighSeedTeam(Team* highSeedTeam) {
    this->highSeedTeam = highSeedTeam;
}

Team* Series::getLowSeedTeam() {
    return this->lowSeedTeam;
}
void Series::setLowSeedTeam(Team* lowSeedTeam) {
    this->lowSeedTeam = lowSeedTeam;
}

Team* Series::getSeriesWinner() {
    return this->seriesWinner;
}
void Series::setSeriesWinner(Team* seriesWinner) {
    this->seriesWinner = seriesWinner;
}

Team* Series::getSeriesLoser() {
    return this->seriesLoser;
}
void Series::setSeriesLoser(Team* seriesLoser) {
    this->seriesLoser = seriesLoser;
}

int Series::getHighSeedWins() {
    return this->highSeedWins;
}
void Series::setHighSeedWins(int highSeedWins) {
    this->highSeedWins = highSeedWins;
}

int Series::getLowSeedWins() {
    return this->lowSeedWins;
}
void Series::setLowSeedWins(int lowSeedWins) {
    this->lowSeedWins = lowSeedWins;
}

vector<SimulatedGame*> Series::getGames() {
    return this->games;
}
void Series::setGames(vector<SimulatedGame*> games) {
    this->games = games;
}

Series* Series::getAdvance() {
    return this->advance;
}

void Series::setAdvance(Series* advance) {
    this->advance = advance;
}