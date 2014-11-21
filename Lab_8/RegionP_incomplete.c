///////////////////////////
// Region Labelling      //
// - paradigma HeartBeat //
///////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 10

/** Functie de calcul a maximului pentru problema */
int max(int **label, int **image, int i, int j, int m) {
    
    int max = label[i][j];
    if (i>0 && max<label[i-1][j] && image[i-1][j]) // vecinul de sus
	max = label[i-1][j];
    if (j>0 && max<label[i][j-1] && image[i][j-1]) // vecinul din stanga
	max = label[i][j-1];
    if (i<m-1 && max<label[i+1][j] && image[i+1][j]) // vecinul de jos
	max = label[i+1][j];
    if (j<N-1 && max<label[i][j+1] && image[i][j+1]) // vecinul din dreapta
	max = label[i][j+1];
    return max;
}

/** Functie necesara pentru afisarea rezultatului */
void print(int mat[N][N], int mat2[N][N]) {

    int i, j;
    
    printf("\n");
    for (i=0; i<N; i++)
	printf("___");
    printf("-\n");
    for (i=0; i<N; i++) {
	printf("|");
	for (j=0; j<N; j++)
	    if (mat2[i][j])
		printf("%2d ", mat[i][j]);
	    else
		printf("   ");
	printf("\b|\n");
    }
    for (i=0; i<N; i++)
	printf("---");
    printf("-\n");
}

/** Functie pentru verificarea algoritmului */
int checkLabel(int **label, int **image, int stripSize) {

    int change = 0;
    int i, j;
    int oldLabel;
    
    for (i=0; i<stripSize+2; i++)
	for (j=0; j<N; j++) {
	    oldLabel = label[i][j];
	    if (image[i][j] == 1)
		label[i][j] = max(label, image, i, j, stripSize+2);
	    if (label[i][j] != oldLabel)
		change = 1;
	}
    return change;
}

int main(int argc, char** argv) {

    int rank, size;
    MPI_Status status;
    int image[N][N], label[N][N]; // global image, global label
    int i, j, k, tmp, n;
    int stripSize;
    int **limage, **llabel; // local image, local label
    int change = 1;

    // init
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    stripSize = N / size;
    if (rank == size-1 && N % size != 0) {
	stripSize = N - stripSize * (size-1);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    // initializarea matricilor locale
    for (k=0; k<size; k++) {
	if (rank == k) {
	    limage = (int**)malloc((stripSize+2) * sizeof(int*));
	    if (limage == NULL) {
		printf("Process %d could not allocate any more memory\n", rank);
		MPI_Abort(MPI_COMM_WORLD, 7777);
	    }
	    llabel = (int**)malloc((stripSize+2) * sizeof(int*));
	    if (llabel == NULL) {
		printf("Process %d could not allocate any more memory\n", rank);
		MPI_Abort(MPI_COMM_WORLD, 7777);
	    }
	    for (i=0; i<stripSize+2; i++) {
    		limage[i] = (int*)malloc(N * sizeof(int));
		if (limage[i] == NULL) {
		    printf("Process %d could not allocate any more memory\n", rank);
		    MPI_Abort(MPI_COMM_WORLD, 7777);
		}
		llabel[i] = (int*)malloc(N * sizeof(int));
		if (llabel[i] == NULL){
		    printf("Process %d could not allocate any more memory\n", rank);
		    MPI_Abort(MPI_COMM_WORLD, 7777);
		}
	    }
	}
    }
    
    // procesul 0 este coordonator - se ocupa de initializare
    if (rank == 0) {    
	for (i=0; i<N; i++)
	    for (j=0; j<N; j++) {
		image[i][j] = random()%2;
		label[i][j] = i * N + j;
	    }
	
	printf("\nMatricea initiala...\n");
	print(label, image);
	    
	// initialize local image and local label
	for (i=0; i<stripSize; i++)
	    for (j=0; j<N; j++) {
		limage[i+1][j] = image[i][j];
		llabel[i+1][j] = label[i][j];	
	    }
	    
	// initialize the rest of the local labels and local images
	n = stripSize;
	for (k=1; k<size; k++) {
	    MPI_Recv(&tmp, 1, MPI_INT, k, 1, MPI_COMM_WORLD, &status); // receive stripSize from k
	    for (i=0; i<tmp; i++) {
	        MPI_Send(image[n+i], N, MPI_INT, k, 1, MPI_COMM_WORLD);
		MPI_Send(label[n+i], N, MPI_INT, k, 1, MPI_COMM_WORLD);
	    }
	    n += tmp;
	}
    } else {
	MPI_Send(&stripSize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	for (i=0; i<stripSize; i++) {
	    MPI_Recv(limage[i+1], N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	    MPI_Recv(llabel[i+1], N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}
    }
    
    for (i=0; i<N; i++) {
	limage[0][i] = limage[1][i];
	limage[stripSize+1][i] = limage[stripSize][i];
	llabel[0][i] = llabel[1][i];
	llabel[stripSize+1][i] = llabel[stripSize][i];
    }
    
    // algoritmul Heartbeat
    while (change) {
	MPI_Barrier(MPI_COMM_WORLD);

	// ---------------------------------------------------------------
	// TODO - trimite/primeste zonele de margine catre/dinspre vecini
	// ---------------------------------------------------------------
	
	MPI_Barrier(MPI_COMM_WORLD);
	tmp = checkLabel(llabel, limage, stripSize);
	if (tmp) {
	    while (tmp) {
		tmp = checkLabel(llabel, limage, stripSize);
	    }
	} else
	    change = 0;

	if (rank == 0) {
	    for (k=1; k<size; k++) {
		MPI_Recv(&tmp, 1, MPI_INT, k, 1, MPI_COMM_WORLD, &status);
		change = change || tmp;
	    }
	    for (k=1; k<size; k++) {
		MPI_Send(&change, 1, MPI_INT, k, 1, MPI_COMM_WORLD);
	    }
	} else {
	    MPI_Send(&change, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	    MPI_Recv(&change, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}
    } // end while 
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
	for (i=0; i<stripSize; i++)
	    for (j=0; j<N; j++) {
		image[i][j] = limage[i+1][j];
		label[i][j] = llabel[i+1][j];
	    }
	n = stripSize;
	for (k=1; k<size; k++) {
	    MPI_Recv(&tmp, 1, MPI_INT, k, 1, MPI_COMM_WORLD, &status); // receive stripSize from k
	    for (i=0; i<tmp; i++) {
		MPI_Recv(image[n+i], N, MPI_INT, k, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(label[n+i], N, MPI_INT, k, 1, MPI_COMM_WORLD, &status);
	    }
	    n += tmp;
	}
	printf("\nMatricea finala...\n");
	print(label, image);
    } else {
	MPI_Send(&stripSize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	for (i=0; i<stripSize; i++) {
	    MPI_Send(limage[i+1], N, MPI_INT, 0, 1, MPI_COMM_WORLD);
	    MPI_Send(llabel[i+1], N, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    for (i=0; i<stripSize+2; i++) {
	free(limage[i]);
	free(llabel[i]);
    }
    free(limage);
    free(llabel);
    
    MPI_Finalize();	
}


