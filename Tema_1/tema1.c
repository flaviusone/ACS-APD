/**
*
* Author: Flavius Tirnacop
* Grupa: 341C1
* Fisier: serial.c
* Descriere: Implementare paralela Conway's Game of Life
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


/*-----  End of Includes & Defines  ------*/

int main(int argc, char **argv){

	/*==========  Variabile declarate  ==========*/
	FILE* f;
	char mode;
	unsigned int W_harta, H_harta,	/* Lungime si latime harta */
				 W, H,				/* Lungime si latime harta simulata */
					N;				/* Number of steps */
	register unsigned int			/* Aux vars */
				 i, j,
				 varaux_h,varaux_w,
				 k,l;
	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	unsigned short** auxmat;	/* Aux mat */
	char* buff = malloc(100*sizeof(char)); /* Aux var */

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
	matA = allocMat(W+2, H+2);
	matB = allocMat(W+2, H+2);

	/* Citire matrice din fisier */
	for(i = 1 ; i < H_harta+1 ; ++i){
		for(j = 1 ; j < W_harta+1 ; ++j){
			fscanf(f,"%hu",&matA[i][j]);
		}
		fgets(buff, INT_MAX, f);
	}
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

		/* Update sides if Toroid */
		if(mode == 'T'){
			update_toroid(matA, matA, W, H);
		}

		/* Varianta de baza */
		// #pragma omp parallel for collapse(2)\
		// shared(H,W,matA,matB)\
		// private(i,j) schedule(auto)
		// for(i = 1 ; i < H + 1 ; ++i)
		// 	for(j = 1 ; j < W + 1 ; ++j){
		// 		// printf("Thread %d cu %d %d\n", omp_get_thread_num(), i, j);
		// 		mutate(matA, matB, i, j);
		// 	}

		/* Varianta de test */
		varaux_h = H/atoi(argv[1]);
		varaux_w = W/atoi(argv[1]);

		/* Parcurgem matricea pe blocuri de W/n_threads H/n_threads
		 * pentru a reduce din cache miss-uri */
		#pragma omp parallel for private(i,j,k,l) schedule(static)
		for( k = 0 ; k < varaux_h ; ++k)
			for( l = 0 ; l < varaux_w ; ++l)
				for(i = 1 + k *  H / varaux_h; i < (k+1) *  H / varaux_h + 1 ; ++i)
					for(j = 1 + l * W / varaux_w ; j < (l+1) * W / varaux_w + 1 ; ++j)
						mutate(matA, matB, i, j, W+2, H+2);

		/* Swap the 2 matrices */
		auxmat = matA;
		matA = matB;
		matB = auxmat;

		N--;

	}
	W_harta = 1;
	H_harta = 1;

	/* Recalculam W_harta si H_harta */
	#pragma omp parallel for private(i,j) shared(matA,W_harta,H_harta) schedule(static)
	for (i = 1; i < H+1; ++i)
		for (j = 1; j < W+1; ++j)
			if(matA[i][j])
			{
				if(i>H_harta)H_harta = i;
				if(j>W_harta)W_harta = j;
			}

	/*-----  End of Secitune Algoritm  ------*/

	/* Scriem output in fisier */
	print_fisier(f, matA, mode, W_harta, H_harta, W, H);

	/* Inchidem fisierul de input */
	fclose(f);

	return 0;
}