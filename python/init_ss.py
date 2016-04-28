#! usr/bin/python

import argparse
import random
import math

AU = 1.4960e11
G = 6.674e-11
size = 50*AU
minMass = 1
maxMass = 1e10

def main(number, file):

    if file == None or number == None:
        raise ValueError('File or Number of Bodies were not set')

    x = []
    y = []
    dist = []
    vx = []
    vy = []
    v = []
    mass = []

    mass_SS = [1.989*math.pow(10,30), 3.3011*math.pow(10,23), 4.8675*math.pow(10,24), 5.972*math.pow(10,24), 6.4171*math.pow(10,23), 1.8986*math.pow(10,27), 5.6836*math.pow(10,26), 8.6810*math.pow(10,25) , 1.0243*math.pow(10,26)];
    dist_SS = [0, 0.387*AU, 0.723*AU, AU, 1.523679*AU, 5.454*AU, 10.086*AU, 20.11*AU, 30.33*AU];

    # First set up the Solar System
    for i in range(len(mass_SS)):
        x.append(dist_SS[i])
        y.append(0)
        dist.append(dist_SS[i])
        mass.append(mass_SS[i])
        vx.append(0)
        if i == 0:
            v.append(0)
            vy.append(0)
        else:
            v.append(math.sqrt(G*(mass[0]+mass[i])/dist[i]))
            vy.append(math.sqrt(G*(mass[0]+mass[i])/dist[i]))


    for i in range(number):
        i = i+len(mass_SS)
        r = size*random.random()
        theta = 2*math.pi*random.random()

        # in m
        x.append(r*math.sin(theta))
        y.append(r*math.cos(theta))
        dist.append(r)

        # in kg
        power = random.random()*10;
        mass.append(random.random()*math.pow(10,power))
        perp_x = -y[i]/dist[i]
        perp_y = x[i]/dist[i]
        speed = math.sqrt(G*(mass[0]+mass[i])/dist[i])
        vx.append(speed*perp_x)
        vy.append(speed*perp_y)
        v.append(speed)

    with open(file, 'w') as init_file:
        init_file.write('# mass, x, y, vx, vy,\n')
        for i in range(len(mass)):
            init_file.write('%s, %s, %s, %s, %s,\n'%(mass[i], x[i], y[i], vx[i], vy[i]))


if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument('-n', '--number', type=int, help='Number of Bodies.')
    argParser.add_argument('-f', '--file', help='Output file')
    parsed = argParser.parse_args()
    main(parsed.number, parsed.file)
