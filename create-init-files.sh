#! /bin/bash

echo "Creating files for 10^1"
python ./python/init.py -n 10 -s 100 -f ./init/init_1e1.dat
python ./python/init.py -n 20 -s 100 -f ./init/init_2e1.dat
python ./python/init.py -n 50 -s 100 -f ./init/init_5e1.dat
echo "Creating files for 10^2"
python ./python/init.py -n 100 -s 100 -f ./init/init_1e2.dat
python ./python/init.py -n 200 -s 100 -f ./init/init_2e2.dat
python ./python/init.py -n 500 -s 100 -f ./init/init_5e2.dat
echo "Creating files for 10^3"
python ./python/init.py -n 1000 -s 100 -f ./init/init_1e3.dat
python ./python/init.py -n 2000 -s 100 -f ./init/init_2e3.dat
python ./python/init.py -n 5000 -s 100 -f ./init/init_5e3.dat
echo "Creating files for 10^4"
python ./python/init.py -n 10000 -s 100 -f ./init/init_1e4.dat
python ./python/init.py -n 20000 -s 100 -f ./init/init_2e4.dat
python ./python/init.py -n 30000 -s 100 -f ./init/init_3e4.dat
python ./python/init.py -n 40000 -s 100 -f ./init/init_4e4.dat
python ./python/init.py -n 50000 -s 100 -f ./init/init_5e4.dat
python ./python/init.py -n 60000 -s 100 -f ./init/init_6e4.dat
python ./python/init.py -n 70000 -s 100 -f ./init/init_7e4.dat
python ./python/init.py -n 80000 -s 100 -f ./init/init_8e4.dat
echo "Creating files for 10^5"
python ./python/init.py -n 100000 -s 100 -f ./init/init_1e5.dat
python ./python/init.py -n 200000 -s 100 -f ./init/init_2e5.dat
python ./python/init.py -n 300000 -s 100 -f ./init/init_3e5.dat
python ./python/init.py -n 400000 -s 100 -f ./init/init_4e5.dat
python ./python/init.py -n 500000 -s 100 -f ./init/init_5e5.dat
python ./python/init.py -n 600000 -s 100 -f ./init/init_6e5.dat
python ./python/init.py -n 700000 -s 100 -f ./init/init_7e5.dat
python ./python/init.py -n 800000 -s 100 -f ./init/init_8e5.dat
echo "Creating files for 10^6"
python ./python/init.py -n 1000000 -s 100 -f ./init/init_1e6.dat

