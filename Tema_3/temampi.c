/*
* Nume: Flavius Tirnacop
* Grupa: 341C1
* Tema: Tema3 APD
*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
// alocare in zona continua de memorie
unsigned short **allocMat(int w, int h)
{
    unsigned short **mat;
    unsigned short *buf;
    buf = (unsigned short *) calloc(h * w, sizeof(unsigned short));
    int i;
    mat = (unsigned short **) calloc(h, sizeof(unsigned short *));
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}
/*
* Functie ajutatoare ce genereaza permutari ale vectorului perm
* From: http://groups.csail.mit.edu/
*/
void randperm(int n,int perm[])
{
	register int i, j, t;
	for(i=0; i<n; i++)
		perm[i] = i;
	for(i=0; i<n; i++) {
		j = rand()%(n-i)+i;
		t = perm[j];
		perm[j] = perm[i];
		perm[i] = t;
	}
}

int main (int argc, char* argv[]){

	MPI_Status Stat;
	register int i,j,aux;
	int rank,numtasks,rc,tag=1;	// Necesare pentru MPI
	int tip,				// 0 - Mandelbrot ; 1 - Julia
		MAX_STEPS,			// Numarul maxim de itaretii
		width,height,		// lungime si latime matrice imagine
		lungime_fragment,	// Numar de pixeli de procesat / proces
		*permutari,			// Vectorul cu permutari random
		*fragment_perm,		// Fragmentul cu permutari primit de proces
		pasi;
	double 	x_min,x_max,y_min,y_max, // Subspatiul planului complex
			resolution,		// Rezolutia subspatiului
			c_real,c_imag,	// Valoarea parametrului c
			*valori;
	register double z_real,z_imag;	// Valoare Z
	unsigned short **imagine; // Matricea pentru imaginea finala
	unsigned short *fragment; // Fragmentul cu date calculate de fiecare proces
	unsigned short color;
/**************************End Sectiune Declarari**************************/
/***************************Sectiune Initializari**************************/
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);



	/* Initializare random */
	srand (time(NULL));

