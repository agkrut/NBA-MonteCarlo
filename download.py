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

NBAbaseURL = "http://data.nba.com/data/10s/prod/v1/"
BILbaseURL = "https://www.balldontlie.io/api/v1/"

parentDir = pathlib.Path("data").absolute()
seasons = range(2012, 2020)

def createOutputStructure():
    for season in seasons:
        targetDir = parentDir.joinpath("season{}".format(season))
        targetDir.mkdir(parents=True, exist_ok=True)

def generateListofTeamsBySeason(season):
    logger.info("Downloading teams [season={}]".format(season))
    url = urllib.parse.urljoin(NBAbaseURL, '{}/teams.json'.format(season))
    teamsJSON = requests.get(url).json()
    
    parsedTeams =  {'teams' : [team for team in teamsJSON['league']['standard'] if team['isNBAFranchise'] == True]}
    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("teams.json")

    with open(str(targetFile), 'w') as f:
        json.dump(parsedTeams, f)

def generateListofGamesByRegularSeason(season):
    logger.info("Downloading games [season={}]".format(season))
    url = urllib.parse.urljoin(BILbaseURL, 'games')
    params = {
        "seasons[]" : season,
        "postseason" : False,
        "per_page" : 100,
        "page" : 0,
    }
    parsedGames = {'games' : []}
    
    sampleJSON = requests.get(url, params=params).json()
    totalPages = int(sampleJSON['meta']['total_pages'])
    parsedGames['games'] += sampleJSON['data']

    for i in range(1, totalPages):
        params['page'] = i

        while True:
            try:
                pageJSON = requests.get(url, params=params).json()
                parsedGames['games'] += pageJSON['data']
            except:
                logger.warn("Error [game, season={}, page={}".format(season, i))
            else:
                break

    targetFile = parentDir.joinpath("season{}".format(season)).joinpath("games.json")
    with open(str(targetFile), 'w') as f:
        json.dump(parsedGames, f)

def main():
    createOutputStructure()

    for season in seasons:
        generateListofTeamsBySeason(season)
        generateListofGamesByRegularSeason(season)


if __name__ == "__main__":
    main()