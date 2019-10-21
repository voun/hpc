#!/bin/bash

REPEATS=100
command=$1
runtimes1=()
runtimes2=()

for ((i=1; i<=$REPEATS; i++))
do

    arr=($(./$command))
    
    runtimes1+=(${arr[0]})
    runtimes2+=(${arr[1]})
done

avg1=0
avg2=0
for ((i=0; i< $REPEATS; i++))
do

    avg1=$(echo "$avg1 + ${runtimes1[$i]}" | bc -l)
    avg2=$(echo "$avg2 + ${runtimes2[$i]}" | bc -l)

done
avg1=$(echo "$avg1 / $REPEATS" | bc -l)
avg2=$(echo "$avg2 / $REPEATS" | bc -l)

echo "$avg1 and $avg2 for $command"
