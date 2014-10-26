#!/bin/bash
T1=$(cat $1 | grep real | cut -f 2 | cut -c 3-7 | head -n 1)
T2=$(cat $1 | grep real | cut -f 2 | cut -c 3-7 | head -n 2 | tail -n 1)
T4=$(cat $1 | grep real | cut -f 2 | cut -c 3-7 | head -n 3 | tail -n 1)
T8=$(cat $1 | grep real | cut -f 2 | cut -c 3-7 | head -n 4 | tail -n 1)
S1=$(echo "scale=3;$T1/$T2" | bc)
S2=$(echo "scale=3;$T2/$T4" | bc)
S3=$(echo "scale=3;$T4/$T8" | bc -l)
S4=$(echo "scale=3;$S1+$S2+$S3" | bc)
S5=$(echo "scale=3;$S4/3" | bc)
echo $S5
