import matplotlib.pyplot as plt
import pathlib
import csv
import os
import json
import itertools
import re

# init info
seasons = range(2012,2019)
K = [20,30,40]
A = [40,50,60,70]
N = [8**i for i in range(1,7)]
T = [2**i for i in range(0,4)]

# returns csv dict given path
def getCSV(path):
    return csv.DictReader(open(path, 'r'))        

# parses sim file name and returns dict
def parseFileName(path):
    parsed = re.findall(r'\d+', path)
    return {
        'year' : int(parsed[0]),
        'K' : int(parsed[1]),
        'A' : int(parsed[2]),
        'N' : int(parsed[3]),
        'T' : int(parsed[4]),
    }

# plots thread speedup and performance given input size N
def plotThreadSpeedup():
    data = {k1:{k2:[] for k2 in T} for k1 in N}

    for nVal in N:
        for season in seasons:
            resultsPath = pathlib.Path("data/season{}/results/".format(season)).absolute()
            for params in list(itertools.product(*[K,A,[nVal],T])):
                fileTarget = "sim_{}_{}_{}_{}.csv".format(params[0],params[1],params[2],params[3])
                fullPath = resultsPath.joinpath(fileTarget).absolute()
                timeColumn = [int(row['D']) for row in getCSV(fullPath)]
                data[nVal][params[3]].append(sum(timeColumn) / len(timeColumn))

    plt.xlabel("Threads")
    plt.ylabel("Time (us)")
    plt.suptitle("Simulation Thread Performance")
    plt.grid()

    for nVal in N:
        plt.plot(T, [sum(data[nVal][tVal]) / len(data[nVal][tVal]) for tVal in T], label = "N={}".format(nVal))

    plt.legend(loc='upper right')
    plt.xscale("log", basex=2)
    plt.savefig("analysis/speedup.png")
    plt.clf()

