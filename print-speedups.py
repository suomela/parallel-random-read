#!/usr/bin/env python

import os

BASELINE = "test-simple"

def keypart(x):
    try:
        return int(x)
    except:
        return x

def sortkey(s):
    return [keypart(x) for x in s.split("-")]

print
for d in sorted(os.listdir("."), key=sortkey):
    if d.startswith("result-"):
        result = {}
        tests = []
        nset = set()
        for a in sorted(os.listdir(d), key=sortkey):
            if a.startswith("test-"):
                result[a] = {}
                if a != BASELINE:
                    tests.append(a)
                with open(os.path.join(d, a)) as f:
                    for l in f:
                        fields = l.rstrip("\n").split("\t")
                        tmin, tavg, tmax = [float(x) for x in fields[0:3]]
                        n = int(fields[3])
                        result[a][n] = (tmin, tavg, tmax)
                        nset.add(n)
        nlist = sorted(nset)
        for a in tests:
            print "{}/{}".format(d, a)
            print
            for n in nlist:
                rmin = result[BASELINE][n][0] / result[a][n][2]
                ravg = result[BASELINE][n][1] / result[a][n][1]
                rmax = result[BASELINE][n][2] / result[a][n][0]
                print "{:.3f}\t{:.3f}\t{:.3f}\t{}".format(rmin, ravg, rmax, n)
            print
