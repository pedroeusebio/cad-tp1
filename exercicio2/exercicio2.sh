#!/bin/bash

RUN_PATH="/home/pedroeusebio/local/hpctoolkit/bin"


for N in 10 100 1000 10000 100000 1000000 10000000 20000000 40000000 60000000 80000000 100000000 110000000 120000000 130000000
do
    mkdir stream-$N
    cd stream-$N
	  gcc -g -o -dstream_array_size=$N ../../stream.c -o stream$N
    cd ../
done

for N in 10 100 1000 10000 100000 1000000 10000000 20000000 40000000 60000000 80000000 100000000 110000000 120000000 130000000
do
    cd stream-$N
    $RUN_PATH/hpcrun ./stream$N
    cd ../
done

for N in 10 100 1000 10000 100000 1000000 10000000 20000000 40000000 60000000 80000000 100000000 110000000 120000000 130000000
do
    cd stream-$N
    $RUN_PATH/hpcstruct ./stream$N
    $RUN_PATH/hpcprof -S stream$N.hpcstruct -I~ hpctoolkit-stream$N-measurements
    cd ../
done