def predictions():
    finalsCorrect = []
    winnerCorrect = []
    for season in seasons:
        # get actual playoff results
        playoffsPath = pathlib.Path("data/season{}/clean/playoffs.json".format(season)).absolute()
        playoffsJSON = json.load(open(playoffsPath))
        finalsID = list(playoffsJSON.keys())[-1]
        lastGame = playoffsJSON[finalsID]["games"][-1]["result"]
        actualTeams = sorted([lastGame['home']['abbreviation'], lastGame['away']['abbreviation']])
        actualWinner = lastGame['home']['abbreviation'] if lastGame['home']['win'] else lastGame['away']['abbreviation']

        # compare sims to actual
        resultsPath = pathlib.Path("data/season{}/results/".format(season)).absolute()
        simFiles = [resultsPath.joinpath(c) for c in os.listdir(resultsPath)]
        for simFile in simFiles:
            data = list(getCSV(simFile))[-1]
            simTeams = sorted([data['T1'], data['T2']])
            simWinner = data['Winner']

            if simTeams == actualTeams and simWinner == actualWinner:
                finalsCorrect.append(simFile)
                winnerCorrect.append(simFile)
            elif simWinner == actualWinner:
                winnerCorrect.append(simFile)

    finalsCorrect = itertools.groupby([parseFileName(str(x)) for x in finalsCorrect], lambda x: x['year'])
    winnerCorrect = itertools.groupby([parseFileName(str(x)) for x in winnerCorrect], lambda x: x['year'])

    finalsCorrect = {k:list(v) for k,v in finalsCorrect}
    winnerCorrect = {k:list(v) for k,v in winnerCorrect}

    # plot values for finalsCorrect
    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("K-Value Frequency in Correct Finals Simulations")
    for season in finalsCorrect:
        kFreq = {k:0 for k in K}
        for game in finalsCorrect[season]:
            kFreq[game['K']] += 1
        plt.bar(season-0.2, kFreq[20], width=0.2, color='b', align='center',label=20)
        plt.bar(season, kFreq[30], width=0.2, color='g', align='center',label=30)
        plt.bar(season+0.2, kFreq[40], width=0.2, color='r', align='center',label=40)
    plt.legend(K, loc='upper right')
    plt.savefig("analysis/k_finals.png")
    plt.clf()

    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("A-Value Frequency in Correct Finals Simulations")
    for season in finalsCorrect:
        aFreq = {k:0 for k in A}
        for game in finalsCorrect[season]:
            aFreq[game['A']] += 1
        plt.bar(season-0.2, aFreq[40], width=0.1, color='b', align='center', label=40)
        plt.bar(season-0.1, aFreq[50], width=0.1, color='g', align='center', label=50)
        plt.bar(season, aFreq[60], width=0.1, color='r', align='center', label=60)
        plt.bar(season+0.1, aFreq[70], width=0.1, color='r', align='center',label=70)
    plt.legend(A, loc='upper right')
    plt.savefig("analysis/a_finals.png")
    plt.clf()

    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("N-Value Frequency in Correct Finals Simulations")
    for season in finalsCorrect:
        nFreq = {k:0 for k in N}
        for game in finalsCorrect[season]:
            nFreq[game['N']] += 1
        plt.bar(season-0.3, nFreq[8**1], width=0.1, color='b', align='center', label=8**1)
        plt.bar(season-0.2, nFreq[8**2], width=0.1, color='g', align='center', label=8**2)
        plt.bar(season-0.1, nFreq[8**3], width=0.1, color='r', align='center', label=8**3)
        plt.bar(season, nFreq[8**4], width=0.1, color='c', align='center', label=8**4)
        plt.bar(season+0.1, nFreq[8**5], width=0.1, color='y', align='center', label=8**5)
        plt.bar(season+0.2, nFreq[8**6], width=0.1, color='m', align='center', label=8**6)
    plt.legend(N, loc='upper right')
    plt.savefig("analysis/n_finals.png")
    plt.clf()

    # plot values for winnerCorrect
    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("K-Value Frequency in Correct Winner Simulations")
    for season in winnerCorrect:
        kFreq = {k:0 for k in K}
        for game in winnerCorrect[season]:
            kFreq[game['K']] += 1
        plt.bar(season-0.2, kFreq[20], width=0.2, color='b', align='center',label=20)
        plt.bar(season, kFreq[30], width=0.2, color='g', align='center',label=30)
        plt.bar(season+0.2, kFreq[40], width=0.2, color='r', align='center',label=40)
    plt.legend(K, loc='upper right')
    plt.savefig("analysis/k_winner.png")
    plt.clf()

    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("A-Value Frequency in Correct Winner Simulations")
    for season in winnerCorrect:
        aFreq = {k:0 for k in A}
        for game in winnerCorrect[season]:
            aFreq[game['A']] += 1
        plt.bar(season-0.2, aFreq[40], width=0.1, color='b', align='center', label=40)
        plt.bar(season-0.1, aFreq[50], width=0.1, color='g', align='center', label=50)
        plt.bar(season, aFreq[60], width=0.1, color='r', align='center', label=60)
        plt.bar(season+0.1, aFreq[70], width=0.1, color='r', align='center',label=70)
    plt.legend(A, loc='upper right')
    plt.savefig("analysis/a_winner.png")
    plt.clf()

    plt.xlabel("Season")
    plt.ylabel("Frequency")
    plt.suptitle("N-Value Frequency in Correct Winner Simulations")
    for season in winnerCorrect:
        nFreq = {k:0 for k in N}
        for game in winnerCorrect[season]:
            nFreq[game['N']] += 1
        plt.bar(season-0.3, nFreq[8**1], width=0.1, color='b', align='center', label=8**1)
        plt.bar(season-0.2, nFreq[8**2], width=0.1, color='g', align='center', label=8**2)
        plt.bar(season-0.1, nFreq[8**3], width=0.1, color='r', align='center', label=8**3)
        plt.bar(season, nFreq[8**4], width=0.1, color='c', align='center', label=8**4)
        plt.bar(season+0.1, nFreq[8**5], width=0.1, color='y', align='center', label=8**5)
        plt.bar(season+0.2, nFreq[8**6], width=0.1, color='m', align='center', label=8**6)
    plt.legend(N, loc='upper right')
    plt.savefig("analysis/n_winner.png")
    plt.clf()

def main():
    plotThreadSpeedup()
    predictions()

if __name__ == "__main__":
    main()