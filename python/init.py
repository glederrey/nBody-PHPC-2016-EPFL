#! usr/bin/python

import argparse
import random
import math

UA = 1.4960e11

def main(number, size, file):

    if file == None or number == None:
        raise ValueError('File or Number of Bodies were not set')

    x = []
    y = []
    vx = []
    vy = []
    v = []
    density = []
    radius = []
    mass = []

    for i in range(number):
        # in m
        x.append(UA*0.9*(random.random()*2.0*size - size))
        y.append(UA*0.9*(random.random()*2.0*size - size))
        # in kg/m^3
        density.append(1000000*(random.random()*10 + 0.01))
        # in m
        radius.append(1000*(random.random()*1000000+1))
        # in kg
        mass.append(density[i]*math.pi*radius[i]*radius[i]*radius[i])

    for i in range(number):
        # Speed in m/s
        vx.append(1000*(random.random()*100-50))
        vy.append(1000*(random.random()*100-50))
        v.append(math.sqrt(vx[i]*vx[i]+vy[i]*vy[i]))

    with open(file, 'w') as init_file:
        init_file.write('# radius, density, mass, x, y, vx, vy,\n')
        for i in range(number):
            init_file.write('%s, %s, %s, %s, %s, %s, %s,\n'%(radius[i], density[i], mass[i], x[i], y[i], vx[i], vy[i]))


if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument('-n', '--number', type=int, help='Number of Bodies.')
    argParser.add_argument('-s', '--size', type=float, help='Size of the square.')
    argParser.add_argument('-f', '--file', help='Output file')
    parsed = argParser.parse_args()
    main(parsed.number, parsed.size, parsed.file)
