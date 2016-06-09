#!/bin/bash



mkdir compile-blocagem

cd compile-blocagem

for O in 0 1 2 3
do
    g++ --std=c++11 -O$O ../exercicio3-blocagem.cpp -o main-blocagem-$O
done


for O in 0 1 2 3
do
    echo "running O=$O" >> output-blocagem-$O.txt
    for N in 1000 2000 4000 6000
    do
        echo "Starting running N=$N" >> output-blocagem-$O.txt
        for nb in `seq 1 10`
        do
            echo "Starting running nb=$nb" >> output-blocagem-$O.txt
            #{ /usr/bin/time -f "\t%E real, \t%U user, \t%S sys" ./main-blocagem-$O $N $nb 1 >> output-blocagem-$O.txt ; } 2>> output-blocagem-$O.txt
            { time ./main-blocagem-$O $N $nb 1 >> output-blocagem-$O.txt ; } 2>> output-blocagem-$O.txt
        done
    done
    echo "\n" >> output-blocagem-$O.txt
done

cd ../
