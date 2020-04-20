import json
import matplotlib.pyplot as plt
import pathlib
seasons = range(2012,2019)

# Routine that saves graphs of regular season
# ELO changes for Eastern and Western conference

for season in seasons:
    eloPath = pathlib.Path("data/season{}/clean/elo.json".format(season)).absolute()
    eloJSON = json.load(open(eloPath))

    playoffsPath = pathlib.Path("data/season{}/clean/playoffs.json".format(season)).absolute()
    playoffsJSON = json.load(open(playoffsPath))

    playoffTeams = []
    for series in playoffsJSON.values():
        if series["round"] == 1:
            playoffTeams.append(series["games"][0]["result"]["home"]["abbreviation"])
            playoffTeams.append(series["games"][0]["result"]["away"]["abbreviation"])

    easternTeams = sorted(playoffTeams[:8])
    westernTeams = sorted(playoffTeams[8:])

    plt.xlabel("Games")
    plt.ylabel("ELO")
    plt.suptitle("Regular Season ELO Changes [EC season={}]".format(season))

    for team in easternTeams:
        plt.plot(eloJSON[team]['elo'], label=team)
    
    plt.legend(loc='upper left')

    plt.savefig("data/season{}/clean/ec_elo.png".format(season))
    plt.clf()

    plt.xlabel("Games")
    plt.ylabel("ELO")
    plt.suptitle("Regular Season ELO Changes [WC season={}]".format(season))

    for team in westernTeams:
        plt.plot(eloJSON[team]['elo'], label=team)
    
    plt.legend(loc='upper left')

    plt.savefig("data/season{}/clean/wc_elo.png".format(season))
    plt.clf()