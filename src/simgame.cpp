#include <simgame.hpp>

SimulatedGame::SimulatedGame(Team* homeTeam, Team* roadTeam) {
    this->homeTeam = homeTeam;
    this->roadTeam = roadTeam;
    this->winner = nullptr;
    this->loser = nullptr;
}
SimulatedGame::SimulatedGame() {
    this->homeTeam = nullptr;
    this->roadTeam = nullptr;
    this->winner = nullptr;
    this->loser = nullptr;
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
    for (int i = 0; i < N; i++) {
        double randNum = distribution(generator);
        if (randNum <= probabilityHomeTeamWins)
            homeTeamWinCnt++;
        else
            roadTeamWinCnt++;
    }

    if (homeTeamWinCnt > roadTeamWinCnt) {
        this->winner = this->homeTeam;
        this->loser = this->roadTeam;
        homeTeamELO += K*(1-probabilityHomeTeamWins);
        roadTeamELO += K*(0-probabilityRoadTeamWins);
    }
    else {
        this->winner = this->roadTeam;
        this->loser = this->homeTeam;
        homeTeamELO += K*(0-probabilityHomeTeamWins);
        roadTeamELO += K*(1-probabilityRoadTeamWins);
    }
    this->homeTeam->addPsELO(homeTeamELO);
    this->roadTeam->addPsELO(roadTeamELO);
}