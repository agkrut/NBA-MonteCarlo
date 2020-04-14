#include <simgame.hpp>

SimulatedGame::SimulatedGame(Team* homeTeam, Team* roadTeam) {
    this->homeTeam = homeTeam;
    this->roadTeam = roadTeam;
}
SimulatedGame::SimulatedGame() {
    this->homeTeam = nullptr;
    this->roadTeam = nullptr;
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