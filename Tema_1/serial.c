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
/*-----  End of Includes & Defines  ------*/

int main(int argc, char **argv){

	/*==========  Variabile declarate  ==========*/
	FILE* f;
	char mode;
	unsigned int W_harta, H_harta,	/* Lungime si latime harta */
				 W, H,				/* Lungime si latime harta simulata */
				 i, j;				/* Aux vars */

	unsigned char** matA, matB;		/* Matrice de lucru */
	/*-----  End of Variabile declarate  ------*/

	/*==============================================
	=            Citiri si initializari            =
	==============================================*/

	/* Deschide fisier citire */
	f = fopen(argv[1], "r");

	/* Citeste modul T sau P */
	fscanf(f, "%c", mode);

	/* Citeste lungime si latime harti */
	fscanf(f, "%u", &W_harta);
	fscanf(f, "%u", &H_harta);
	fscanf(f, "%u", &W);
	fscanf(f, "%u", &H);

	/* TODO - Alocam matrice initiale */

	/* TODO - Citire matrice din fisier */


	/*-----  End of Citiri & initializari  ------*/



	return 0;
}



comm