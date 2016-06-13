# Trabalho Prático 1
## Aluno: Pedro Eusébio

### **Introdução**
O trabalho é referente a matéria de Computação de alto desempenho. Os testes realizados durante o desenvolvimento do trabalho foram rodados em um computador com sistema operacional Ubuntu 14.04 de 64bits, processador i7 terceira geração com 8 *cores*. Além disso, possuia 8GBs de memória RAM.
Para facilidade e praticidade da realização dos testes, foram utilizados *scripts* em bash, do forma que houvesse uma organização dos dados gerados e facilitasse a utilização dos diversos comandos do HPCToolKit. 
Todos os dados gerados para a confecção deste relatório encontra-se no repositório publico do GitHub : [https://github.com/pedroeusebio/cad-tp1](https://github.com/pedroeusebio/cad-tp1).


### **Exercício 1: Medindo a taxa de transferência da mémoria em MB/s**

O programa foi compilado com algumas variações do tamanho do array(`DSTREAM_ARRAY_SIZE`). Os tamanho N variou de 10 até 1.3E+8. A compilação foi realizada através de um *script* para agilizar e facilitar o coleta dos dados.
O Script:

``` sh
#!/bin/bash

myArray=(1000000 10000000 20000000 40000000 60000000 
        80000000 100000000 110000000 120000000 130000000)
for N in myArray 
do
	gcc -O -DSTREAM_ARRAY_SIZE=$N stream.c -o stream$N
done

for N in myArray
do 
	echo "running N=$N\n" >> output-exercicio1.txt
	./stream$N >> output-exercicio1.txt
	echo "----------------------------\n" >> output-exercicio1.txt
done
```
O *script* acima tem como princípio um número `N` que representa o tamanho do *array* que será usado para compilar o `stream.c`. Para cada N definido, executa-se a compilação do `stream.c`. Após a compilação de todos, executa-se o arquivo binário e salva o seu *output* em um arquivo texto para ser analisado posteriormente.
o gráfico abaixo, é o resultado da análise dos valores da taxa de transferência para as operações *Add*, *Copy*, *Scale*, *Triad*. Cada um representado por uma linha de cor deferente. Vale atentar para o fato de que valores abaixo de 10000 não foram inseridos no grafíco pois os dados coletados pelo `stream.c` foram elevados e informados como infinito. Além disso, valores maiores que 130 milhões também não puderam ser computados por conta de erro de alocação na memória.
![imagem](https://i.imgur.com/QIO62Ku.png)


Podemos perceber pelo gráfico que para valores muito pequenos a taxa de transfêrencia é elevada chegando no maximo quando o tamanho do *array* é de 100 mil valores. Percebe-se também que para valores maiores de 1 milhão a taxa de transfêrencia fica praticamente constante, tendo uma variação de +/- 100 MB/s. A tabela usada para gerar o gráfico pode ser vista a seguir:

|N    |10   |100  |1000   |10k    |100k   |1M     |10M    |20M    |
|:----|:---:|:--: |:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
|Copy |inf  |inf  |inf    |31956,6|31956,6|14184,9|13068,7|13181,1|
|Scale|inf  |inf  |inf    |23141  |28557  |13888,4|13119  |12901,1|
|Add  |inf  |inf  |inf    |30504  |27280  |14779,5|14735,6|14361,4|
|Triad|inf  |inf  |25165,8|21883,3|34239,2|15208,2|14567,6|14246,8|
|Error|1E-13|1E-13|1E-13  |1E-13  |1E-13  |1E-13  |1E-13  |1E-13  |

|N    |40M    |60M    |80M    |100M   |110M   |120M    |130M   |
|:----|:-----:|:-----:|:-----:|:-----:|:-----:|:------:|:-----:|
|Copy |13042,6|12849,5|13110,6|12385,2|12877,8|13074,8 |12757,8|
|Scale|13258,8|13020,8|12863,3|12650,2|13261,3|13306,7 |13180  |
|Add  |14314,3|14324,4|14074,9|13523,6|14556,8|14329,1 |14620,7|
|Triad|14143,2|14418,5|13906,9|13382,9|14522,9| 14280,8|14517,5|
|Error|1E-13  |1E-13  |1E-13  |1E-13  |1E-13  |1E-13   |1E-13  |

![](https://i.imgur.com/N7PGUXl.png)
A imagem acima mostra de maneira mais detalhada as diferenças entre as taxas de tranferência para cada operação. As duas operações *add* e *triad* geraram dados muito próximos, já as operações de *copy* e *scale* obtiveram taxas um pouco menores comparadas as outras duas. 
Podemos concluir

### **Exercício 2: Trabalhando com perfiladores**

O programa Foi compilado e rodado para diversas variações de N que representa o tamanho do array a ser alocado. Para tal, foi utilizado um script em bash para facilitar a organização dos arquivos gerados pelo `hpcrun`, `hpcstruct`e `hpcprof`. 

``` sh
#!/bin/bash

RUN_PATH="/home/pedroeusebio/local/hpctoolkit/bin"
myArray=(10 100 1000 10000 100000 1000000 10000000 20000000
        40000000 60000000 80000000 100000000 110000000 120000000 130000000)

for N in myArray
do
    mkdir stream-$N
    cd stream-$N
	  gcc -g -O -DSTREAM_ARRAY_SIZE=$N ../../stream.c -o stream$N
    cd ../
done

for N in myArray
do
    cd stream-$N
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./stream$N
    cd ../
done

for N in myArray
do
    cd stream-$N
    $RUN_PATH/hpcstruct ./stream$N
    $RUN_PATH/hpcprof -S stream$N.hpcstruct -I~ hpctoolkit-stream$N-measurements
    cd ../
done
```
O script funciona da seguinte maneira: 
	1. Para cada N que representa o tamanho do array:
	2. cria-se uma pasta onde todos os dados do `./stream`serão armazenados. Dentro dessa pasta insere-se o arquivo compilado que será usado para gerar os dados pelo *HPCToolKit*.
	2. Em cada pasta roda-se o `hpcrun` no arquivo `stream` para gerar os dados.
	3. Em seguida gera-se outros dados com o `hpcstruc`e o `hpcprof`.
Os dados foram analisados usando o *HPCViewer* e foram gerados arquivo `.csv` para facilitar a criação das tabelas.

Os dados gerados abaixo : 
Para N = 1000000, temos  o cache miss de L2 = 8.00e+06 e de L1 = 1.20e+07.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 6.42e+04 100 %     | 6.42e+04 100 %     |
| main                         | 6.42e+04 100 %     | 6.42e+04 100 %     |
| stream.c: 316                | 1.60e+04 24.9%     | 1.60e+04 24.9%     |
| stream.c: 326                | 7.97e+03 12.4%     | 7.97e+03 12.4%     |
| stream.c: 336                | 2.47e+04 38.4%     | 2.47e+04 38.4%     |
| stream.c: 346                | 1.56e+04 24.3%     | 1.56e+04 24.3%     |

Para N=10000000, temos o cache miss de L2 = 8.40e+07 e de L1 = 1.29e+08.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 6.86e+05 100 %     | 6.86e+05 100 %     |
| main                         | 6.86e+05 100 %     | 6.54e+05 95.4%     |
| stream.c: 336                | 1.60e+05 23.3%     | 1.60e+05 23.3%     |
| stream.c: 335                |  2.39e+04 3.5%     |  2.39e+04 3.5%     |
| stream.c: 346                | 1.44e+05 21.0%     | 1.44e+05 21.0%     |
| stream.c: 345                |  2.40e+04 3.5%     |  2.40e+04 3.5%     |
| stream.c: 316                | 9.58e+04 14.0%     | 9.58e+04 14.0%     |
| stream.c: 315                |  3.19e+04 4.7%     |  3.19e+04 4.7%     |
| stream.c: 326                | 9.58e+04 14.0%     | 9.58e+04 14.0%     |
| stream.c: 325                |  4.00e+04 5.8%     |  4.00e+04 5.8%     |
| stream.c: 464                |  7.99e+03 1.2%     |  7.99e+03 1.2%     |
| stream.c: 466                |  2.38e+04 3.5%     |  2.38e+04 3.5%     |
| stream.c: 271                |  2.36e+04 3.4%     |  2.36e+04 3.4%     |
| stream.c: 272                |  7.70e+03 1.1%     |  7.70e+03 1.1%     |
| stream.c: 289                |  7.99e+03 1.2%     |  7.99e+03 1.2%     |

Para N= 20000000, temos o cache miss de L2 = 1.76e+08 e de L1 = 2.65e+08.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 1.38e+06 100 %     | 1.38e+06 100 %     |
| main                         | 1.38e+06 100 %     | 1.32e+06 96.0%     |
| stream.c: 336                | 3.51e+05 25.5%     | 3.51e+05 25.5%     |
| stream.c: 335                |  1.60e+04 1.2%     |  1.60e+04 1.2%     |
| stream.c: 346                | 3.11e+05 22.6%     | 3.11e+05 22.6%     |
| stream.c: 345                |  3.20e+04 2.3%     |  3.20e+04 2.3%     |
| stream.c: 316                | 2.00e+05 14.5%     | 2.00e+05 14.5%     |
| stream.c: 315                |  7.19e+04 5.2%     |  7.19e+04 5.2%     |
| stream.c: 326                | 2.48e+05 18.0%     | 2.48e+05 18.0%     |
| stream.c: 466                |  1.60e+04 1.2%     |  1.60e+04 1.2%     |
| stream.c: 463                |  7.99e+03 0.6%     |  7.99e+03 0.6%     |
| stream.c: 464                |  1.58e+04 1.1%     |  1.58e+04 1.1%     |
| stream.c: 465                |  1.60e+04 1.2%     |  1.60e+04 1.2%     |
| stream.c: 289                |  1.59e+04 1.2%     |  1.59e+04 1.2%     |
| stream.c: 288                |  7.98e+03 0.6%     |  7.98e+03 0.6%     |
| stream.c: 270                |  8.02e+03 0.6%     |  8.02e+03 0.6%     |
| stream.c: 271                |  2.95e+04 2.1%     |  2.95e+04 2.1%     |
| stream.c: 272                |  3.15e+04 2.3%     |  3.15e+04 2.3%     |

Para N=40000000, temos o cache miss de L2 = 3.60e+08 e de L1 = 5.32e+08.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 2.80e+06 100 %     | 2.80e+06 100 %     |
| main                         | 2.80e+06 100 %     | 2.67e+06 95.4%     |
| stream.c: 336                | 6.87e+05 24.5%     | 6.87e+05 24.5%     |
| stream.c: 335                |  3.20e+04 1.1%     |  3.20e+04 1.1%     |
| stream.c: 346                | 6.47e+05 23.1%     | 6.47e+05 23.1%     |
| stream.c: 345                |  7.99e+04 2.9%     |  7.99e+04 2.9%     |
| stream.c: 316                | 3.67e+05 13.1%     | 3.67e+05 13.1%     |
| stream.c: 315                |  1.44e+05 5.1%     |  1.44e+05 5.1%     |
| stream.c: 326                | 4.63e+05 16.5%     | 4.63e+05 16.5%     |
| stream.c: 325                |  5.59e+04 2.0%     |  5.59e+04 2.0%     |
| stream.c: 464                |  3.99e+04 1.4%     |  3.99e+04 1.4%     |
| stream.c: 465                |  2.40e+04 0.9%     |  2.40e+04 0.9%     |
| stream.c: 271                |  7.17e+04 2.6%     |  7.17e+04 2.6%     |
| stream.c: 269                |  4.77e+04 1.7%     |  4.77e+04 1.7%     |
| stream.c: 270                |  2.95e+04 1.1%     |  2.95e+04 1.1%     |
| stream.c: 272                |  7.97e+03 0.3%     |  7.97e+03 0.3%     |
| stream.c: 289                |  3.98e+04 1.4%     |  3.98e+04 1.4%     |

Para N=60000000, temos o cache miss de L2 = 5.28e+08 e de L1 = 8.01e+08.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 4.14e+06 100 %     | 4.14e+06 100 %     |
| main                         | 4.14e+06 100 %     | 3.97e+06 95.8%     |
| stream.c: 336                | 9.26e+05 22.4%     | 9.26e+05 22.4%     |
| stream.c: 335                |  1.28e+05 3.1%     |  1.28e+05 3.1%     |
| stream.c: 346                | 8.46e+05 20.4%     | 8.46e+05 20.4%     |
| stream.c: 345                |  2.32e+05 5.6%     |  2.32e+05 5.6%     |
| stream.c: 316                | 5.83e+05 14.1%     | 5.83e+05 14.1%     |
| stream.c: 315                |  1.84e+05 4.4%     |  1.84e+05 4.4%     |
| stream.c: 326                | 7.03e+05 17.0%     | 7.03e+05 17.0%     |
| stream.c: 325                |  9.58e+04 2.3%     |  9.58e+04 2.3%     |
| stream.c: 464                |  4.79e+04 1.2%     |  4.79e+04 1.2%     |
| stream.c: 465                |  3.99e+04 1.0%     |  3.99e+04 1.0%     |
| stream.c: 466                |  8.77e+04 2.1%     |  8.77e+04 2.1%     |
| stream.c: 271                |  6.34e+04 1.5%     |  6.34e+04 1.5%     |
| stream.c: 269                |  3.18e+04 0.8%     |  3.18e+04 0.8%     |
| stream.c: 270                |  5.61e+04 1.4%     |  5.61e+04 1.4%     |
| stream.c: 272                |  6.19e+04 1.5%     |  6.19e+04 1.5%     |
| stream.c: 288                |  8.00e+03 0.2%     |  8.00e+03 0.2%     |
| stream.c: 289                |  4.78e+04 1.2%     |  4.78e+04 1.2%     |

Para N=8000temos o cache miss de L2 = 7.24e+08 e de L1 = 1.07e+09.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 5.56e+06 100 %     | 5.56e+06 100 %     |
| main                         | 5.56e+06 100 %     | 5.32e+06 95.7%     |
| stream.c: 336                | 1.29e+06 23.1%     | 1.29e+06 23.1%     |
| stream.c: 335                |  1.28e+05 2.3%     |  1.28e+05 2.3%     |
| stream.c: 346                | 1.21e+06 21.8%     | 1.21e+06 21.8%     |
| stream.c: 345                |  2.16e+05 3.9%     |  2.16e+05 3.9%     |
| stream.c: 316                | 7.27e+05 13.1%     | 7.27e+05 13.1%     |
| stream.c: 315                |  3.03e+05 5.5%     |  3.03e+05 5.5%     |
| stream.c: 326                | 9.34e+05 16.8%     | 9.34e+05 16.8%     |
| stream.c: 325                |  1.28e+05 2.3%     |  1.28e+05 2.3%     |
| stream.c: 466                |  7.17e+04 1.3%     |  7.17e+04 1.3%     |
| stream.c: 464                |  7.99e+04 1.4%     |  7.99e+04 1.4%     |
| stream.c: 465                |  7.19e+04 1.3%     |  7.19e+04 1.3%     |
| stream.c: 463                |  1.60e+04 0.3%     |  1.60e+04 0.3%     |
| stream.c: 269                |  4.74e+04 0.9%     |  4.74e+04 0.9%     |
| stream.c: 272                |  7.87e+04 1.4%     |  7.87e+04 1.4%     |
| stream.c: 270                |  6.39e+04 1.2%     |  6.39e+04 1.2%     |
| stream.c: 271                |  1.20e+05 2.2%     |  1.20e+05 2.2%     |
| stream.c: 288                |  1.60e+04 0.3%     |  1.60e+04 0.3%     |
| stream.c: 289                |  5.59e+04 1.0%     |  5.59e+04 1.0%     |

Para N=100000000, temos o cache miss de L2 = 8.92e+08 e de L1 = 1.33e+09.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 7.24e+06 100 %     | 7.24e+06 100 %     |
| main                         | 7.21e+06 99.7%     | 6.93e+06 95.7%     |
| stream.c: 336                | 1.70e+06 23.5%     | 1.70e+06 23.5%     |
| stream.c: 335                |  1.76e+05 2.4%     |  1.76e+05 2.4%     |
| stream.c: 346                | 1.72e+06 23.8%     | 1.72e+06 23.8%     |
| stream.c: 345                |  1.76e+05 2.4%     |  1.76e+05 2.4%     |
| stream.c: 316                | 9.42e+05 13.0%     | 9.42e+05 13.0%     |
| stream.c: 315                |  3.91e+05 5.4%     |  3.91e+05 5.4%     |
| stream.c: 326                | 1.22e+06 16.9%     | 1.22e+06 16.9%     |
| stream.c: 325                |  1.36e+05 1.9%     |  1.36e+05 1.9%     |
| stream.c: 464                |  8.78e+04 1.2%     |  8.78e+04 1.2%     |
| stream.c: 465                |  1.28e+05 1.8%     |  1.28e+05 1.8%     |
| stream.c: 466                |  7.16e+04 1.0%     |  7.16e+04 1.0%     |
| stream.c: 289                |  7.57e+04 1.0%     |  7.57e+04 1.0%     |
| stream.c: 288                |  1.60e+04 0.2%     |  1.60e+04 0.2%     |
| stream.c: 269                |  7.17e+04 1.0%     |  7.17e+04 1.0%     |
| stream.c: 271                |  1.44e+05 2.0%     |  1.44e+05 2.0%     |
| stream.c: 272                |  8.68e+04 1.2%     |  8.68e+04 1.2%     |
| stream.c: 270                |  6.36e+04 0.9%     |  6.36e+04 0.9%     |
    
Para N=110000000, temos o cache miss de L2 = 9.    40e+08 e de L1 = 1.46e+09.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 7.44e+06 100 %     | 7.44e+06 100 %     |
| main                         | 7.42e+06 99.8%     | 7.08e+06 95.2%     |
| stream.c: 336                | 1.68e+06 22.6%     | 1.68e+06 22.6%     |
| stream.c: 335                | 2.08e+05 2.8%      | 2.08e+05 2.8%      |
| stream.c: 346                | 1.73e+06 23.3%     | 1.73e+06 23.3%     |
| stream.c: 345                | 1.60e+05 2.1%      | 1.60e+05 2.1%      |
| stream.c: 326                | 1.23e+06 16.5%     | 1.23e+06 16.5%     |
| stream.c: 325                | 1.68e+05 2.3%      | 1.68e+05 2.3%      |
| stream.c: 316                | 1.04e+06 14.0%     | 1.04e+06 14.0%     |
| stream.c: 315                | 3.83e+05 5.2%      | 3.83e+05 5.2%      |
| stream.c: 289                | 7.98e+04 1.1%      | 7.98e+04 1.1%      |
| stream.c: 288                | 2.39e+04 0.3%      | 2.39e+04 0.3%      |
| stream.c: 464                | 7.18e+04 1.0%      | 7.18e+04 1.0%      |
| stream.c: 465                | 9.56e+04 1.3%      | 9.56e+04 1.3%      |
| stream.c: 466                | 1.76e+05 2.4%      | 1.76e+05 2.4%      |
| stream.c: 271                | 1.43e+05 1.9%      | 1.43e+05 1.9%      |
| stream.c: 272                | 7.18e+04 1.0%      | 7.18e+04 1.0%      |
| stream.c: 269                | 6.25e+04 0.8%      | 6.25e+04 0.8%      |
| stream.c: 270                | 9.61e+04 1.3%      | 9.61e+04 1.3%      |

Para N=120000000, temos que o cache miss de L2 = 1.08e+09 e de L1 = 1.61e+09.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics | 8.31e+06 100 %     | 8.31e+06 100 %     |
| main                         | 8.30e+06 99.9%     | 7.95e+06 95.7%     |
| stream.c: 336                | 1.97e+06 23.7%     | 1.97e+06 23.7%     |
| stream.c: 335                | 1.60e+05 1.9%      | 1.60e+05 1.9%      |
| stream.c: 346                | 1.84e+06 22.2%     | 1.84e+06 22.2%     |
| stream.c: 345                | 2.87e+05 3.5%      | 2.87e+05 3.5%      |
| stream.c: 316                | 1.08e+06 13.0%     | 1.08e+06 13.0%     |
| stream.c: 315                | 4.79e+05 5.8%      | 4.79e+05 5.8%      |
| stream.c: 326                | 1.35e+06 16.2%     | 1.35e+06 16.2%     |
| stream.c: 325                | 2.00e+05 2.4%      | 2.00e+05 2.4%      |
| stream.c: 464                | 1.44e+05 1.7%      | 1.44e+05 1.7%      |
| stream.c: 466                | 1.12e+05 1.3%      | 1.12e+05 1.3%      |
| stream.c: 463                | 7.99e+03 0.1%      | 7.99e+03 0.1%      |
| stream.c: 465                | 8.77e+04 1.1%      | 8.77e+04 1.1%      |
| stream.c: 272                | 1.20e+05 1.4%      | 1.20e+05 1.4%      |
| stream.c: 269                | 7.17e+04 0.9%      | 7.17e+04 0.9%      |
| stream.c: 270                | 7.19e+04 0.9%      | 7.19e+04 0.9%      |
| stream.c: 271                | 2.05e+05 2.5%      | 2.05e+05 2.5%      |
| stream.c: 289                | 1.12e+05 1.3%      | 1.12e+05 1.3%      |

Para N=13000000, temos que o cache miss de L2 = 1.12e+09 e de L1 = 1.74e+09.

| Scope                        | CPUTIME (usec) (I) | CPUTIME (usec) (E) |
|------------------------------|:------------------:|:------------------:|
| Experiment Aggregate Metrics |  8.84e+06 100 %    |   8.84e+06 100 %   |
| main                         |  8.83e+06 99.9%    |   8.45e+06 95.6%   |
| stream.c: 336                |  2.01e+06 22.8%    |   2.01e+06 22.8%   |
| stream.c: 335                |   2.23e+05 2.5%    |    2.23e+05 2.5%   |
| stream.c: 346                |  1.88e+06 21.3%    |   1.88e+06 21.3%   |
| stream.c: 345                |   3.91e+05 4.4%    |    3.91e+05 4.4%   |
| stream.c: 316                |  1.25e+06 14.1%    |   1.25e+06 14.1%   |
| stream.c: 315                |   4.71e+05 5.3%    |    4.71e+05 5.3%   |
| stream.c: 326                |  1.51e+06 17.1%    |   1.51e+06 17.1%   |
| stream.c: 325                |   1.44e+05 1.6%    |    1.44e+05 1.6%   |
| stream.c: 289                |   9.58e+04 1.1%    |    9.58e+04 1.1%   |
| stream.c: 288                |   1.60e+04 0.2%    |    1.60e+04 0.2%   |
| stream.c: 465                |   7.17e+04 0.8%    |    7.17e+04 0.8%   |
| stream.c: 466                |   1.60e+05 1.8%    |    1.60e+05 1.8%   |
| stream.c: 463                |   8.00e+03 0.1%    |    8.00e+03 0.1%   |
| stream.c: 464                |   1.44e+05 1.6%    |    1.44e+05 1.6%   |
| stream.c: 269                |   1.35e+05 1.5%    |    1.35e+05 1.5%   |
| stream.c: 271                |   1.68e+05 1.9%    |    1.68e+05 1.9%   |
| stream.c: 270                |   9.56e+04 1.1%    |    9.56e+04 1.1%   |
| stream.c: 272                |   5.59e+04 0.6%    |    5.59e+04 0.6%   |

Os diretórios gerados pelo HPCToolKit podem ser encontrados no [link](https://dl.dropboxusercontent.com/u/13096197/All%20Databases.zip)  para o  zip.

### **Exercício 3: Operacões com matrizes**

O exercício 3 propõe que façamos uma subrotina que realize a multiplicação de duas matrizes quadradas de ordem N. Para tal utilizei o código abaixo para gerar as matrizes : 

``` cpp
void fillArray(int** element, long n, int number) {
  for (int i = 0; i < n; i++ ){
    for(int j = 0; j < n; j++) {
      element[i][j] = number;
    }
  }
}
```
A função fillArray recebe um array `int **vectorX = new int*[n]` , o numero `n` de elementos que ele deve tere por fim o número `number` que simboliza o numero que a matriz deve conter.

Para realizar a multiplicação de matrizes, utilizei a função `multiplyArray` que segue o pseudo-código descrito no documento do trabalho: 

``` cpp
void multiplyArray(int** result, int** vectorX, int** vectorY, long n){
  for(int j = 0 ; j < n; j++){
    for(int i = 0; i < n; i++){
      result[i][j] = 0;
      for(int k=0; k < n; k++) {
        result[i][j] = result[i][j] + vectorX[i][k] * vectorY[k][j];
      }
    }
  }
}
```
Da mesma forma, para a inversão dos laços I e J, utilizei o código abaixo:

``` cpp
void multiplyArray(int** result, int** vectorX, int** vectorY, long n){
  for(int i = 0 ; i < n;i++){
    for(int j = 0; j < n; j++){
      result[i][j] = 0;
      for(int k=0; k < n; k++) {
        result[i][j] = result[i][j] + vectorX[i][k] * vectorY[k][j];
      }
    }
  }
}
```

Para mais detalhes do código, acesse o link do [github](https://github.com/pedroeusebio/cad-tp1) onde está armazenado o repositório do projeto.

Os resultados gerados pela compilação em diferentes niveis de otimização pode ser representados na tabela abaixo:
Para a multiplicação com os laços I e J invertidos, tivemos os resultados:

| Array Size | Otimização 0 (min) | Otimização 1 (min) | Otimização 2 (min) | Otimização 3 (min) |
|------------|:------------------:|:------------------:|:------------------:|:------------------:|
| 1000       |       0:07.69      |       0:03.07      |       0:03.16      | 0:03.14            |
| 2000       |       1:48.19      |       1:10.83      |       1:11.84      | 1:13.01            |
| 4000       |      12:30.41      |      12:33.23      |       8:40.76      | 11:06.75           |
| 6000       |      44:52.49      |      28:25.29      |      24.25.71      | 27:51.95           |

O gráfico abaixo gerado a partir da tabela acima, aponta que o aumento do número do tamanho do *array* aumenta o tempo de execução. Além disso, percebe-se que com maior otimização mais rápido será a multiplicação das matrizes. No eixo X temos o tempo em segundos e no eixo Y temos o tamanho do *array*.
![](https://i.imgur.com/cK3LpYr.png)


Para a multiplicação com os laços I e J iguais ao pseudo-código, tivemos os resultados:

| Array Size | Otimização 0 (min) | Otimização 1 (min) | Otimização 2 (min) | Otimização 3 (min) |
|------------|:------------------:|:------------------:|:------------------:|:------------------:|
| 1000       |       0:06.63      |       0:03.34      |       0:03.15      |       0:03.16      |
| 2000       |       1:17.76      |       1:10.14      |       1:11.82      |       1:12.55      |
| 4000       |      13:07.94      |      13:01.03      |      12:36.04      |      12:34.29      |
| 6000       |      36:16.34      |      28:34.05      |      28:59.27      |      17:00.24      |


O gráfico abaixo é resultado da tabela de tempo para a multiplicação das matrizes de acordo com o pseudo-código.
 
![](/Users/pedroeusebio/Workspace/university/computacao-de-alto-desempenho/cad-tp1/exercicio3/normal2.png)
Podemos perceber que ambos tiveram desempenho bem parecido quando se trata de matrizes com tamanho entre 1000 e 2000. Podemos notar também que a compilação utilizando otimização resulta em uma melhora significativa no tempo de execução do programa. Utilizando a otimização de nivel 3 teve-se uma melhora de quase 50% no caso do `array` de 6000.
Além disso, utilizando o HPCTookit para realizar a perfilagem dos dois códigos, pode se perceber que o maior gargalo do sistema é a função de multiplicação. Além disso, percebe-se que a diferença de tempo é baixa entre os dois métodos utilizados. O HPCToolkit apontou um tempo de 6.47e+06 µsec para o código original e 6.99e+06 µsec para o código invertido.
A melhor implementação escolhida foi de lanço I anterior ao laço J, por conta dos testes realizados acima. A diferença em comparação aos dois métodos já utilizados é visível. O tempo de execução para N=1000 foi para 1,25 segundos com otimização nivel 3. O tamanho do bloco utilizado foi de 8, por conta de testes sequenciais e pela verificação dos tempos obtidos. 
 
``` cpp
void multiplyArray(float** result, float** vectorX, float** vectorY, int n, int nb){
  for(int ii = 0; ii < n ; ii += nb){
    for(int jj = 0; jj < n ; jj += nb){
      for(int kk = 0; kk < n; kk += nb){
        for(int i = ii; i < std::min(n, ii + nb); i++){
          for(int j = jj; j < std::min(n, jj + nb); j++){
            for(int k = kk; k < std::min(n, kk +nb); k++) {
              result[i][j] = result[i][j] + vectorX[i][k] *
			  vectorY[k][j];
            }
          }
        }
      }
    }
  }
```

O código utilizado para fazer a multiplicação de matrizes com a técnica de blocagem está exibida acima.

### **Exercício 4: Otimizando um código científico.**

Utilizando o HCPTooKit para realizar a perfilagem do código, podemos perceber que existe um gargalo na função `iso_3dfd_it`. Essa função é chamada duas vezes dentro da outra função `run_wave_propagation`. Elas, possuem 50% do tempo de execução do programa. Podemos perceber isso na tabela abaixo:

| Scope                                                                   | CPUTIME (usec) (I) |
|-------------------------------------------------------------------------|--------------------|
| Experiment Aggregate Metrics                                            | 1.38e+09 100 %     |
| <program root>                                                          | 1.38e+09 100 %     |
| main                                                                    | 1.38e+09 100 %     |
| loop at : 0                                                             | 1.38e+09 100 %     |
| run_wave_propagation() 												  | 1.38e+09 99.9%     |
| loop at : 0                                                             | 1.38e+09 99.9%     |
| iso_3dfd_it() 													      | 6.94e+08 50.4%     |
| iso_3dfd_it()        													  | 6.81e+08 49.4%     |
| write_plane_XY(float***, Parameters*, int, char const*)                 | 1.37e+06 0.1%      |
| initialize(float***, float***, float***, Parameters*)                   | 1.54e+06 0.1%      |
| write_plane_XY(float***, Parameters*, int, char const*)                 | 6.35e+04 0.0%      |
| Allocate3DMatrix(int, int, int)                                         | 1.60e+04 0.0%      |
| Allocate3DMatrix(int, int, int)                                         | 1.60e+04 0.0%      |
| Allocate3DMatrix(int, int, int)                                         | 1.50e+04 0.0%      |
| Deallocate3DMatrix(float***, Parameters*)                               | 9.82e+03 0.0%      |
| Deallocate3DMatrix(float***, Parameters*)                               | 7.99e+03 0.0%      |

A função `iso_3dfd_it` original está descrita abaixo.

``` cpp
void iso_3dfd_it(float ***ptr_next, float ***ptr_prev, float ***ptr_vel,
float *coeff, const int n1, const int n2, const int n3) {
  for (int k = 0;k< n3; k++) {
    for (int j = 0; j<n2; j++) {
      for (int i = 0; i<n1; i++) {
        if (i >= HALF_LENGTH && i<(n1 - HALF_LENGTH) && j >= HALF_LENGTH &&
           j<(n2 - HALF_LENGTH) && k >= HALF_LENGTH && k<(n3 - HALF_LENGTH)) {
          float value = 0.0;
          value += ptr_prev[i][j][k] * coeff[0];
          for (int ir = 1; ir <= HALF_LENGTH; ir++) {
            value += coeff[ir] * (ptr_prev[i+ir][j][k] + ptr_prev[i-ir][j][k]);
            value += coeff[ir] * (ptr_prev[i][j+ir][k] + ptr_prev[i][j-ir][k]);
            value += coeff[ir] * (ptr_prev[i][j][k+ir] + ptr_prev[i][j][k-ir]);
          }
          ptr_next[i][j][k] = 2.0f* ptr_prev[i][j][k] - ptr_next[i][j][k]
		                    + value*ptr_vel[i][j][k];
        }
      }
    }
  }
}
```

Podemos perceber que existe um gargalo `if()` onde verifica-se os valores de i, j e k. Dessa forma podemos alterar o código para que não seja necessário entrar no loop e depois verificar se os valores são validos. Essa verificação pode ser feita de com o cada laço, podemos inciar o valor como `HALF_LENGTH` e depois verificar a cada interação se é menor do que `(n - HALF_LENGTH)`. Como mostra o código abaixo:

``` cpp
void iso_3dfd_it(float ***ptr_next, float ***ptr_prev, float ***ptr_vel,
float *coeff, const int n1, const int n2, const int n3){
  for (int k = HALF_LENGTH; k < n3 - HALF_LENGTH; k++){
    for (int j = HALF_LENGTH; j < n2 - HALF_LENGTH; j++){
      for (int i = HALF_LENGTH; i < n1 - HALF_LENGTH; i++){
        float value = 0.0;
        value += ptr_prev[i][j][k] * coeff[0];
        for (int ir = 1; ir <= HALF_LENGTH; ir++){
          value += coeff[ir] * (ptr_prev[i+ir][j][k] + ptr_prev[i-ir][j][k]);
          value += coeff[ir] * (ptr_prev[i][j+ir][k] + ptr_prev[i][j-ir][k]);
          value += coeff[ir] * (ptr_prev[i][j][k+ir] + ptr_prev[i][j][k-ir]);
        }
        ptr_next[i][j][k] = 2.0f* ptr_prev[i][j][k] - ptr_next[i][j][k]
		                  + value*ptr_vel[i][j][k];
      }
    }
  }
}
```

Entretanto, essa modificação não resultou em uma grande diferença visto que o número `HALF_LENGTH` é muito pequeno comparado ao tamanho do *array*. Dessa maneira o tempo de execução é de 1.37e+09 µsec. Apenas 10 segundos de diferença para a versão original
Chega-se a uma ultima versão utilizando blocagem. Como O código abaixo: 

``` cpp
void iso_3dfd_it(float ***ptr_next, float ***ptr_prev, float ***ptr_vel,
float *coeff, const int n1, const int n2, const int n3, int nb){
  for (int kk = HALF_LENGTH; kk < n3 - HALF_LENGTH; kk += nb) {
    for (int jj = HALF_LENGTH; jj < n2 - HALF_LENGTH; jj += nb) {
      for (int ii = HALF_LENGTH; ii < n1 - HALF_LENGTH; ii += nb){
        for (int k = kk; k < std::min(n3 - HALF_LENGTH, kk + nb); k++){
          for (int j = jj; j < std::min(n2 - HALF_LENGTH, jj + nb); j++){
            for (int i = ii; i < std::min(n1 - HALF_LENGTH, ii + nb); i++){
              float value = 0.0;
              value += ptr_prev[i][j][k] * coeff[0];
              for (int ir = 1; ir <= HALF_LENGTH; ir++) {
                value += coeff[ir] * (ptr_prev[i+ir][j][k] + ptr_prev[i-ir][j][k]);
                value += coeff[ir] * (ptr_prev[i][j+ir][k] + ptr_prev[i][j-ir][k]);
                value += coeff[ir] * (ptr_prev[i][j][k+ir] + ptr_prev[i][j][k-ir]);
              }
              ptr_next[i][j][k] = 2.0f* ptr_prev[i][j][k] - ptr_next[i][j][k]
			                    + value*ptr_vel[i][j][k];
            }
          }
        }
      }
    }
  }
}
```

A utilização da blocagem proporcionou uma melhora de desempenho bem maior do que a alteração do `if()` feita anteriormente. Utilizando o numero `nb` com 4, foi possível ter um tempo de execução igual à 7.98e+08 µsec. Uma melhora significativa, deixando o código 50% mais rápido que a versão original.