import requests
import pathlib
import urllib.parse
import json
import logging
import sys
import dateutil.parser
from itertools import groupby

logger = logging.getLogger('nba-download')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler(sys.stdout)
handler.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

# Official NBA Data API used to generate team data
# Deprecated NBA Stats API used to generate playoff data
# BallDontLie API used to generate game data
NBAdataURL = "http://data.nba.com/data/10s/prod/v1/"
NBAstatsURL = "http://stats.nba.com/stats/"
BDLbaseURL = "https://www.balldontlie.io/api/v1/"

NBA_REQ_HEADERS = {
    "Host" : "stats.nba.com",
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0',
    'Accept': 'application/json, text/plain, */*',
    'Accept-Language': 'en-US,en;q=0.5',
    'Accept-Encoding': 'gzip, deflate, br',
    'Connection': 'keep-alive',
    "Referer" : "https://stats.nba.com/",
    "x-nba-stats-origin" : "stats",
    "x-nba-stats-token" : "true",
}

parentDir = pathlib.Path("data").absolute()
seasons = range(2012, 2019)

# Routine that creates /data folder structure
def createOutputStructure():
    for season in seasons:
        targetDir = parentDir.joinpath("season{}".format(season))
        targetDir.mkdir(parents=True, exist_ok=True)
        uncleanDir = targetDir.joinpath("unclean")
        cleanDir = targetDir.joinpath("clean")
        uncleanDir.mkdir(parents=True, exist_ok=True)
        cleanDir.mkdir(parents=True, exist_ok=True)
        uncleanDir.joinpath("schedules").mkdir(parents=True, exist_ok=True)
        cleanDir.joinpath("schedules").mkdir(parents=True, exist_ok=True)

# Routine that downloads and writes unclean team JSON data
def getTeams(season):
    logger.info("Downloading teams  [season={}]".format(season))
    url = urllib.parse.urljoin(NBAdataURL, '{}/teams.json'.format(season))
    teamsJSON = requests.get(url).json()
    
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("unclean/teams.json")
    with open(str(targetFile), 'w') as f:
        json.dump(teamsJSON, f, indent=4)

# Cleans teams to only include NBA franchises
def cleanTeams(season):
    logger.info("Cleaning teams     [season={}]".format(season))
    uncleanFilePath = parentDir.joinpath("season{}".format(season)).joinpath("unclean/teams.json")
    uncleanJSON = json.load(open(uncleanFilePath))
    parsedTeams = {"teams" : [team for team in uncleanJSON['league']['standard'] if team['isNBAFranchise'] == True]}
    
    nbaIDs = {i+1:(team['teamId'],team['tricode']) for i,team in enumerate(parsedTeams['teams'])}
    
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("clean/teams.json")
    with open(str(targetFile), 'w') as f:
        json.dump(parsedTeams, f, indent=4)
    return nbaIDs

# Routine that downloads and writes unclean game JSON data by team
def getSchedules(season):
    logger.info("Downloading games  [season={}]".format(season))
    url = urllib.parse.urljoin(BDLbaseURL, 'games')
    for id in range(1,31):
        params = {
            "seasons[]" : season,
            "team_ids[]" : id,
            "postseason" : False,
            "per_page" : 100,
            "page" : 0,
        }
        while True:
            try:
                scheduleJSON = requests.get(url, params=params).json()
            except:
                continue
            else:
                break
        targetFile = parentDir.joinpath("season{}".format(season)).joinpath("unclean/schedules/{}.json".format(id))
        with open(str(targetFile), 'w') as f:
            json.dump(scheduleJSON, f, indent=4)

# Cleans schedules to update teamIDs to be consistent with NBA API
def cleanSchedules(season, nbaIDs):
    logger.info("Cleaning schedules [season={}]".format(season))
    for id in range(1,31):
        tricode = nbaIDs[id][1]
        uncleanFilePath = parentDir.joinpath("season{}".format(season)).joinpath("unclean/schedules/{}.json".format(id))
        uncleanJSON = json.load(open(uncleanFilePath))
        uncleanSchedule = uncleanJSON['data']
        
        for game in uncleanSchedule:
            oldHomeTeamID = game['home_team']['id']
            oldRoadTeamID = game['visitor_team']['id']
            game['home_team']['id'] = int(nbaIDs[oldHomeTeamID][0])
            game['visitor_team']['id'] = int(nbaIDs[oldRoadTeamID][0])
            game['id'] = int(game['id'])

        cleanSchedule = {'games' : sorted(uncleanSchedule, key = lambda x: dateutil.parser.parse(x['date']))}
        targetFile = parentDir.joinpath("season{}".format(season)).joinpath("clean/schedules/{}.json".format(tricode))
        with open(str(targetFile), 'w') as f:
            json.dump(cleanSchedule, f, indent=4)

