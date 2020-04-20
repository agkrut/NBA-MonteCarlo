import matplotlib.pyplot as plt
import pathlib
import csv
import os

seasons = range(2012,2019)
K = [20,30,40]
A = [40,50,60,70]
N = [8**i for i in range(1,7)]
T = [2**i for i in range(0,4)]

import itertools

def getCSVDict(path):
    return csv.DictReader(open(path, 'r'))        

def plotThreadSpeedup():
    data = {k1:{k2:[] for k2 in T} for k1 in N}

    for nVal in N:
        for season in seasons:
            resultsPath = pathlib.Path("data/season{}/results/".format(season)).absolute()
            for params in list(itertools.product(*[K,A,[nVal],T])):
                fileTarget = "sim_{}_{}_{}_{}.csv".format(params[0],params[1],params[2],params[3])
                fullPath = resultsPath.joinpath(fileTarget).absolute()
                timeColumn = [int(row['D']) for row in getCSVDict(fullPath)]
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

def main():
    plotThreadSpeedup()

if __name__ == "__main__":
    main()