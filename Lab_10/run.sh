#!/bin/bash
# mpicc mutualex.c -o mutualex
# mpirun -np 10 ./mutualex

mpicc oets_mpi.c -o oets_mpi
mpirun -np 10 ./oets_mpi