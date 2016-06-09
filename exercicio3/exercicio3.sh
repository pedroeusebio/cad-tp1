#!/bin/bash



mkdir compiler

cd compiler

for O in 0 1 2 3
do
    g++ --std=c++11 -O$O ../exercicio3.cpp -o main-$O
    g++ --std=c++11 -O$O ../exercicio3-invertido.cpp -o main-invertido-$O
done


for O in 0 1 2 3
do
    echo "running O=$O" >> output$O.txt
    echo "running O=$O" >> output-invertido-$O.txt
    for N in 1000 2000 4000 6000
    do
        echo "Starting running N=$N" >> output$O.txt
        echo "Starting running N=$N" >> output-invertido-$O.txt
        { /usr/bin/time -f "\t%E real, \t%U user, \t%S sys" ./main-$O $N 1 >> output$O.txt ; } 2>> output$O.txt
        { /usr/bin/time -f "\t%E real, \t%U user, \t%S sys" ./main-invertido-$O $N 1 >> output-invertido-$O.txt ; } 2>> output-invertido-$O.txt
        echo "\n" >> output$O.txt
        echo "\n" >> output-invertido-$O.txt
    done
done

cd ../
