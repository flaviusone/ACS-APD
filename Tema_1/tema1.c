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

#define GOL 0
#define INDIVID 1
/*-----  End of Includes & Defines  ------*/

/**
*
* Functia mutate numara vecinii si evalueaza urmatoarea stare
* Evident citeste din o matrice si pune rezultatu in alta
*
**/
unsigned int mutate(unsigned short** matA, unsigned short** matB,
	unsigned int i, unsigned int j){
	int vecini = 0;
	int k, l;
	unsigned short element;
	/* Count vecini */
	for(k = -1 ; k < 2 ; ++k)
		for(l = -1 ; l < 2 ; ++l)
			vecini += matA[i+k][j+l];

	element = matA[i][j];

	if (element == GOL)
	{
		if (vecini == 3)
		{
			matB[i][j] = INDIVID;
			return INDIVID;
		}
		else
		{
			matB[i][j] = GOL;
			return GOL;
		}
	}
	else
	{
		if (vecini == 2 || vecini == 3)
		{
			matB[i][j] = INDIVID;
			return INDIVID;
		}
		else
		{
			matB[i][j] = GOL;
			return GOL;
		}
	}


	return -1;
}

int main(int argc, char **argv){

	/*==========  Variabile declarate  ==========*/
	FILE* f;
	char mode;
	unsigned int W_harta, H_harta,	/* Lungime si latime harta */
				 W, H,				/* Lungime si latime harta simulata */
					N,				/* Number of steps */
			 threadNo,				/* Number of threads */
				 i, j, res, 		/* Aux vars */
				 flag_w, flag_h;	/* Aux vars */

	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	unsigned short** auxmat;	/* Aux mat */
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

	/* Alocam matrice initiale bordate cu 0*/
	matA = (unsigned short **)calloc(H+2, sizeof(unsigned short *));
	matB = (unsigned short **)calloc(H+2, sizeof(unsigned short *));
	for (i = 0; i < H+2; i++) {
		matA[i] = (unsigned short *)calloc(W+2, sizeof(unsigned short));
		matB[i] = (unsigned short *)calloc(W+2, sizeof(unsigned short));
	}
	/* Citire matrice din fisier */
	for(i = 1 ; i < H_harta+1 ; ++i){
		for(j = 1 ; j < W_harta+1 ; ++j){
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
		for(i = 1 ; i < H_harta ; ++i)
			for(j = 1 ; j < W_harta ; ++j)
				mutate(matA, matB, i, j);


		/* Verificam marginile pentru a vedea daca
		 * trebuie extinsa suprafata actuala de verificare */
		flag_h = flag_w = 0;
		for(i = 1 ; i < H_harta ; ++i){
			if(W_harta == W)
				res = mutate(matA, matB, i, W_harta);
			else
				res = mutate(matA, matB, i, W_harta + 1);
			if (res == INDIVID)
				flag_w = 1;
		}

		for(j = 1 ; j < W_harta ; ++j){
			if(H_harta == H)
				res = mutate(matA, matB, H_harta, j);
			else
				res = mutate(matA, matB, H_harta + 1, j);
			if (res == INDIVID)
				flag_h = 1;
		}

		if (flag_h && H_harta!=H) ++H_harta;
		if (flag_w && W_harta!=W) ++W_harta;

		/* Update sides if Toroid */
		// if(mode == "T"){
		// }

		/* Swap the 2 matrices */
		auxmat = matA;
		matA = matB;
		matB = auxmat;


		N--;
	}

	print_fisier(f, matA, mode, W_harta, H_harta, W, H);
	/*-----  End of Secitune Algoritm  ------*/


	/* Inchidem fisierul de input */
	fclose(f);
	return 0;
}



