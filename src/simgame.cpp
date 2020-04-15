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
    // for now set home as win and road as loss
    this->winner = this->homeTeam;
    this->loser = this->roadTeam;
}