# Uses individual team schedules to form master schedule
def getMasterSchedule(season, tricodes):
    duplicateTracker = set()
    unsortedMaster = []
    for tricode in tricodes:
        teamSchedulePath = parentDir.joinpath("season{}".format(season)).joinpath("clean/schedules/{}.json".format(tricode))
        teamSchedule = json.load(open(teamSchedulePath))['games']
        for game in teamSchedule:
            if game['id'] not in duplicateTracker:
                unsortedMaster.append(game)
                duplicateTracker.add(game['id'])
    sortedMaster = {'master' : sorted(unsortedMaster, key=lambda x: dateutil.parser.parse(x['date']))}
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("clean/master.json")
    with open(str(targetFile), 'w') as f:
        json.dump(sortedMaster, f, indent=4)

# Gets unclean version of playoffs from stats.nba.com
def getPlayoffs(season):
    logger.info("Scraping playoffs  [season={}]".format(season))
    seasonStr = "{}-{}".format(season, int(str(season)[2:])+1)
    url = urllib.parse.urljoin(NBAstatsURL, 'commonplayoffseries?LeagueID=00&Season={}'.format(seasonStr))

    playoffsJSON = requests.get(url, headers=NBA_REQ_HEADERS).json()
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("unclean/playoffs.json")
    with open(str(targetFile), 'w') as f:
        json.dump(playoffsJSON, f, indent=4)

# Gathers data for each individual playoff game
def getGameWinner(gameID, homeTeamID, roadTeamID):
    url = urllib.parse.urljoin(NBAstatsURL, 'boxscoresummaryv2?gameID={}'.format(gameID))
    gameResultJSON = requests.get(url, headers=NBA_REQ_HEADERS).json()
    lineScore = gameResultJSON["resultSets"][5]
    headers = [h.lower() for h in lineScore['headers']]
    rowSet = lineScore['rowSet']
    
    team1 = dict(zip(headers, rowSet[0]))
    team2 = dict(zip(headers, rowSet[1]))

    team1parsed = {
        'abbreviation' : team1['team_abbreviation'],
        'points' : team1['pts'],
        'win' : team1['pts'] > team2['pts']
    }

    team2parsed = {
        'abbreviation' : team2['team_abbreviation'],
        'points' : team2['pts'],
        'win' : team2['pts'] > team1['pts']
    }
    if team1['team_id'] == homeTeamID:
        return {"home" : team1parsed, "away" : team2parsed}
    else:
        return {"home" : team2parsed, "away" : team1parsed}

# Cleans playoff data into comprehensible form
def cleanPlayoffs(season):
    logger.info("Cleaning playoffs  [season={}]".format(season))
    seasonDir = parentDir.joinpath("season{}".format(season))
    uncleanPlayoffsPath = seasonDir.joinpath("unclean/playoffs.json")
    uncleanPlayoffsJSON = json.load(open(uncleanPlayoffsPath))
    
    headers = [h.lower() for h in uncleanPlayoffsJSON["resultSets"][0]["headers"]]
    rowSet = uncleanPlayoffsJSON["resultSets"][0]["rowSet"]
    games = [dict(zip(headers, game)) for game in rowSet]

    for game in games:
        gameID = game['game_id']
        homeTeamID = game['home_team_id']
        roadTeamID = game['visitor_team_id']
        game['result'] = getGameWinner(gameID, homeTeamID, roadTeamID)

    series = {s[0]:{"games" : list(s[1]), "round": int(s[0][-2])} for s in groupby(games, lambda x: x['series_id'])}
    targetFile = seasonDir.joinpath("clean/playoffs.json")
    with open(str(targetFile), 'w') as f:
        json.dump(series, f, indent=4)

def main():
    createOutputStructure()

    for season in seasons:
        getTeams(season)
        nbaIDs = cleanTeams(season)
        getSchedules(season)
        tricodes = [x[1] for x in nbaIDs.values()]
        cleanSchedules(season, nbaIDs)
        getMasterSchedule(season, tricodes)
        getPlayoffs(season)

    for season in seasons:
        cleanPlayoffs(season)

if __name__ == "__main__":
    main()