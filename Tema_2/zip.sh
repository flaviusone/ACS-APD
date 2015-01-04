#!/bin/bash
rm *.zip
TIME=$(date +%H:%M:%S)
echo $TIME
zip Tema2_$TIME src/mapreduce/*.java build.xml README.txt