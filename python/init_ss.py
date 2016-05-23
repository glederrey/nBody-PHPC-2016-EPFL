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
    mass = []

    mass_SS = [1.989*math.pow(10,30), 3.3011*math.pow(10,23), 4.8675*math.pow(10,24), 5.972*math.pow(10,24), 6.4171*math.pow(10,23), 1.8986*math.pow(10,27), 5.6836*math.pow(10,26), 8.6810*math.pow(10,25) , 1.0243*math.pow(10,26)];
    dist_SS = [0, 0.387*AU, 0.723*AU, AU, 1.523679*AU, 5.454*AU, 10.086*AU, 20.11*AU, 30.33*AU];
    size_SS = [50*AU, 0.05*AU, 0.1*AU, 0.2*AU, 0.3*AU, AU, 2*AU, 4*AU, 6*AU];
    #size_SS = [50*AU, 1e-3*AU, 2e-3*AU, 3e-3*AU, 4e-3*AU, 1e-2*AU, 2e-5*AU, 4e-2*AU, 1e-1*AU];

    # First set up the Solar System
    for i in range(len(mass_SS)):
        theta = 2*math.pi*random.random()
        x.append(dist_SS[i]*math.sin(theta))
        y.append(dist_SS[i]*math.cos(theta))
        dist.append(dist_SS[i])
        mass.append(mass_SS[i])
        if i == 0:
            vx.append(0)
            vy.append(0)
        else:
            perp_x = -y[i]/dist[i]
            perp_y = x[i]/dist[i]
            speed = math.sqrt(G*(mass[0]+mass[i])/dist[i])
            vx.append(speed*perp_x)
            vy.append(speed*perp_y)


    for i in range(number-len(mass_SS)):
        rnd_sun = random.randint(0,1)
        if rnd_sun == 1:
            idx = random.randint(1,8)
        else:
            idx = 0
        i = i+len(mass_SS)
        r = (size_SS[idx])*random.random()
        theta = 2*math.pi*random.random()
        x_loc = r*math.sin(theta)
        y_loc = r*math.cos(theta)

        # in m
        x.append(x[idx]+r*math.sin(theta))
        y.append(y[idx]+r*math.cos(theta))
        dist.append(math.sqrt(math.pow(x[i],2)+math.pow(y[i],2)))

        # in kg
        power = random.random()*10;
        mass.append(random.random()*math.pow(10,power))

        if rnd_sun == 1:

            perp_x_loc = -y_loc/r
            perp_y_loc = x_loc/r

            speed_loc = math.sqrt(G*(mass[idx]+mass[i])/r)

            perp_x_glob = -y[i]/dist[i]
            perp_y_glob = x[i]/dist[i]

            speed_glob = math.sqrt(G*(mass[0]+mass[i])/dist[i])

            vx.append(speed_loc*perp_x_loc + speed_glob*perp_x_glob)
            vy.append(speed_loc*perp_y_loc + speed_glob*perp_y_glob)
        else:

            perp_x_glob = -y[i]/dist[i]
            perp_y_glob = x[i]/dist[i]
            speed_glob = math.sqrt(G*(mass[0]+mass[i])/dist[i])

            vx.append(speed_glob*perp_x_glob)
            vy.append(speed_glob*perp_y_glob)


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
