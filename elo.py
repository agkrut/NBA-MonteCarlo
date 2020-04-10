import json
import pathlib

seasons = range(2012, 2013)

def eloInit(teamsFile):
    elos = {}
    for team in json.load(open(teamsFile))['teams']:
        elos[team["tricode"]] = {
            "city": team['city'],
            "altCityName": team['altCityName'],
            "tricode": team['tricode'],
            "nickname": team['nickname'],
            "confName": team['confName'],
            "divName": team['divName'],
            "elo" : 1500
        }
    return elos

# input: tuple (homeTeamElo, awayTeamElo) before game
# output: tuple (homeTeamElo, awayTeamElo) after game
def eloFunction(matchup):
    homeTeam = matchup[0]
    awayTeam = matchup[1]

def main():
    for season in seasons:
        seasonDir = pathlib.Path("data/season{}/".format(season)).absolute()
        teamsPath = seasonDir.joinpath("teams.json").absolute()
        gamesPath = seasonDir.joinpath("games.json").absolute()
        elos = eloInit(teamsPath)
        print(elos)

if __name__ == "__main__":
    main()