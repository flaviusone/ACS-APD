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
	unsigned short element;
	/* Count vecini */
	vecini += matA[i-1][j-1]; 	vecini += matA[i][j+1];
	vecini += matA[i-1][j];		vecini += matA[i+1][j-1];
	vecini += matA[i-1][j+1];	vecini += matA[i+1][j];
	vecini += matA[i][j-1];		vecini += matA[i+1][j+1];

	// if(vecini > 0)
	// 	printf("Vecini [%d][%d] %d\n", i, j, vecini);
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
					N;				/* Number of steps */
	register unsigned int threadNo,	/* Number of threads */
				 i, j, res,			/* Aux vars */
				 flag_w, flag_h,	/* Aux vars */
				 dim_h, dim_w;		/* Aux vars */

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
		for(i = 1 ; i < H_harta + 1 ; ++i)
			for(j = 1 ; j < W_harta + 1 ; ++j)
				mutate(matA, matB, i, j);



		/* Verificam marginile pentru a vedea daca
		 * trebuie extinsa suprafata actuala de verificare */
		flag_h = flag_w = 0;

		/* Evitam depasirea matricei in cazul in
		 * care se ajunge la margini */
		if(W_harta == W)
			dim_w = W_harta ;
		else
			dim_w = W_harta + 1;

		if(H_harta == H)
			dim_h = H_harta;
		else
			dim_h = H_harta + 1;

		/* Verificam marginea inferioara*/
		for(i = 1 ; i < dim_h ; ++i){
			res = mutate(matA, matB, i, dim_w);
			if (res == INDIVID)
				flag_w = 1;
		}

		/* Evitam depasirea matricei in cazul in
		 * care se ajunge la margini */


		/* Verificam marginea din dreapta*/
		for(j = 1 ; j < dim_w ; ++j){
			res = mutate(matA, matB, dim_h, j);
			if (res == INDIVID)
				flag_h = 1;
		}


		// printf("H_harta %d W_harta %d\n",H_harta, W_harta );
		if (flag_h && H_harta<H) ++H_harta;
		if (flag_w && W_harta<W) ++W_harta;


		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();


		/* Update sides if Toroid */
		if(mode == 'T'){

				/* Tops */
				for (j = 1; j < W+1; ++j)
					matB[0][j] = matB[H][j];
				for (j = 1; j < W+1; ++j)
					matB[H+1][j] = matB[1][j];

				/* Sides */
				for (i = 1; i < H+1; ++i)
					matB[i][0] = matB[i][W];
				for (i = 1; i < H+1; ++i)
					matB[i][W+1] = matB[i][1];

				matB[0][0] = matB[H][W];
				matB[H+1][W+1] = matB[1][1];
				matB[0][W+1] = matB[H][1];
				matB[H+1][0] = matB[1][W];
		}

		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();

		/* Swap the 2 matrices */
		auxmat = matA;
		matA = matB;
		matB = auxmat;

		N--;
	}
	W_harta = 0;
	H_harta = 0;
	for (i = 1; i < H+1; ++i)
	{
		for (j = 1; j < W+1; ++j)
		{
			if(matA[i][j]){
				W_harta = j;
				H_harta = i;
			}
		}
	}

	print_fisier(f, matA, mode, W_harta, H_harta, W, H);
	/*-----  End of Secitune Algoritm  ------*/


	/* Inchidem fisierul de input */
	fclose(f);
	return 0;
}



