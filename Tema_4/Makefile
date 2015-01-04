# Nume: Flavius Tirnacop
# Grupa: 341C1
# Makefile Tema 4

sursa = tema4.c
out = tema4
fisier_topologie = topologie
fisier_mesaje = mesaje
proc = 12


all:
	mpicc $(sursa) -o $(out) -Wall -lm
	mpiexec -n $(proc) ./$(out) $(fisier_topologie) $(fisier_mesaje)

build:
	mpicc $(sursa) -o $(out) -Wall -lm

.PHONY : clean
clean:
	rm -f $(out)
	rm proc*

