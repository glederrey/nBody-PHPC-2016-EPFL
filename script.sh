#! /bin/bash

echo "Barnes-Hut 1e4"
mpiexec -n 2 ./exec/barnes-hut-mpi ./config/bh_1e4_2.init &
mpiexec -n 4 ./exec/barnes-hut-mpi ./config/bh_1e4_4.init &
mpiexec -n 6 ./exec/barnes-hut-mpi ./config/bh_1e4_6.init &
mpiexec -n 8 ./exec/barnes-hut-mpi ./config/bh_1e4_8.init &
mpiexec -n 10 ./exec/barnes-hut-mpi ./config/bh_1e4_10.init &
mpiexec -n 14 ./exec/barnes-hut-mpi ./config/bh_1e4_14.init &
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/bh_1e4_1.init

mpiexec -n 20 ./exec/barnes-hut-mpi ./config/bh_1e4_20.init &
mpiexec -n 18 ./exec/barnes-hut-mpi ./config/bh_1e4_18.init

mpiexec -n 24 ./exec/barnes-hut-mpi ./config/bh_1e4_24.init

mpiexec -n 30 ./exec/barnes-hut-mpi ./config/bh_1e4_30.init

mpiexec -n 36 ./exec/barnes-hut-mpi ./config/bh_1e4_36.init

mpiexec -n 42 ./exec/barnes-hut-mpi ./config/bh_1e4_42.init

mpiexec -n 48 ./exec/barnes-hut-mpi ./config/bh_1e4_48.init

echo "Brute-Force 1e4"
mpiexec -n 2 ./exec/brute-force-mpi ./config/bf_1e4_2.init &
mpiexec -n 4 ./exec/brute-force-mpi ./config/bf_1e4_4.init &
mpiexec -n 6 ./exec/brute-force-mpi ./config/bf_1e4_6.init &
mpiexec -n 8 ./exec/brute-force-mpi ./config/bf_1e4_8.init &
mpiexec -n 10 ./exec/brute-force-mpi ./config/bf_1e4_10.init &
mpiexec -n 14 ./exec/brute-force-mpi ./config/bf_1e4_14.init &
mpiexec -n 1 ./exec/brute-force-mpi ./config/bf_1e4_1.init

mpiexec -n 20 ./exec/brute-force-mpi ./config/bf_1e4_20.init &
mpiexec -n 18 ./exec/brute-force-mpi ./config/bf_1e4_18.init

mpiexec -n 24 ./exec/brute-force-mpi ./config/bf_1e4_24.init

mpiexec -n 30 ./exec/brute-force-mpi ./config/bf_1e4_30.init

mpiexec -n 36 ./exec/brute-force-mpi ./config/bf_1e4_36.init

mpiexec -n 42 ./exec/brute-force-mpi ./config/bf_1e4_42.init

mpiexec -n 48 ./exec/brute-force-mpi ./config/bf_1e4_48.init

echo "Barnes-Hut 1e5"
mpiexec -n 2 ./exec/barnes-hut-mpi ./config/bh_1e5_2.init &
mpiexec -n 4 ./exec/barnes-hut-mpi ./config/bh_1e5_4.init &
mpiexec -n 6 ./exec/barnes-hut-mpi ./config/bh_1e5_6.init &
mpiexec -n 8 ./exec/barnes-hut-mpi ./config/bh_1e5_8.init &
mpiexec -n 10 ./exec/barnes-hut-mpi ./config/bh_1e5_10.init &
mpiexec -n 14 ./exec/barnes-hut-mpi ./config/bh_1e5_14.init &
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/bh_1e5_1.init

mpiexec -n 20 ./exec/barnes-hut-mpi ./config/bh_1e5_20.init &
mpiexec -n 18 ./exec/barnes-hut-mpi ./config/bh_1e5_18.init

mpiexec -n 24 ./exec/barnes-hut-mpi ./config/bh_1e5_24.init

mpiexec -n 30 ./exec/barnes-hut-mpi ./config/bh_1e5_30.init

mpiexec -n 36 ./exec/barnes-hut-mpi ./config/bh_1e5_36.init

mpiexec -n 42 ./exec/barnes-hut-mpi ./config/bh_1e5_42.init

mpiexec -n 48 ./exec/barnes-hut-mpi ./config/bh_1e5_48.init

