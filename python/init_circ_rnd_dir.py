#! usr/bin/python

import argparse
import random
import math

UA = 1.4960e11
G = 6.674e-11

def main(number, size, file):

    if file == None or number == None:
        raise ValueError('File or Number of Bodies were not set')

    x = []
    y = []
    dist = []
    vx = []
    vy = []
    v = []
    mass = []

    x.append(0)
    y.append(0)
    dist.append(0)
    mass.append(1*math.pow(10,35))
    vx.append(0)
    vy.append(0)
    v.append(0)

    min_dist = 10*UA*size
    max_dist = 0

    for i in range(number):
        r = UA*size*(0.01+0.89*(random.random()))
        theta = 2*math.pi*random.random()

        # in m
        x.append(r*math.sin(theta))
        y.append(r*math.cos(theta))
        dist.append(r)

        if r > max_dist:
            max_dist = r

        if r < min_dist:
            min_dist = r
        # in kg
        power = random.random()*20;
        mass.append(random.random()*math.pow(10,power))

    print min_dist/UA

    max_dist =  max_dist*1.1
    min_dist = min_dist*0.9

    for i in range(number):
        integer = random.randint(0,1)
        if integer==0:
            integer = -1
        perp_x = integer*-y[i+1]/dist[i+1]
        perp_y = integer*x[i+1]/dist[i+1]
        speed = math.sqrt(G*(mass[0]+mass[i+1])/dist[i+1])
        # Speed in m/s
        vx.append(speed*perp_x)
        vy.append(speed*perp_y)
        v.append(speed)

    with open(file, 'w') as init_file:
        init_file.write('# mass, x, y, vx, vy,\n')
        for i in range(number):
            init_file.write('%s, %s, %s, %s, %s,\n'%(mass[i], x[i], y[i], vx[i], vy[i]))


if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument('-n', '--number', type=int, help='Number of Bodies.')
    argParser.add_argument('-s', '--size', type=float, help='Size of the square.')
    argParser.add_argument('-f', '--file', help='Output file')
    parsed = argParser.parse_args()
    main(parsed.number, parsed.size, parsed.file)
