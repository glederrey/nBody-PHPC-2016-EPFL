#! /usr/bin/sh

# This script is used to launch the benchmark test for the Amdahl's law
./barnes-hut-opt config_1e4_bh.init
./barnes-hut-opt config_1e5_bh.init
./barnes-hut-opt config_1e6_bh.init

./brute-force-opt config_1e4_bf.init
./brute-force-opt config_1e5_bf.init
./brute-force-opt config_1e6_bf.init
