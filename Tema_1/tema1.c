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

	/* Tops */
	#pragma omp parallel for private(j) shared(H, W, matA, matB)\
	schedule(static)
	for (j = 1; j < W_harta+1; ++j){
		matB[0][j] = matB[H][j];
		matB[H+1][j] = matB[1][j];
	}
	// #pragma omp parallel for private(j) shared(H, W, matA, matB)\
	// schedule(static)
	// for (j = 1; j < W_harta+1; ++j)
	// 	matB[H+1][j] = matB[1][j];

	/* Sides */
	#pragma omp parallel for private(i) shared(H, W, matA, matB)\
	schedule(static)
	for (i = 1; i < H_harta+1; ++i){
		matB[i][0] = matB[i][W];
		matB[i][W+1] = matB[i][1];
	}
	// #pragma omp parallel for private(i) shared(H, W, matA, matB)\
	// schedule(static)
	// for (i = 1; i < H_harta+1; ++i)
	// 	matB[i][W+1] = matB[i][1];

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
	// unsigned short element;
	/* Count vecini */
	vecini = vecini + \
			matA[i-1][j-1] 	+ 	matA[i-1][j] 	+\
			matA[i-1][j+1] 	+ 	matA[i][j+1] 	+\
			matA[i][j-1] 	+ 	matA[i+1][j-1] 	+\
			matA[i+1][j] 	+	matA[i+1][j+1];

	// if(vecini > 0)
	// 	printf("Vecini [%d][%d] %d\n", i, j, vecini);
	// element = matA[i][j];

	// if(vecini == 2)
	// {
	// 	matB[i][j] = element;
	// 	return element;
	// }
	// if(vecini == 3)
	// {
	// 	matB[i][j] = INDIVID;
	// 	return INDIVID;
	// }
	// else
	// {
	// 	matB[i][j] = GOL;
	// 	return GOL;
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
	unsigned int 			/* Number of threads */
				 i, j, res,			/* Aux vars */
				 // flag_w, flag_h,	/* Aux vars */
				 dim_h, dim_w;		/* Aux vars */
	unsigned int flag_h[8] , flag_w[8];

	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	unsigned short** auxmat;	/* Aux mat */

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
	int chunk = W/atoi(argv[1]);
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
		#pragma omp parallel for collapse(2)\
		shared(H_harta,W_harta,matA,matB)\
		private(i,j) schedule(guided)
		for(i = 1 ; i < H_harta + 1 ; ++i)
			for(j = 1 ; j < W_harta + 1 ; ++j){
				// printf("Thread %d cu %d %d\n", omp_get_thread_num(), i, j);
				mutate(matA, matB, i, j);
			}



		/* Verificam marginile pentru a vedea daca
		 * trebuie extinsa suprafata actuala de verificare */
		// flag_h = flag_w = 0;
		memset(flag_h, 0 ,sizeof(flag_h));
		memset(flag_w, 0 ,sizeof(flag_w));

		/* Evitam depasirea matricei in cazul in
		 * care se ajunge la margini */
		if(W_harta >= W)
			dim_w = W_harta ;
		else
			dim_w = W_harta + 1;

		if(H_harta >= H)
			dim_h = H_harta;
		else
			dim_h = H_harta + 1;

		/* Verificam marginea inferioara*/
		#pragma omp parallel for private(i,res) shared(dim_w, flag_w, matA, matB)\
		schedule(static)
		for(i = 1 ; i < dim_h ; ++i){
			res = mutate(matA, matB, i, dim_w);
			if (res == INDIVID && !flag_w[omp_get_thread_num()]){
				flag_w[omp_get_thread_num()] = 1;
				// printf("BOOM flag[%d]\n", omp_get_thread_num());
			}
		}

		/* Evitam depasirea matricei in cazul in
		 * care se ajunge la margini */


		/* Verificam marginea din dreapta*/
		#pragma omp parallel for private(j,res) shared(dim_w, flag_h, matA, matB)\
		schedule(static)
		for(j = 1 ; j < dim_w ; ++j){
			res = mutate(matA, matB, dim_h, j);
			if (res == INDIVID && !flag_h[omp_get_thread_num()]){
				flag_h[omp_get_thread_num()] = 1;
				// printf("BOOM flag[%d]\n", omp_get_thread_num());
			}
		}
		for (i = 0; i < 8; ++i)
		{
			if(flag_h[i] && H_harta<H)
			{

				++H_harta;
				break;
			}
		}
		for (i = 0; i < 8; ++i)
		{
			if(flag_w[i] && W_harta<W)
			{
				++W_harta;
				break;
			}
		}
		// printf("H_harta %d W_harta %d\n",H_harta, W_harta );
		// if (flag_h && H_harta<H) ++H_harta;
		// if (flag_w && W_harta<W) ++W_harta;


		// print_consola(matA, mode, W_harta, H_harta, W, H);
		// getchar();


		/* Update sides if Toroid */
		if(mode == 'T'){
			update_sides(matA, matB, W_harta, H_harta, W, H);
		}

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
		// #pragma omp parallel for collapse(2) schedule(static) private(i,j)
  //       for(i = 0 ; i < H + 2 ; ++i)
		// 	for(j = 0 ; j < W + 2 ; ++j)
		// 		matB[i][j] = 0;


		N--;
	}
	W_harta = 0;
	H_harta = 0;

	// #pragma omp parallel for\
	// shared(H_harta,W_harta,matA)\
	// private(i,j) schedule(static)
	for (i = H_harta; i < H+1; ++i)
	{
		for (j = W_harta; j < W+1; ++j)
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



