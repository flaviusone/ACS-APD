#!/bin/bash
time ./tema1 1 1000 resurse/test2.txt test2.out.txt
diff -s resurse/test2.out.txt test2.out.txt
time ./tema1 2 1000 resurse/test2.txt test2.out.txt
diff -s resurse/test2.out.txt test2.out.txt
time ./tema1 4 1000 resurse/test2.txt test2.out.txt
diff -s resurse/test2.out.txt test2.out.txt
time ./tema1 8 1000 resurse/test2.txt test2.out.txt
diff -s resurse/test2.out.txt test2.out.txt

