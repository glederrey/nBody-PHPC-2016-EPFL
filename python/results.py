#! usr/bin/python

import argparse
import time
import numpy as np
import matplotlib.pyplot as plt

size = 10000

def main(result_file):
    if result_file == None:
        raise ValueError('Result file was not added.')

    data = np.genfromtxt(result_file ,delimiter=",")

    idx = 0;
    plt.figure(1)
    plt.ion()
    plt.xlim(-size, size)
    plt.ylim(-size, size)
    plt.axis('equal')
    nbr_bodies_start = 0
    nbr_bodies_end = 0
    while idx < len(data):
        mass = []
        x = []
        y = []
        time = data[idx, 0]
        while data[idx,0] == time:
            mass.append(data[idx,1])
            x.append(data[idx,2])
            y.append(data[idx,3])
            if time==0:
                nbr_bodies_start = len(x)
            idx+=1
            if idx==len(data):
                nbr_bodies_end = len(x)
                break
        print "LOOL"
        
        plt.clf()
        plt.plot(x,y, 'bo')
        plt.axis('equal')
        plt.xlim(-size, size)
        plt.ylim(-size, size)
        plt.title(time)
        plt.pause(0.001)

    print "Number bodies at the beginning of the simulation: %s"%nbr_bodies_start
    print "Number bodies at the end of the simulation: %s"%nbr_bodies_end
    print "Percentage of bodies lost: %s %%"%((float(nbr_bodies_start)-float(nbr_bodies_end))/float(nbr_bodies_start)*100)

if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument('-f', '--file', help='Result file. ')
    parsed = argParser.parse_args()
    main(parsed.file)
