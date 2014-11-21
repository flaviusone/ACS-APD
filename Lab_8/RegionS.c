//////////////////////////////////////////
// Region Labelling			//
// - implementarea seriala a rezolvarii //
//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define N 10

/** Functie necesara pentru afisarea rezultatelor partiale obinute */
void print(int mat[N][N], int mat2[N][N]) {

    int i, j;
    
    printf("\n");
    for (i=0; i<N; i++)
	printf("___");
    printf("_\n");
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

/** Functie de calcul a maximului pentru problema */
int max(int mat[N][N], int image[N][N], int i, int j) {

    int max = mat[i][j];
    if (i > 0 && max < mat[i-1][j] && image[i-1][j]) // vecinul de sus
	max = mat[i-1][j];
    if (j > 0 && max < mat[i][j-1] && image[i][j-1]) // vecinul din stanga
	max = mat[i][j-1];
    if (i < N-1 && max < mat[i+1][j] && image[i+1][j]) // vecinul de jos
	max = mat[i+1][j];
    if (j < N-1 && max < mat[i][j+1] && image[i][j+1]) // vecinul din dreapta
	max = mat[i][j+1];
    return max;
}

int main(int argc, char** argv) {

    int image[N][N];
    int label[N][N];
    int i, j;
    int change = 1;
    int oldLabel;
    int pas = 0;

    // initializarea matricii si a etichetelor
    for (i=0; i<N; i++)
	for (j=0; j<N; j++) {
	    image[i][j] = random()%2;
	    label[i][j] = i * N + j;
	}
    printf("\nLa pasul %d...\n", pas++);
    print(label, image);
    
    // algoritmul de rezolvare
    while (change) {
	change = 0;
	for (i=0; i<N; i++)
	    for (j=0; j<N; j++) {
		oldLabel = label[i][j];
		if (image[i][j] == 1)
		    label[i][j] = max(label, image, i, j);
		if (label[i][j] != oldLabel)
		    change = 1;
	    }
	printf("\nLa pasul %d...\n", pas++);
	print(label, image);
    }
}

