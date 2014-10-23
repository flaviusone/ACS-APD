/**
*
* Author: Flavius Tirnacop
* Grupa: 341C1
* Fisier: serial.c
* Descriere: Implementare seriala Conway's Game of Life
*
**/

/*==========  Includes & Defines  ==========*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
/*-----  End of Includes & Defines  ------*/

int main(int argc, char **argv){

	/*==========  Variabile declarate  ==========*/
	FILE* f;
	char mode;
	unsigned int W_harta, H_harta,	/* Lungime si latime harta */
				 W, H,				/* Lungime si latime harta simulata */
					N,				/* Number of steps */
			 threadNo,				/* Number of threads */
				 i, j;				/* Aux vars */

	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	/*-----  End of Variabile declarate  ------*/

	/*==============================================
	=            Citiri si initializari            =
	==============================================*/

	/* Citire number of threads */
	threadNo = atoi(argv[1]);

	/* Citeste Number of steps */
	N = atoi(argv[2]);

	/* Deschide fisier citire */
	f = fopen(argv[3], "r");
	if (f == NULL) {
    	perror("open");
    	exit(EXIT_FAILURE);
	}

	/* Citeste modul T sau P */
	fscanf(f, "%c", &mode);

	/* Citeste lungime si latime harti */
	fscanf(f, "%u", &W_harta);
	fscanf(f, "%u", &H_harta);
	fscanf(f, "%u", &W);
	fscanf(f, "%u", &H);

	/* Alocam matrice initiale */
	matA = (unsigned short **)calloc(H, sizeof(unsigned short *));
	matB = (unsigned short **)calloc(H, sizeof(unsigned short *));
	for (i = 0; i < H; i++) {
		matA[i] = (unsigned short *)calloc(W, sizeof(unsigned short));
		matB[i] = (unsigned short *)calloc(W, sizeof(unsigned short));
	}
	/* Citire matrice din fisier */
	for(i = 0 ; i < H_harta ; i++){
		for(j = 0 ; j < W_harta ; j++){
			fscanf(f,"%hu",&matA[i][j]);
		}
	}
	/* Deschide fisier pentru scriere */
	f = fopen(argv[4], "w+");
	if (f == NULL) {
    	perror("open");
    	exit(EXIT_FAILURE);
	}
	/*-----  End of Citiri & initializari  ------*/


	/*=========================================
	=            Secitune Algoritm            =
	=========================================*/

	while(N > 0){
		for(i = 0 ; i < H_harta+1 ; ++i){
			for(j = 0 ; j < W_harta+1 ; ++j){
				// Call Mutate
				// Daca este pe margine si a devenit alive W_harta/H_harta++
			}
		}


		N--;
	}

	print_fisier(f, matA, W, H);
	/*-----  End of Secitune Algoritm  ------*/


	/* Inchidem fisierul de input */
	fclose(f);
	return 0;
}



