#include <playoffs.hpp>

Playoffs::Playoffs(json playoffsJSON, map<string,Team*> teams) {
    // Routine to initialize first round
    for (json::iterator it = playoffsJSON.begin(); it != playoffsJSON.end(); ++it) {
        string seriesID = (string) it.key();
        int seriesNum = std::stoi(seriesID.substr(seriesID.size() - 2));

        /* Explanation: the second to last digit of the series ID is the round. The last
        digit tracks which series of the round it is. Series [10-13] are Eastern Conf.
        and [14-17] are Western Conf. Series with last digit 0 is 1 vs. 8, digit 1 is
        2 vs. 7, etc. */
        
        // round 1
        if (seriesNum >= 10 && seriesNum <= 17) {
            int highSeedNum = -1;
            int lowSeedNum = -1;

            switch ((seriesNum % 10) % 4) {
            case 0:
                highSeedNum = 1;
                lowSeedNum = 8;
                break;
            case 1:
                highSeedNum = 2;
                lowSeedNum = 7;
                break;
            case 2:
                highSeedNum = 3;
                lowSeedNum = 6;
                break;
            case 3:
                highSeedNum = 4;
                lowSeedNum = 5;
                break;
            default:
                break;
            }

            json firstGame = (*it)["games"][0]["result"];
            Team* highSeedTeam = teams[firstGame["home"]["abbreviation"]];
            Team* lowSeedTeam = teams[firstGame["away"]["abbreviation"]];
            Series* series = new Series(1, highSeedTeam, lowSeedTeam, highSeedNum, lowSeedNum);

            // eastern conference
            if (seriesNum <= 13)
                this->easternConference.push_back(series);
            // western conference
            else
                this->westernConference.push_back(series);
        }
    }

    // create conf semis
    for (int i = 0; i < 2; i++) {
        this->easternConference.push_back(new Series(2));
        this->westernConference.push_back(new Series(2));
    }
    // create conf finals
    this->easternConference.push_back(new Series(3));
    this->westernConference.push_back(new Series(3));
    this->finals = new Series(4);
}

Playoffs::Playoffs::~Playoffs() {
    for (size_t i = 0; i < easternConference.size(); i++)
        delete easternConference.at(i);
    for (size_t i = 0; i < westernConference.size(); i++)
        delete westernConference.at(i);
    delete finals;
}

vector<Series*> Playoffs::getWesternConference() {
    return this->westernConference;
}
void Playoffs::setWesternConference(vector<Series*> westernConference) {
    this->westernConference = westernConference;
}

vector<Series*> Playoffs::getEasternConference() {
    return this->easternConference;
}
void Playoffs::setEasternConference(vector<Series*> easternConference) {
    this->easternConference = easternConference;
}

map<bool,pair<int,Team*>> Playoffs::calculateSeeding(pair<int,Team*> team1, pair<int,Team*> team2) {
    map<bool,pair<int,Team*>> seeds;
    if (team1.first < team2.first) { // team1 is higher seed
        seeds[true] = team1;
        seeds[false] = team2;
    }
    else { // team 2 is higher seed
        seeds[true] = team2;
        seeds[false] = team1;
    }
    return seeds;
}

void Playoffs::simulateRound1() {
    for (int i = 0; i < 4; i++) { // sim round 1
        this->easternConference.at(i)->simulateSeries();
        this->westernConference.at(i)->simulateSeries();
    }
}
void Playoffs::simulateRound2() {
    for (int i = 4; i < 6; i++) {
            // init round 2 (conf semi-finals)
            // matchups must be 0 vs 3 and 1 vs 2
            int S1 = (i == 4) ? 0 : 1;
            int S2 = (i == 4) ? 3 : 2;

            pair<int,Team*> ecT1 = this->easternConference.at(S1)->getWinnerPair();
            pair<int,Team*> ecT2 = this->easternConference.at(S2)->getWinnerPair();
            pair<int,Team*> wcT1 = this->westernConference.at(S1)->getWinnerPair();
            pair<int,Team*> wcT2 = this->westernConference.at(S2)->getWinnerPair();

            map<bool,pair<int,Team*>> ecSeeds = this->calculateSeeding(ecT1, ecT2);
            map<bool,pair<int,Team*>> wcSeeds = this->calculateSeeding(wcT1, wcT2);

            this->easternConference.at(i)->setHighSeed(ecSeeds[true]);
            this->easternConference.at(i)->setLowSeed(ecSeeds[false]);
            this->westernConference.at(i)->setHighSeed(wcSeeds[true]);
            this->westernConference.at(i)->setLowSeed(wcSeeds[false]);

            // sim round 2
            this->easternConference.at(i)->simulateSeries();
            this->westernConference.at(i)->simulateSeries();
    }
}

void Playoffs::simulateRound3() {
    // init round 3 (conf finals)
    pair<int,Team*> ecT1 = this->easternConference.at(4)->getWinnerPair();
    pair<int,Team*> ecT2 = this->easternConference.at(5)->getWinnerPair();
    pair<int,Team*> wcT1 = this->westernConference.at(4)->getWinnerPair();
    pair<int,Team*> wcT2 = this->westernConference.at(5)->getWinnerPair();

    map<bool,pair<int,Team*>> ecSeeds = this->calculateSeeding(ecT1, ecT2);
    map<bool,pair<int,Team*>> wcSeeds = this->calculateSeeding(wcT1, wcT2);

    this->easternConference.at(6)->setHighSeed(ecSeeds[true]);
    this->easternConference.at(6)->setLowSeed(ecSeeds[false]);
    this->westernConference.at(6)->setHighSeed(wcSeeds[true]);
    this->westernConference.at(6)->setLowSeed(wcSeeds[false]);

    // sim round 3
    this->easternConference.at(6)->simulateSeries();
    this->westernConference.at(6)->simulateSeries();
}

void Playoffs::simulateFinals() {
    // for now assume west coast won all-star game
    // therefore west coast has home-court advantage
    pair<int,Team*> ecTeam = this->easternConference.at(6)->getWinnerPair();
    pair<int,Team*> wcTeam = this->westernConference.at(6)->getWinnerPair();
    this->finals->setHighSeed(wcTeam);
    this->finals->setLowSeed(ecTeam);
    this->finals->simulateSeries();
}

void Playoffs::simulate() {
    this->simulateRound1();
    this->simulateRound2();
    this->simulateRound3();
    this->simulateFinals();
}