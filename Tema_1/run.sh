#!/bin/bash

time ./tema1 1 1000 resurse/test1.txt test1.out.txt
diff -s resurse/test1.out.txt test1.out.txt
time ./tema1 2 1000 resurse/test1.txt test1.out.txt
diff -s resurse/test1.out.txt test1.out.txt
time ./tema1 4 1000 resurse/test1.txt test1.out.txt
diff -s resurse/test1.out.txt test1.out.txt
time ./tema1 8 1000 resurse/test1.txt test1.out.txt
diff -s resurse/test1.out.txt test1.out.txt