echo "Brute-Force 1e5"
mpiexec -n 2 ./exec/brute-force-mpi ./config/bf_1e5_2.init &
mpiexec -n 4 ./exec/brute-force-mpi ./config/bf_1e5_4.init &
mpiexec -n 6 ./exec/brute-force-mpi ./config/bf_1e5_6.init &
mpiexec -n 8 ./exec/brute-force-mpi ./config/bf_1e5_8.init &
mpiexec -n 10 ./exec/brute-force-mpi ./config/bf_1e5_10.init &
mpiexec -n 14 ./exec/brute-force-mpi ./config/bf_1e5_14.init &
mpiexec -n 1 ./exec/brute-force-mpi ./config/bf_1e5_1.init

mpiexec -n 20 ./exec/brute-force-mpi ./config/bf_1e5_20.init &
mpiexec -n 18 ./exec/brute-force-mpi ./config/bf_1e5_18.init

mpiexec -n 24 ./exec/brute-force-mpi ./config/bf_1e5_24.init

mpiexec -n 30 ./exec/brute-force-mpi ./config/bf_1e5_30.init

mpiexec -n 36 ./exec/brute-force-mpi ./config/bf_1e5_36.init

mpiexec -n 42 ./exec/brute-force-mpi ./config/bf_1e5_42.init

mpiexec -n 48 ./exec/brute-force-mpi ./config/bf_1e5_48.init

echo "Barnes-Hut 1e6"
mpiexec -n 2 ./exec/barnes-hut-mpi ./config/bh_1e6_2.init &
mpiexec -n 4 ./exec/barnes-hut-mpi ./config/bh_1e6_4.init &
mpiexec -n 6 ./exec/barnes-hut-mpi ./config/bh_1e6_6.init &
mpiexec -n 8 ./exec/barnes-hut-mpi ./config/bh_1e6_8.init &
mpiexec -n 10 ./exec/barnes-hut-mpi ./config/bh_1e6_10.init &
mpiexec -n 14 ./exec/barnes-hut-mpi ./config/bh_1e6_14.init &
mpiexec -n 1 ./exec/barnes-hut-mpi ./config/bh_1e6_1.init

mpiexec -n 20 ./exec/barnes-hut-mpi ./config/bh_1e6_20.init &
mpiexec -n 18 ./exec/barnes-hut-mpi ./config/bh_1e6_18.init

mpiexec -n 24 ./exec/barnes-hut-mpi ./config/bh_1e6_24.init

mpiexec -n 30 ./exec/barnes-hut-mpi ./config/bh_1e6_30.init

mpiexec -n 36 ./exec/barnes-hut-mpi ./config/bh_1e6_36.init

mpiexec -n 42 ./exec/barnes-hut-mpi ./config/bh_1e6_42.init

mpiexec -n 48 ./exec/barnes-hut-mpi ./config/bh_1e6_48.init

echo "Brute-Force 1e6"
mpiexec -n 2 ./exec/brute-force-mpi ./config/bf_1e6_2.init &
mpiexec -n 4 ./exec/brute-force-mpi ./config/bf_1e6_4.init &
mpiexec -n 6 ./exec/brute-force-mpi ./config/bf_1e6_6.init &
mpiexec -n 8 ./exec/brute-force-mpi ./config/bf_1e6_8.init &
mpiexec -n 10 ./exec/brute-force-mpi ./config/bf_1e6_10.init &
mpiexec -n 14 ./exec/brute-force-mpi ./config/bf_1e6_14.init &
mpiexec -n 1 ./exec/brute-force-mpi ./config/bf_1e6_1.init

mpiexec -n 20 ./exec/brute-force-mpi ./config/bf_1e6_20.init &
mpiexec -n 18 ./exec/brute-force-mpi ./config/bf_1e6_18.init

mpiexec -n 24 ./exec/brute-force-mpi ./config/bf_1e6_24.init

mpiexec -n 30 ./exec/brute-force-mpi ./config/bf_1e6_30.init

mpiexec -n 36 ./exec/brute-force-mpi ./config/bf_1e6_36.init

mpiexec -n 42 ./exec/brute-force-mpi ./config/bf_1e6_42.init

mpiexec -n 48 ./exec/brute-force-mpi ./config/bf_1e6_48.init
