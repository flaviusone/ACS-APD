#include<stdio.h>
#include<stdlib.h>

#define N 1048576 // needs to be power of 2

merge(int A[], int Left, int Right, int End, int B[])
{
	int iL = Left;
	int iR = Right;
	int j;

	for (j = Left; j < End; j++)
	{
		if (iL < Right && (iR >= End || A[iL] <= A[iR])) {
			B[j] = A[iL];
			iL++;
		} else {
			B[j] = A[iR];
			iR++;
		}
	}
}

int main(int argc, char argv[]) {
	srand(42);
	int *v = (int*)malloc(sizeof(int)*N);
	int *vNew = (int*)malloc(sizeof(int)*N);
	int *aux;
	int i, width;
	int sorted = 0;

	// generate the vector v with random values
	for(i = 0; i < N; i++) {
		v[i] = rand()%N;
	}

	// sort the vector v
	for (width = 1; width < N; width = 2 * width) {
		for (i = 0; i < N; i = i + 2 * width) {
			merge(v, i, i+width, i + 2*width, vNew);
		}
		aux = v;
		v= vNew;
		vNew=aux;
	}

	// display the vector v
	for(i = 0; i < N; i++) {
		printf("%i\t", v[i]);
	}
	printf("\n");

	int sw=0;
	for(i = 0; i < N-1; i++)
		if(v[i]>v[i+1])
			sw=1;
	if(sw)
		printf("Sorted incorrectly\n");
	else
		printf("Sorted correctly\n");

}