/******************************************************************************
* FILE: sort.c
* AUTHOR: Cristian Chilipirea
* Generates serially sorts and displays a vector
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 4


int cmpfunc1 (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int cmpfunc2 (const void * a, const void * b)
{
   return ( *(int*)b - *(int*)a );
}


int main(int argc, char argv[]) {
	srand(42);
	int v[N][N];
	int i,j,pas,k;
	int sorted = 0;
	int aux;
	omp_set_num_threads(4);
	// generate the vector v with random values
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			v[i][j] = rand()%N;

	// sort the matrix vector
	for (pas = 0; pas < 2*log2(N*N) ; ++pas)
	{
		if(pas % 2 == 0)
		{
			/* pas par */
			#pragma omp parallel for private(i)
			for (i = 0; i < N; ++i)
			{
				if(i % 2 == 0)
				{
					/* sort asc */
					qsort(v[i], N, sizeof(int), cmpfunc1);
				}
				else
				{
					/* sort desc */
					qsort(v[i], N, sizeof(int), cmpfunc2);
				}
			}
		}
		else
		{
			/* pas impar */
			#pragma omp parallel for private(i,j,k,sorted)
			for (i = 0; i < N; ++i){
				/* naive sort */
				for (j = 0 ; j < N-1 ; ++j)
				{
					for (k = j+1 ; k < N ; ++k)
					{
						if(v[j][i] > v[k][i])
						{
							aux = v[j][i];
							v[j][i] = v[k][i];
							v[k][i] = aux;
						}
					}
				}
			}
		}
		#pragma omp barrier



	}


	printf("Dupa %d\n", pas);
	for ( i = 0; i < N; ++i)
	{
		for ( j = 0; j < N; ++j)
		{
			printf("%d ",v[i][j] );
		}
		printf("\n");
	}

	// // display the vector v
	// for(i = 0; i < N; i++) {
	// 	if(i%2) {
	// 		for(j = 0; j < N; j++) {
	// 			printf("%i\t", v[i][j]);
	// 		}
	// 	} else {
	// 		for(j = N-1; j >= 0; j--) {
	// 			printf("%i\t", v[i][j]);
	// 		}
	// 	}
	// }
	printf("\n");
}