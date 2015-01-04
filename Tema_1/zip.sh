#!/bin/bash
rm *.zip
TIME=$(date +%H:%M:%S)
echo $TIME
zip Tema1_$TIME tema1.c utils.h Makefile README