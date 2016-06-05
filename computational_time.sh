#! /bin/bash

mkdir -p ./results-computational-time

echo "Brute-Force"
./exec/brute-force ./config/computational_time/bf_1e1.init
./exec/brute-force ./config/computational_time/bf_2e1.init
./exec/brute-force ./config/computational_time/bf_5e1.init
./exec/brute-force ./config/computational_time/bf_1e2.init
./exec/brute-force ./config/computational_time/bf_2e2.init
./exec/brute-force ./config/computational_time/bf_5e2.init
./exec/brute-force ./config/computational_time/bf_1e3.init
./exec/brute-force ./config/computational_time/bf_2e3.init
./exec/brute-force ./config/computational_time/bf_5e3.init
./exec/brute-force ./config/computational_time/bf_1e4.init
./exec/brute-force ./config/computational_time/bf_2e4.init
./exec/brute-force ./config/computational_time/bf_5e4.init
./exec/brute-force ./config/computational_time/bf_1e5.init

echo "Barnes-Hut - Theta = 0"
./exec/barnes-hut ./config/computational_time/bh_1e1_0.init
./exec/barnes-hut ./config/computational_time/bh_2e1_0.init
./exec/barnes-hut ./config/computational_time/bh_5e1_0.init
./exec/barnes-hut ./config/computational_time/bh_1e2_0.init
./exec/barnes-hut ./config/computational_time/bh_2e2_0.init
./exec/barnes-hut ./config/computational_time/bh_5e2_0.init
./exec/barnes-hut ./config/computational_time/bh_1e3_0.init
./exec/barnes-hut ./config/computational_time/bh_2e3_0.init
./exec/barnes-hut ./config/computational_time/bh_5e3_0.init
./exec/barnes-hut ./config/computational_time/bh_1e4_0.init
./exec/barnes-hut ./config/computational_time/bh_2e4_0.init
./exec/barnes-hut ./config/computational_time/bh_5e4_0.init
./exec/barnes-hut ./config/computational_time/bh_1e5_0.init

echo "Barnes-Hut - Theta = 0.5"
./exec/barnes-hut ./config/computational_time/bh_1e1_05.init
./exec/barnes-hut ./config/computational_time/bh_2e1_05.init
./exec/barnes-hut ./config/computational_time/bh_5e1_05.init
./exec/barnes-hut ./config/computational_time/bh_1e2_05.init
./exec/barnes-hut ./config/computational_time/bh_2e2_05.init
./exec/barnes-hut ./config/computational_time/bh_5e2_05.init
./exec/barnes-hut ./config/computational_time/bh_1e3_05.init
./exec/barnes-hut ./config/computational_time/bh_2e3_05.init
./exec/barnes-hut ./config/computational_time/bh_5e3_05.init
./exec/barnes-hut ./config/computational_time/bh_1e4_05.init
./exec/barnes-hut ./config/computational_time/bh_2e4_05.init
./exec/barnes-hut ./config/computational_time/bh_5e4_05.init
./exec/barnes-hut ./config/computational_time/bh_1e5_05.init

echo "Barnes-Hut - Theta = 1"
./exec/barnes-hut ./config/computational_time/bh_1e1_1.init
./exec/barnes-hut ./config/computational_time/bh_2e1_1.init
./exec/barnes-hut ./config/computational_time/bh_5e1_1.init
./exec/barnes-hut ./config/computational_time/bh_1e2_1.init
./exec/barnes-hut ./config/computational_time/bh_2e2_1.init
./exec/barnes-hut ./config/computational_time/bh_5e2_1.init
./exec/barnes-hut ./config/computational_time/bh_1e3_1.init
./exec/barnes-hut ./config/computational_time/bh_2e3_1.init
./exec/barnes-hut ./config/computational_time/bh_5e3_1.init
./exec/barnes-hut ./config/computational_time/bh_1e4_1.init
./exec/barnes-hut ./config/computational_time/bh_2e4_1.init
./exec/barnes-hut ./config/computational_time/bh_5e4_1.init
./exec/barnes-hut ./config/computational_time/bh_1e5_1.init
