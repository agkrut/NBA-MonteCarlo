import json
import pathlib
import dateutil.parser

seasons = range(2018, 2019)

def eloInit(teamsFile):
    teams = {}
    for team in json.load(open(teamsFile))['teams']:
        team['elo'] = [1400]
        teams[team['tricode']] = team
    return teams

def eloFunction(game, homeTeam, roadTeam):
    homeTeamOldElo = homeTeam['elo'][-1]
    roadTeamOldElo = roadTeam['elo'][-1]

    K = 20
    A = 40
    probabilityHomeTeamWins = 1.0 / (1 + pow(10, ((roadTeamOldElo-homeTeamOldElo-A)/400)))
    probabilityRoadTeamWins = 1.0 / (1 + pow(10, ((homeTeamOldElo-roadTeamOldElo+A)/400)))

    # home team wins
    if game['home_team_score'] > game['visitor_team_score']:
        homeTeamNewElo = homeTeamOldElo + K*(1-probabilityHomeTeamWins)
        roadTeamNewElo = roadTeamOldElo + K*(0-probabilityRoadTeamWins)
    else: # road team wins
        homeTeamNewElo = homeTeamOldElo + K*(0-probabilityHomeTeamWins)
        roadTeamNewElo = roadTeamOldElo + K*(1-probabilityRoadTeamWins)       

    homeTeam['elo'].append(homeTeamNewElo)
    roadTeam['elo'].append(roadTeamNewElo)

def main():
    for season in seasons:
        seasonDir = pathlib.Path("data/season{}/".format(season)).absolute()
        teamsPath = seasonDir.joinpath("clean/teams.json").absolute()
        masterSchedPath = seasonDir.joinpath("clean/master.json").absolute()
        
        teams = eloInit(teamsPath)
        masterSchedule = json.load(open(masterSchedPath))['master']

        for game in masterSchedule:
            homeTeam = teams[game['home_team']['abbreviation']]
            roadTeam = teams[game['visitor_team']['abbreviation']]
            eloFunction(game, homeTeam, roadTeam)
        
        targetFile = seasonDir.joinpath("clean/elo.json")
        with open(str(targetFile), 'w') as f:
            json.dump(teams, f, indent=4)

if __name__ == "__main__":
    main()