#include <simgame.hpp>

SimulatedGame::SimulatedGame(Team* homeTeam, Team* roadTeam) {
    this->homeTeam = homeTeam;
    this->roadTeam = roadTeam;
    this->winner = nullptr;
    this->loser = nullptr;
    this->winTeamOldELO = -1;
    this->loseTeamOldELO = -1;
    this->winTeamNewELO = -1;
    this->loseTeamNewELO = -1;
    this->winTeamSimWins = 0;
    this->winTeamSimLosses = 0;
}
SimulatedGame::SimulatedGame() {
    this->homeTeam = nullptr;
    this->roadTeam = nullptr;
    this->winner = nullptr;
    this->loser = nullptr;
    this->winTeamOldELO = -1;
    this->loseTeamOldELO = -1;
    this->winTeamNewELO = -1;
    this->loseTeamNewELO = -1;
    this->winTeamSimWins = 0;
    this->winTeamSimLosses = 0;
}
SimulatedGame::~SimulatedGame() {}

Team* SimulatedGame::getHomeTeam() {
    return this->homeTeam;
}
void SimulatedGame::setHomeTeam(Team* homeTeam) {
    this->homeTeam = homeTeam;
}

Team* SimulatedGame::getRoadTeam() {
    return this->roadTeam;
}
void SimulatedGame::setRoadTeam(Team* roadTeam) {
    this->roadTeam = roadTeam;
}

Team* SimulatedGame::getWinner() {
    return this->winner;
}
void SimulatedGame::setWinner(Team* winner) {
    this->winner = winner;
}

Team* SimulatedGame::getLoser() {
    return this->loser;
}
void SimulatedGame::setLoser(Team* loser) {
    this->loser = loser;
}

double SimulatedGame::getWinTeamOldELO() {
    return this->winTeamOldELO;
}
void SimulatedGame::setWinTeamOldELO(double winTeamOldELO) {
    this->winTeamOldELO = winTeamOldELO;
}

double SimulatedGame::getWinTeamNewELO() {
    return this->winTeamNewELO;
}
void SimulatedGame::setWinTeamNewELO(double winTeamNewELO) {
    this->winTeamNewELO = winTeamNewELO;
}

double SimulatedGame::getLoseTeamOldELO() {
    return this->loseTeamOldELO;
}
void SimulatedGame::setLoseTeamOldELO(double loseTeamOldELO) {
    this->loseTeamOldELO = loseTeamOldELO;
}

double SimulatedGame::getLoseTeamNewELO() {
    return this->loseTeamNewELO;
}
void SimulatedGame::setLoseTeamNewELO(double loseTeamNewELO) {
    this->loseTeamNewELO = loseTeamNewELO;
}

int SimulatedGame::getWinTeamSimWins() {
    return this->winTeamSimWins;
}
void SimulatedGame::setWinTeamSimWins(int winTeamSimWins) {
    this->winTeamSimWins = winTeamSimWins;
}

int SimulatedGame::getWinTeamSimLosses() {
    return this->winTeamSimLosses;
}
void SimulatedGame::setWinTeamSimLosses(int winTeamSimLosses) {
    this->winTeamSimLosses = winTeamSimLosses;
}

#include <iostream>
void SimulatedGame::simulateGame() {
    double homeTeamELO = this->homeTeam->getPsELO().back();
    double roadTeamELO = this->roadTeam->getPsELO().back();
    double probabilityHomeTeamWins = 1.0 / (1 + pow(10, ((roadTeamELO-homeTeamELO-A)/400)));
    double probabilityRoadTeamWins = 1.0 / (1 + pow(10, ((homeTeamELO-roadTeamELO+A)/400)));

    std::random_device rd;
    std::default_random_engine generator(rd()); // rd() provides a random seed
    std::uniform_real_distribution<double> distribution(0.01, 1.00);
    
    int homeTeamWinCnt = 0;
    int roadTeamWinCnt = 0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double randNum = srand(generator);
        std::cout<<omp_get_thread_num() << ": " << randNum<< endl;
        if (randNum <= probabilityHomeTeamWins)
            #pragma omp critical
            {
                homeTeamWinCnt++;
            }
        else
            #pragma omp critical
            {
                roadTeamWinCnt++;
            }
    }

    if (homeTeamWinCnt > roadTeamWinCnt) {
        this->winner = this->homeTeam;
        this->loser = this->roadTeam;
        
        this->winTeamOldELO = homeTeamELO;
        this->loseTeamOldELO = roadTeamELO;
        
        homeTeamELO += K*(1-probabilityHomeTeamWins);
        roadTeamELO += K*(0-probabilityRoadTeamWins);
        
        this->winTeamNewELO = homeTeamELO;
        this->loseTeamNewELO = roadTeamELO;

        this->winTeamSimWins = homeTeamWinCnt;
        this->winTeamSimLosses = roadTeamWinCnt;
    }
    else {
        this->winner = this->roadTeam;
        this->loser = this->homeTeam;

        this->winTeamOldELO = roadTeamELO;
        this->loseTeamOldELO = homeTeamELO;

        homeTeamELO += K*(0-probabilityHomeTeamWins);
        roadTeamELO += K*(1-probabilityRoadTeamWins);

        this->winTeamNewELO = roadTeamELO;
        this->loseTeamNewELO = homeTeamELO;
        
        this->winTeamSimWins = roadTeamWinCnt;
        this->winTeamSimLosses = homeTeamWinCnt;
    }
    this->homeTeam->addPsELO(homeTeamELO);
    this->roadTeam->addPsELO(roadTeamELO);
}