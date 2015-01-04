#!/bin/bash

mpicc sondaj.c -o sondaj
mpirun -np 12 ./sondaj