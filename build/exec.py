import os
import itertools

K = [20,30,40]
A = [40,50,60,70]
N = [8**i for i in range(1,7)]
T = [2**i for i in range(0,5)]

os.system("rm ../data/season*/results/*.csv")

for params in list(itertools.product(*[K,A,N,T])):
    args = " ".join(str(s) for s in params)
    os.system("./exec {}".format(args))