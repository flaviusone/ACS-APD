# Nume: Flavius Tirnacop
# Grupa: 341C1
# Makefile Tema 3

sursa = mjlonir.c
out = main
file_in = in/mandelbrot2.in
file_out = out
proc = 4


all:
	mpicc $(sursa) -o $(out) -Wall -lm
	mpiexec -n $(proc) ./$(out) $(file_in) $(file_out)

build:
	mpicc $(sursa) -o $(out) -Wall -lm

.PHONY : clean
clean:
	rm -f $(out)

