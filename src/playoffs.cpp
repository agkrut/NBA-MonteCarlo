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

    // init advance for conference series
    for (int i = 0; i < 2; i++) {
        this->easternConference.at(i)->setAdvance(this->easternConference.at(i/2 + 4));
        this->westernConference.at(i)->setAdvance(this->westernConference.at(i/2 + 4));
    }

    this->easternConference.back()->setAdvance(this->finals);
    this->westernConference.back()->setAdvance(this->finals);
}

Playoffs::Playoffs::~Playoffs() {
    for (size_t i = 0; i < easternConference.size(); i++)
        delete easternConference.at(i);
    for (size_t i = 0; i < westernConference.size(); i++)
        delete westernConference.at(i);
    delete finals;
}