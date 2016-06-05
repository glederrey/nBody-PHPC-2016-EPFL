#! /bin/bash

mkdir -p ./results-weak-scaling

echo "Brute-Force 10^4"

#mpiexec -n 1 ./exec/brute-force-mpi ./config/weak_scaling/bf_1e4_1.init
#mpiexec -n 2 ./exec/brute-force-mpi ./config/weak_scaling/bf_2e4_2.init
#mpiexec -n 3 ./exec/brute-force-mpi ./config/weak_scaling/bf_3e4_3.init
#mpiexec -n 4 ./exec/brute-force-mpi ./config/weak_scaling/bf_4e4_4.init
#mpiexec -n 5 ./exec/brute-force-mpi ./config/weak_scaling/bf_5e4_5.init
#mpiexec -n 6 ./exec/brute-force-mpi ./config/weak_scaling/bf_6e4_6.init
#mpiexec -n 7 ./exec/brute-force-mpi ./config/weak_scaling/bf_7e4_7.init
#mpiexec -n 8 ./exec/brute-force-mpi ./config/weak_scaling/bf_8e4_8.init

echo "Brute-Force 10^5"

#mpiexec -n 1 ./exec/brute-force-mpi ./config/weak_scaling/bf_1e5_1.init
#mpiexec -n 2 ./exec/brute-force-mpi ./config/weak_scaling/bf_2e5_2.init
#mpiexec -n 3 ./exec/brute-force-mpi ./config/weak_scaling/bf_3e5_3.init
#mpiexec -n 4 ./exec/brute-force-mpi ./config/weak_scaling/bf_4e5_4.init
#mpiexec -n 5 ./exec/brute-force-mpi ./config/weak_scaling/bf_5e5_5.init
#mpiexec -n 6 ./exec/brute-force-mpi ./config/weak_scaling/bf_6e5_6.init
#mpiexec -n 7 ./exec/brute-force-mpi ./config/weak_scaling/bf_7e5_7.init
mpiexec -n 8 ./exec/brute-force-mpi ./config/weak_scaling/bf_8e5_8.init

echo "Barnes-Hut 10^4"

#mpiexec -n 1 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_1e4_1.init
#mpiexec -n 2 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_2e4_2.init
#mpiexec -n 3 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_3e4_3.init
#mpiexec -n 4 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_4e4_4.init
#mpiexec -n 5 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_5e4_5.init
#mpiexec -n 6 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_6e4_6.init
#mpiexec -n 7 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_7e4_7.init
#mpiexec -n 8 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_8e4_8.init

echo "Barnes-Hut 10^5"

#mpiexec -n 1 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_1e5_1.init
#mpiexec -n 2 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_2e5_2.init
#mpiexec -n 3 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_3e5_3.init
#mpiexec -n 4 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_4e5_4.init
#mpiexec -n 5 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_5e5_5.init
#mpiexec -n 6 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_6e5_6.init
#mpiexec -n 7 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_7e5_7.init
#mpiexec -n 8 ./exec/barnes-hut-mpi ./config/weak_scaling/bh_8e5_8.init
