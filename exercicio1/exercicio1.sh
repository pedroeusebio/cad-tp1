#!/bin/bash

for N in 10 100 1000 10000 100000 1000000 10000000 20000000 40000000 60000000 80000000 100000000 110000000 120000000 130000000
do
	gcc -O -DSTREAM_ARRAY_SIZE=$N stream.c -o stream$N
done

for N in 10 100 1000 10000 100000 1000000 10000000 20000000 40000000 60000000 80000000 100000000 110000000 120000000 130000000
do 
	echo "running N=$N\n" >> output-exercicio1.txt
	./stream$N >> output-exercicio1.txt
	echo "----------------------------\n" >> output-exercicio1.txt
done


