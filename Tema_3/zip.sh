#!/bin/bash
rm *.zip
TIME=$(date +%H:%M:%S)
echo $TIME
zip Tema3_$TIME temampi.c Makefile README