#!/bin/bash

REPEATS=100
command=$1
runtimes=()

for ((i=1; i<=$REPEATS; i++))
do
    runtime=$(./$command)
    runtimes+=($runtime)
done

avg=0
for ele in ${runtimes[@]}
do
    avg=$(echo "$avg + $ele" | bc -l)
done
avg=$(echo "$avg / $REPEATS" | bc -l)

echo "It takes $avg seconds to run $command"