/************************End Sectiune Initializari************************/
/*********************Sectiune RANK 0 (MASTER) initia*********************/
/*
*	Aici se citesc datele initiale din fisier si se trimit catre toate
*   procesele ramase.
*	Date initiale:
*	rezolutie,x_min,x_max,y_min,y_max,MAX_STEPS,lungime_fragment
*/
	if (rank == 0){

		/* Citire date intrare*/
		FILE *f = fopen(argv[1], "r");
		if (f == NULL){
			fprintf(stderr, "Fisierul de intrare nu a putut fi deschis\n");
			MPI_Finalize();
			return -1;
		}

		fscanf(f,"%d",&tip);
		fscanf(f,"%lf%lf%lf%lf",&x_min,&x_max,&y_min,&y_max);
		fscanf(f,"%lf%d",&resolution,&MAX_STEPS);
		if(tip == 1) // Daca este de tip Julia
			fscanf(f,"%lf%lf",&c_real,&c_imag);
		else{
			c_real = x_min;
			c_imag = y_min;
		}
		fclose(f);

		/* End Citire Date intrare */

		/* Construiesc vector de valori de trimis*/
		valori = (double*) calloc(7,sizeof(double));
		valori[0] = resolution;
		valori[1] = x_min;
		valori[2] = x_max;
		valori[3] = y_min;
		valori[4] = y_max;
		valori[5] = c_real;
		valori[6] = c_imag;

		/* Calcul Width si Height */
		width = (int)floor((x_max-x_min)/resolution);
		height = (int)floor((y_max-y_min)/resolution);

		/* Alocare matrice in zona continua de memorie */
		imagine = allocMat(width,height);

		/* Generare vector de permutari */
		permutari = (int *) calloc(width*height,sizeof(int));

		for(i=0;i<width*height;i++)
			permutari[i] = i;
		randperm(width*height,permutari);

		/* Calcul numar de pixeli de procesat / proces */
		lungime_fragment = width * height / numtasks;
		/* Construiesc vectorul de date pentru procesat pentru Rank 0 */
		fragment_perm = (int*) calloc(lungime_fragment,sizeof(int));
		for(i=0;i<lungime_fragment;i++)
			fragment_perm[i] = permutari[i];

		/* Trimite date initiale catre toate procesele */
		for(i=1;i<numtasks;i++){

			/* Trimit fragment de permutari */
			MPI_Send(&lungime_fragment, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(permutari+(lungime_fragment*i),lungime_fragment,
				MPI_INT,i,tag,MPI_COMM_WORLD);
			MPI_Send(&MAX_STEPS, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(&tip, 1, MPI_INT, i, tag, MPI_COMM_WORLD);

			/* Trimit rezolutie,x_min,x_max,y_min,y_max,MAX_STEPS*/
			MPI_Send(valori, 7, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);

		}
	}
/******************End Sectiune RANK 0 initiala (MASTER)*******************/
/*********************** Sectiune Other Ranks initial**********************/
/*
*	Aici se primesc datele initiale:
*	rezolutie,x_min,x_max,y_min,y_max,MAX_STEPS,fragment,lungime_fragment
*/
	else{

		/* Primesc fragment de permutari */
		MPI_Recv(&lungime_fragment, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&Stat);
		fragment_perm = (int*) calloc(lungime_fragment,sizeof(int));
		MPI_Recv(fragment_perm,lungime_fragment,MPI_INT,0,tag,MPI_COMM_WORLD,&Stat);

		MPI_Recv(&MAX_STEPS, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&Stat);
		MPI_Recv(&tip, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&Stat);

		valori = (double*) calloc(7,sizeof(double));
		/* Primesc rezolutie,x_min,x_max,y_min,y_max,MAX_STEPS*/
		MPI_Recv(valori, 7, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,&Stat);

		resolution = valori[0];
		x_min = valori[1];
		x_max = valori[2];
		y_min = valori[3];
		y_max = valori[4];
		c_real = valori[5];
		c_imag = valori[6];

	}
/*********************END Sectiune Other Ranks initial********************/

/************************ Sectiune Prelucrare date ***********************/

	fragment = (unsigned short*)calloc(lungime_fragment,
		sizeof(unsigned short));
	width = (int)floor((x_max-x_min)/resolution);
	height = (int)floor((y_max-y_min)/resolution);

	/* Rezolvare Manderblot */
	/*
	*	F(z) = z^2 + c
	*	c = c_real + c_imag*i
	*	z = z_real + z_imag*i
	*/
	if(tip==0){
		// MANDELBROT
		// Parcurg vectorul de elemente permutate
		for(i=0;i<lungime_fragment;i++){

			c_real = x_min + (fragment_perm[i]%width)*resolution;
			c_imag = y_min + (fragment_perm[i]/width)*resolution;
			z_real = z_imag = 0.0;
			pasi = 0;
			while(pasi<MAX_STEPS && ((z_real*z_real+z_imag*z_imag) < 4.0)){
				register double tmp;
				tmp = z_real*z_real - z_imag*z_imag + c_real;
				z_imag = 2*z_real*z_imag + c_imag;
				z_real = tmp;
				pasi++;
			}
			color = pasi % 256	;
			fragment[i] = color;
		}
	}else{
		// JULIA
		// Parcurg vectorul de elemente permutate

		for(i=0;i<lungime_fragment;i++){
			z_real = x_min + (fragment_perm[i]%width)*resolution;
			z_imag = y_min + (fragment_perm[i]/width)*resolution;
			pasi = 0;
			while(pasi<MAX_STEPS && ((z_real*z_real+z_imag*z_imag) < 4)){
				double tmp;
				tmp = z_real*z_real - z_imag*z_imag + c_real;
				z_imag = 2*z_real*z_imag + c_imag;
				z_real = tmp;
				pasi++;
			}
			color = pasi % 256;
			fragment[i] = color;
		}
	}


/******************** Sectiune RANK 0 finala (MASTER)*********************/
/*
*	Aici se primesc datele calculate de procese si se genereaza imaginea
*	finala care apoi se scrie in fisierul de output
*/
	if(rank == 0){

		/* Trec in matrice ce a procesat Rank 0 */
		for(i=0;i<lungime_fragment;i++){
			aux = permutari[i];
			(*imagine+aux)[0] = fragment[i];


		}
		for(i=1;i<numtasks;i++){
			/* Primesc vector permutari */
			MPI_Recv(fragment, lungime_fragment,
				MPI_UNSIGNED_SHORT, i, tag, MPI_COMM_WORLD,&Stat);
			/* pun elementele la loc in matrice */
			/* permutari ar trebui sa aibe aceeasi ordine cu fragment*/
			for(j=0;j<lungime_fragment;j++){
				aux = permutari[j+lungime_fragment*i];
				(*imagine+aux)[0] = fragment[j];
			}
		}

		/* Scriu datele in fisier */
		FILE *f = fopen(argv[2],"w");
		fprintf(f, "P2\n");
    	fprintf(f, "%d %d\n", width, height);
    	fprintf(f, "255\n");
    	for (i = 0; i < height; i++) {
	        for (j = 0; j < width; j++)
    	        fprintf(f, "%hu ", imagine[height-i-1][j]);
        	fprintf(f, "\n"); //- daca se vrea sa fie afisata ca matrice se decomenteaza aceasta linie
    	}
    	fclose(f);
	}

/******************* END Sectiune RANK 0 finala (MASTER)*******************/
/************************ Sectiune Other Ranks final***********************/
/*
*	Aici se trimit inapoi catre rank 0 datele calculate in etapa anterioara
*
*/
	else{
		/* Trimit fragment calculat */
		MPI_Send(fragment, lungime_fragment,
		 	MPI_UNSIGNED_SHORT, 0, tag, MPI_COMM_WORLD);
	}
/********************** END Sectiune Other Ranks final*********************/
	MPI_Finalize();
	return 0;
}