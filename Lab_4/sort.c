/******************************************************************************
* FILE: sort.c
* AUTHOR: Cristian Chilipirea
* Generates serially sorts and displays a vector
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

int main(int argc, char argv[]) {
	srand(42);
	int v[N];
	int i, j ,pas;
	int sorted = 0;
	int aux;

	omp_set_num_threads(4);
	// generate the vector v with random values
	for(i = 0; i < N; i++) {
		v[i] = rand()%N;
	}

	// sort the vector v
	// while(!sorted) {
	// 	sorted = 1;
	// 	for(i = 0; i < N-1; i++) {
	// 		if(v[i] > v[i + 1]) {
	// 			aux = v[i];
	// 			v[i] = v[i + 1];
	// 			v[i + 1] = aux;
	// 			sorted = 0;
	// 		}
	// 	}
	// }

	#pragma omp parallel private(pas)
	{
		for ( pas = 1 ; pas <= N ; pas++){
			#pragma omp for private(j,aux)
			for(j = pas % 2 ; j < N ; j += 2)
			{
				if(v[j]>v[j + 1]){
					aux = v[j];
					v[j] = v[j + 1];
					v[j + 1] = aux;
				}
			}
		}
	}
	// display the vector v
	for(i = 0; i < N; i++) {
		printf("%i\t", v[i]);
	}
	printf("\n");
}