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
/*-----  End of Includes & Defines  ------*/

int main(int argc, char **argv){

	/*==========  Variabile declarate  ==========*/
	FILE* f;
	char mode;
	unsigned int W_harta, H_harta,	/* Lungime si latime harta */
				 W, H,				/* Lungime si latime harta simulata */
				 i, j;				/* Aux vars */

	unsigned short** matA;		/* Matrice de lucru veche*/
	unsigned short** matB;		/* Matrice de lucru noua*/
	/*-----  End of Variabile declarate  ------*/

	/*==============================================
	=            Citiri si initializari            =
	==============================================*/

	/* Deschide fisier citire */
	f = fopen(argv[1], "r");
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

	for(i = 0 ; i < H_harta ; i++){
		for(j = 0 ; j < W_harta ; j++){
			printf("%hu\t",matA[i][j]);
		}
		printf("\n");
	}

	/* Inchidem fisierul de input */
	fclose(f);

	/* TODO - Deschide fisier de scriere */

	/*-----  End of Citiri & initializari  ------*/



	return 0;
}



