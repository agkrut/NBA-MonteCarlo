import requests
import pathlib
import urllib.parse
import json
import logging
import sys

logger = logging.getLogger('nba-download')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler(sys.stdout)
handler.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

# Official NBA Stats API used to generate team data
# BallDontLie API used to generate game data
NBAbaseURL = "http://data.nba.com/data/10s/prod/v1/"
BDLbaseURL = "https://www.balldontlie.io/api/v1/"

parentDir = pathlib.Path("data").absolute()
seasons = range(2012, 2020)

# Routine that creates /data folder structure
def createOutputStructure():
    for season in seasons:
        targetDir = parentDir.joinpath("season{}".format(season))
        targetDir.mkdir(parents=True, exist_ok=True)

# Routine that downloads and writes team JSON data
def generateListofTeamsBySeason(season):
    logger.info("Downloading teams [season={}]".format(season))
    url = urllib.parse.urljoin(NBAbaseURL, '{}/teams.json'.format(season))
    teamsJSON = requests.get(url).json()
    
    parsedTeams =  {'teams' : [team for team in teamsJSON['league']['standard'] if team['isNBAFranchise'] == True]}
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("teams.json")

    with open(str(targetFile), 'w') as f:
        json.dump(parsedTeams, f)

    return [x['tricode'] for x in parsedTeams['teams']]

# Routine that downloads and writes game JSON data by team
def generateListofGamesByRegularSeason(season, tricodes):
    logger.info("Downloading games [season={}]".format(season))
    url = urllib.parse.urljoin(BDLbaseURL, 'games')
    for id in range(30):
        params = {
            "seasons[]" : season,
            "team_ids[]" : id + 1,
            "postseason" : False,
            "per_page" : 100,
            "page" : 0,
        }
        while True:
            try:
                scheduleJSON = requests.get(url, params=params).json()
                parsedGames = {'games' : scheduleJSON['data']}
            except:
                continue
            else:
                break
        tricode = tricodes[id]
        targetFile = parentDir.joinpath("season{}".format(season)).joinpath("{}.json".format(tricode))
        with open(str(targetFile), 'w') as f:
            json.dump(parsedGames, f)

def main():
    createOutputStructure()

    for season in seasons:
        tricodes = generateListofTeamsBySeason(season)
        generateListofGamesByRegularSeason(season, tricodes)


if __name__ == "__main__":
    main()