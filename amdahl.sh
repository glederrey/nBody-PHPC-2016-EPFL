#! /bin/bash

mkdir -p ./results-strong-scaling

echo "Barnes-Hut 1e4"
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/strong_scaling/bh_1e4_1.init

echo "Brute-Force 1e4"
mpiexec -n 1 ./exec/brute-force-mpi ./config/strong_scaling/bf_1e4_1.init

echo "Barnes-Hut 1e5"
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/strong_scaling/bh_1e5_1.init

echo "Brute-Force 1e5"
mpiexec -n 1 ./exec/brute-force-mpi ./config/strong_scaling/bf_1e5_1.init

echo "Barnes-Hut 1e6"
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/strong_scaling/bh_1e6_1.init

echo "Brute-Force 1e6"
mpiexec -n 1 ./exec/brute-force-mpi ./config/strong_scaling/bf_1e6_1.init
