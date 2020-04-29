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
    this->seeds = new unsigned int[T];
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
    this->seeds = new unsigned int[T];
}
SimulatedGame::~SimulatedGame() {
    delete[] this->seeds;
}

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

unsigned int* SimulatedGame::getSeeds() {
    return this->seeds;
}
void SimulatedGame::setSeeds(unsigned int* seeds) {
    this->seeds = seeds;
}

double SimulatedGame::getTime() {
    return this->time;
}
void SimulatedGame::setTime(double time) {
    this->time = time;
}

void SimulatedGame::seedThreads() {
    // #pragma omp parallel num_threads(T)
    // {
    //     uint threadID = omp_get_thread_num();
        
    //     //create seed on thread using current time
    //     unsigned int seed = (unsigned) std::time(NULL);
        
    //     //munge the seed using our thread number so that each thread has its
    //     //own unique seed, therefore ensuring it will generate a different set of numbers
    //     this->seeds[threadID] = (seed & 0xFFFFFFF0) | (threadID + 1);
    // }
}
  
void SimulatedGame::simulateGame() {
    // get the start time
    auto start = high_resolution_clock::now(); 

    double homeTeamELO = this->homeTeam->getPsELO().back();
    double roadTeamELO = this->roadTeam->getPsELO().back();
    double probabilityHomeTeamWins = 1.0 / (1 + pow(10, ((roadTeamELO-homeTeamELO-A)/400)));
    double probabilityRoadTeamWins = 1.0 / (1 + pow(10, ((homeTeamELO-roadTeamELO+A)/400)));
    

    // Parallel type 1 - arrays (false sharing)
    int homeTeamWinCnt = 0;
    int roadTeamWinCnt = 0;
    std::random_device rd;
    std::default_random_engine generator(rd()); // rd() provides a random seed
    std::uniform_real_distribution<double> distribution(0.01, 1.00);
    int* homeWinsArr = new int[T];
    int* roadWinsArr = new int[T];
    for (int i = 0; i < T; i++) {
        homeWinsArr[i] = 0;
        roadWinsArr[i] = 0;
    }
    #pragma omp parallel num_threads(T)
    {
        uint threadID = omp_get_thread_num();
        for (int i = 0; i < N/T; i++) {
            double randNum = distribution(generator);
            if (randNum <= probabilityHomeTeamWins)
                homeWinsArr[threadID]++;
            else
                roadWinsArr[threadID]++;     
        }
    }
    for (int i = 0; i < T; i++) {
        homeTeamWinCnt += homeWinsArr[i];
        roadTeamWinCnt += roadWinsArr[i];
    }
    delete[] homeWinsArr;
    delete[] roadWinsArr;

    // Parallelism type 2 - local variables
    // int homeTeamWinCnt = 0;
    // int roadTeamWinCnt = 0;

    // std::random_device rd;
    // std::default_random_engine generator(rd()); // rd() provides a random seed
    // std::uniform_real_distribution<double> distribution(0.01, 1.00);

    // #pragma omp parallel num_threads(T)
    // {
    //     uint threadID = omp_get_thread_num();
        
    //     int localHomeTeamWinCnt = 0;
    //     int localRoadTeamWinCnt = 0;
    //     for (int i = 0; i < N/T; i++) {
    //         double randNum = distribution(generator);
    //         if (randNum <= probabilityHomeTeamWins)
    //             localHomeTeamWinCnt++;
    //         else
    //             localRoadTeamWinCnt++;        
    //     }
    
    //     #pragma omp critical
    //     {
    //         homeTeamWinCnt += localHomeTeamWinCnt;
    //         roadTeamWinCnt += localRoadTeamWinCnt;
    //     }
    // }

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

    // stop timing
    auto stop = high_resolution_clock::now();
    this->time = duration_cast<microseconds>(stop - start).count();
}