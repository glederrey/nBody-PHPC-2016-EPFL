#! usr/bin/python

import argparse
import time
import numpy as np
import matplotlib.pyplot as plt

size = 1000

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
    while idx < len(data):
        mass = []
        x = []
        y = []
        time = data[idx, 0]
        while data[idx,0] == time:
            mass.append(data[idx,1])
            x.append(data[idx,3])
            y.append(data[idx,4])
            idx+=1
            if idx==len(data):
                break

        plt.clf()
        plt.plot(x,y, 'bo')
        plt.xlim(-size, size)
        plt.ylim(-size, size)
        plt.title(time)
        plt.axis('equal')
        plt.pause(0.001)

if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument('-f', '--file', help='Result file. ')
    parsed = argParser.parse_args()
    main(parsed.file)
