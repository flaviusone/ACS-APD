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
#include <omp.h>
#include <limits.h>

#define GOL 0
#define INDIVID 1
/*-----  End of Includes & Defines  ------*/

/**
*
* Functia mutate numara vecinii si evalueaza urmatoarea stare
* Evident citeste din o matrice si pune rezultatu in alta
*
**/
static void update_sides(unsigned short** matA, unsigned short** matB,
	unsigned int W_harta,unsigned int H_harta, unsigned int W,unsigned int H)
{
	register int i,j;

	/* Top & Bottom */
	#pragma omp parallel for private(j) shared(H, W, matA, matB)\
	schedule(static)
	for (j = 1; j < W_harta+1; ++j){
		matB[0][j] = matB[H][j];
		matB[H+1][j] = matB[1][j];
	}

	/* Sides */
	#pragma omp parallel for private(i) shared(H, W, matA, matB)\
	schedule(static)
	for (i = 1; i < H_harta+1; ++i){
		matB[i][0] = matB[i][W];
		matB[i][W+1] = matB[i][1];
	}

	/* Corners */
	#pragma omp parallel shared(matB, H, W)
	{
		matB[0][0] = matB[H][W];
		matB[H+1][W+1] = matB[1][1];
		matB[0][W+1] = matB[H][1];
		matB[H+1][0] = matB[1][W];
	}
}

static unsigned int mutate(unsigned short** matA, unsigned short** matB,
	unsigned int i, unsigned int j){
	unsigned short vecini = 0;
	/* Count vecini */
	vecini = vecini + \
			matA[i-1][j-1] 	+ 	matA[i-1][j] 	+\
			matA[i-1][j+1] 	+ 	matA[i][j+1] 	+\
			matA[i][j-1] 	+ 	matA[i+1][j-1] 	+\
			matA[i+1][j] 	+	matA[i+1][j+1];

	// if( vecini < 4){
	// 	vecini += matA[i+1][j-1] + matA[i+1][j]	+ matA[i+1][j+1];
	// }

	if (matA[i][j] == GOL)
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
	unsigned int 					/* Number of threads */
				 i, j,
				 k,l;				/* Aux vars */

	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	unsigned short** auxmat;	/* Aux mat */
	char* buff = malloc(sizeof(char));

	/*-----  End of Variabile declarate  ------*/

	/*==============================================
	=            Citiri si initializari            =
	==============================================*/

	/* Citire number of threads */
	omp_set_num_threads(atoi(argv[1]));

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
	// int chunk = W/atoi(argv[1]);
	if(W_harta > W) W_harta = W;
	if(H_harta > H) H_harta = H;

	/* Alocam matrice initiale bordate cu 0*/
	// matA = allocMat(W+2, H+2);
	// matB = allocMat(W+2, H+2);
	matA = (unsigned short **)calloc(H+2, sizeof(unsigned short *));
	matB = (unsigned short **)calloc(H+2, sizeof(unsigned short *));

	#pragma omp parallel for private(i)
	for (i = 0; i < H+2; i++) {
		matA[i] = (unsigned short *)calloc(W+2, sizeof(unsigned short));
		matB[i] = (unsigned short *)calloc(W+2, sizeof(unsigned short));
	}
	/* Citire matrice din fisier */
	for(i = 1 ; i < H_harta+1 ; ++i){
		for(j = 1 ; j < W_harta+1 ; ++j){
			fscanf(f,"%hu",&matA[i][j]);
		}
		fgets(buff, INT_MAX, f);
	}
	fclose(f);

	/* Deschide fisier pentru scriere */
	f = fopen("stuff.vmr", "a");
	if (f == NULL) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	fprintf(f,"%c %u %u %u %u\n", mode, W_harta, H_harta, W, H);

	fclose(f);

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

		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();

		/* Update sides if Toroid */
		if(mode == 'T'){
			update_sides(matA, matA, W, H, W, H);
		}

		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();


		/* Varianta de baza */
		// #pragma omp parallel for collapse(2)\
		// shared(H,W,matA,matB)\
		// private(i,j) schedule(static)
		// for(i = 1 ; i < H + 1 ; ++i)
		// 	for(j = 1 ; j < W + 1 ; ++j){
		// 		// printf("Thread %d cu %d %d\n", omp_get_thread_num(), i, j);
		// 		mutate(matA, matB, i, j);
		// 	}

		/* Varianta de test */
		int varaux = 2 * atoi(argv[1]);
		#pragma omp parallel for collapse(2) private(i,j,k,l) schedule(static)
		for( k = 0 ; k < varaux ; k++){
			for( l = 0 ; l < varaux ; l++){
				for(i = 1 + k *  H / varaux; i < (k+1) *  H / varaux + 1 ; ++i)
					for(j = 1 + l * W / varaux ; j < (l+1) * W / varaux + 1 ; ++j){
						mutate(matA, matB, i, j);
						// printf("Thread %d cu %d %d\n", omp_get_thread_num(), i, j);
					}
			}
		}

		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();



		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();

		/* Swap the 2 matrices */
		auxmat = matA;
		matA = matB;
		matB = auxmat;

		// #pragma omp parallel for schedule(static) private(i)
		// for (i = 0; i < H+2; i++) {
		// 	memset(matB[i], 0 , (W+2) * sizeof(unsigned short));
		// }

		N--;

	}
	W_harta = 1;
	H_harta = 1;

	/* Recalculam W_harta si H_harta */
	#pragma omp parallel for private(i,j) shared(matA,W_harta,H_harta) schedule(static)
	for (i = 1; i < H+1; ++i)
	{
		for (j = W_harta; j < W+1; ++j)
		{

			if(matA[i][j]){
				if(i>H_harta)H_harta = i;
				if(j>W_harta)W_harta = j;
			}
		}
	}

	print_fisier(f, matA, mode, W_harta, H_harta, W, H);
	/*-----  End of Secitune Algoritm  ------*/


	/* Inchidem fisierul de input */
	fclose(f);
	return 0;
}