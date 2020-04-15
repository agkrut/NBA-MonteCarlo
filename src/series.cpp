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
    for (int i = 0; i < this->games.size(); i++)
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

pair<int,Team*> Series::getWinnerPair() {
    if (this->seriesWinner == this->highSeedTeam)
        return std::make_pair(this->highSeedNum, this->highSeedTeam);
    else
        return std::make_pair(this->lowSeedNum, this->lowSeedTeam);
}

pair<int,Team*> Series::getLoserPair() {
    if (this->seriesLoser == this->highSeedTeam)
        return std::make_pair(this->highSeedNum, this->highSeedTeam);
    else
        return std::make_pair(this->lowSeedNum, this->lowSeedTeam);    
}

void Series::setHighSeed(pair<int,Team*> highSeed) {
    this->highSeedNum = highSeed.first;
    this->highSeedTeam = highSeed.second;
    this->games.at(0)->setHomeTeam(highSeed.second);
    this->games.at(1)->setHomeTeam(highSeed.second);
    this->games.at(2)->setRoadTeam(highSeed.second);
    this->games.at(3)->setRoadTeam(highSeed.second);
}
void Series::setLowSeed(pair<int,Team*> lowSeed) {
    this->lowSeedNum = lowSeed.first;
    this->lowSeedTeam = lowSeed.second;
    this->games.at(0)->setRoadTeam(lowSeed.second);
    this->games.at(1)->setRoadTeam(lowSeed.second);
    this->games.at(2)->setHomeTeam(lowSeed.second);
    this->games.at(3)->setHomeTeam(lowSeed.second);
}

void Series::simulateSeries() {
    // simulate games 1-4
    for (int i = 0; i < 4; i++) {
        SimulatedGame* currGame = this->games.at(i);
        currGame->simulateGame();
        if (this->highSeedTeam == currGame->getWinner()) // high seed wins
            this->highSeedWins++;
        else // low seed wins
            this->lowSeedWins++;
    }

    while (!(this->highSeedWins == 4 || this->lowSeedWins == 4)) {
        SimulatedGame* nextGame = nullptr;
        if (this->games.size() == 4 || this->games.size() == 6)
            nextGame = new SimulatedGame(this->highSeedTeam, this->lowSeedTeam);
        else
            nextGame = new SimulatedGame(this->lowSeedTeam, this->highSeedTeam);
        this->games.push_back(nextGame);
        nextGame->simulateGame();
        if (this->highSeedTeam == nextGame->getWinner()) // high seed wins
            this->highSeedWins++;
        else // low seed wins
            this->lowSeedWins++;
    }

    if (this->highSeedWins == 4) { // high seed wins series
        this->seriesWinner = this->highSeedTeam;
        this->seriesLoser = this->lowSeedTeam;
    }
    else { // low seed wins series
        this->seriesWinner = this->lowSeedTeam;
        this->seriesLoser = this->highSeedTeam;
    }
}