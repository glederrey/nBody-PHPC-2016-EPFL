# nBody-PHPC-2016-EPFL

### Introduction

This project is about Parallel and High Performance Computing. It has been during the Spring Semester 2016 at the EPFL for the course PHPC (http://short.epfl.ch/hpc-phpc-2016).  In the following README, you will find how to use the code (compiling the C++, using the python codes and the matlab ones)

In this project, I created 4 different codes:
- Serial version of the Brute-Force algorithm
- Parralel (MPI) version of the Brute-Force algorithm
- Serial version of the Barnes-Hut algorithm
- Parralel (MPI) version of the Barnes-Hut algorithm

The purpose of this project was to test these two algorithms (Brute-Force and Barnes-Hut) and then test their parallel version.

You can find the video showing some results for a simulation with 1 million bodies here: https://www.youtube.com/watch?v=h0nFjeHjKYs. The video was done for the presentation.

### Report and Slides

You can find the Proposal of the project, the Final Report and the Slides for the presentation in the folder *Latex*. 

### Compiling the C++ code

In order to compile the C++ code, you need to go in the *src* folder. First, you need to enable/disable the different compilation flags. They are at line 3 in the **Makefile**. :
```
CFLAGS+=-Wall -g -Ofast -ftree-vectorize -DDEBUG -DWRITE_TIME -DWRITE_OUTPUT #-DWRITE_QUADTREE
```
The different flags are descrbibed below:
- **-DDEBUG**: This flag enables the *cout* output for debugging or for having an idea how the simulation is going.
- **-DWRITE_TIME**: If this flag is enabled, then it will write the different times in a file. 
- **-DWRITE_OUTPUT**: If this flag is enabled, then it will write the positions of the bodies in a file.
- **-DWRITE_QUADTREE**: If this flag is enable, then it will write the quadtree in a file. (Works only with the Barnes-Hut codes)

Then, you can compile it just using the command `make`. This will create a folder called exec and put all the object files and the executables in this folder.

### Executing the C++ code

In order to be executed each of the codes need a configuration file looking like this:
```
dt=0.1
finalTime=10
samplingFreq=1
maxSize=1000000
minSize=1e-9
theta=1
initialFile=./init/init_1e6.dat
outputFile=./results/test.dat
```
`dt` is the time step. It is given in days as for the `finalTime`. The `samplingFreq` is used when the flags **-DWRITE_TIME**, **-DWRITE_OUTPUT** and/or **-DWRITE_QUADTREE** are used. It defines the frequency at which it will write in a file. For example, if `samplingFreq=0.5`, then it will write every two time steps in the files. `maxSize` is the maximum size of the simulations. If a body has a position further than this `maxSize`, it will be removed. The `minSize` is used for combining two bodies that are too close. Both of these values are given in AU. `theta` is the precision parameter used for the Barnes-Hut algorithm. It takes a value equal or bigger than 0. It's not used by the brute-force executables. But it's not a problem if it's in their configuration file. The `initialFile` is the file where the initial mass, positions and velocities are given. The `outputFile` is the file in which the results will be written if the flag **-DWRITE_OUTPUT** is given. If the flag **-DWRITE_QUADTREE** is given, the quadtree will be written in the file `./results/test_quadtree.dat` and if the flag **-DWRITE_TIME** is given, the times will be written in the file `./results/test_time.dat`.

Some examples are given in the folder *config*. You can then execute the codes like this:
- Brute-Force serial: `./exec/brute-force ./config/bf.init`
- Brute-Force parallel: `mpiexec -n 4 ./exec/brute-force-mpi ./config/bf_parallel.init`
- Brute-Force serial: `./exec/barnes-hut ./config/bh.init`
- Brute-Force serial: `mpiexec -n 4 ./exec/barnes-hut-mpi ./config/bh_parallel.init`

### Create new initial data

In the folder *python*, you have two files that can create new initial conditions. The first file **init.py** will create totally random bodies. You can use it like this: `python init.py -n 10000 -s 100 -f ../init/test.dat`. The first parameter is the number of bodies, the second parameter is the edge length of the square in which the bodies are created (in AU) and the final parameter is the path and name of the file that will be created. For the report, I only used this file.

For the fun, I created a script that create the initial positions of the solar system with the sun and the 8 biggest planets. The script is the file **init_ss.py**. You can use it like this: `python init.py -n 10000 -f ../init/test-ss.dat`. The bodies have 50% of chance to orbite around the sun, otherwise they will randomly orbite around one of the planets.

### Recreate the figures in the Final Report

First, if you want to recreate the initial files for the figures, you can use the script **create-init-files.sh**. In this repository, the initial files are already here. But you can always recreate them.

In order to recreate the figures that are in the report, you need to run the following three bash script: **computational_time.sh**, **amdahl.sh**, **strong_scaling.sh** and **weak_scaling.sh**. And then you can use the different Matlab scripts that are in the *matlab* folder. The list below gives you the scripts you need to run to recreate each of the figures:
- Figure 1 (Computational times): Run the script **computational_time.sh** and then run the matlab script **computational_time.m**
- Table 1 & Figure 2 (Amdahl's law): Run the script **amdahl.sh** and then run the matlab script **amdahl.m**
- Figures 3 & 4 (Strong scaling): Run the script **strong_scaling.sh** and then run the matlab script **strong_scaling.m**
- Figures 5 & 6 (Weak scaling): Run the script **weak_scaling.sh** and then run the matlab script **weak_scaling.m**

Each of the Figures have been then saved as tex file using `matlab2tikz`. (available here: http://www.mathworks.com/matlabcentral/fileexchange/22022-matlab2tikz-matlab2tikz) Their size also has been changed to fit the 10 pages requirement of the report. 

