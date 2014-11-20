#!/bin/bash
rm *.zip
TIME=$(date +%H:%M:%S)
echo $TIME
zip Tema1_$TIME src/ build.xml README.txt