/**
*
* Author: Flavius Tirnacop
* Grupa: 341C1
* Fisier: utils.h
* Descriere: Include functii ajutatoare pentru algoritmul principal
*
**/

#define GOL 0
#define INDIVID 1


/**
*
* Rezolva dependentele marginilor in cazul modului toroid
*
**/
static void update_toroid(unsigned short** matA, unsigned short** matB,
						unsigned int W, unsigned int H)
{
	register int i,j;

	/* Top & Bottom */
	#pragma omp parallel for private(j) shared(H, W, matA, matB)\
	schedule(static)
	for (j = 1; j < W+1; ++j){
		matB[0][j] = matB[H][j];
		matB[H+1][j] = matB[1][j];
	}

	/* Sides */
	#pragma omp parallel for private(i) shared(H, W, matA, matB)\
	schedule(static)
	for (i = 1; i < H+1; ++i){
		matB[i][0] = matB[i][W];
		matB[i][W+1] = matB[i][1];
	}

	/* Corners */
	matB[0][0] = matB[H][W];
	matB[H+1][W+1] = matB[1][1];
	matB[0][W+1] = matB[H][1];
	matB[H+1][0] = matB[1][W];

}

/**
*
* Functia mutate numara vecinii si evalueaza urmatoarea stare
* Evident citeste din o matrice si pune rezultatu in alta
*
**/
static unsigned int mutate(unsigned short** matA, unsigned short** matB,
	unsigned int i, unsigned int j, unsigned int W, unsigned int H){

	register unsigned short vecini = 0;

	/* Count vecini */
	unsigned short* pij = &matA[i][j];
	vecini += *(pij-W-1) + *(pij-W) + *(pij-W+1) ;
	vecini += *(pij-1) + *(pij+1);


	/* Incercam sa evitam op suplimentare daca deja s-au gasit destui vecini */
	if( vecini < 4){
		vecini += *(pij+W-1) + *(pij+W) + *(pij+W+1) ;
	}

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

/**
*
* Afiseaza matricea la consola.For debug purposes only.
*
**/
void print_consola(unsigned short** mat, char mode,
		 	unsigned int W_h, unsigned int H_h, unsigned int W, unsigned int H){
	int i,j;
	printf("%c %u %u %u %u\n", mode, W_h, H_h, W, H);
	for(i = 0 ; i < H+2 ; i++){
		for(j = 0 ; j < W+2 ; j++){
			printf("%hu ",mat[i][j]);
		}
		printf("\n");
	}
}

/**
*
* Scrie matricea in fisier
*
**/
void print_fisier(FILE* f, unsigned short** mat, char mode,
		 	unsigned int W_h, unsigned int H_h, unsigned int W, unsigned int H){
	int i,j;
	fprintf(f,"%c %u %u %u %u\n", mode, W_h, H_h, W, H);
	for(i = 1 ; i < H_h+1 ; i++){
		for(j = 1 ; j < W_h+1 ; j++){
			fprintf(f,"%hu ",mat[i][j]);
		}
		fprintf(f,"\n");
	}
}

/**
*
* Functie de alocare in zona continua de memorie
*
**/
unsigned short **allocMat(int w, int h)
{
    unsigned short **mat;
    unsigned short *buf;
    buf = (unsigned short *) calloc(h * w, sizeof(unsigned short));
    int i;
    mat = (unsigned short **) calloc(h, sizeof(unsigned short *));
    #pragma omp parallel for private(i) shared(buf, w, h) schedule(static)
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}

