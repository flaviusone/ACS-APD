#include "mpi.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int numtasks,rank,dest,source,rc,count,tag = 1,i,j,k;
	rc = MPI_Init(&argc,&argv);
	MPI_Status Stat;
	int mat[12][12] =
	{
		0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	};
	int n = 12;
	int sondaj = 1;
	int vecini = 0;
	int parinte, aux;

	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	/* Stergem din matrice stuff */
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j){
			if(i != rank){
				mat[i][j] = 0;
			}
		}
	}

	if( rank == 0 )
	{
		for(i=1;i<n;i++)
		{
			if(mat[0][i]==1){
				//Fac send sondaj
				MPI_Send(&sondaj, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
				vecini++;
			}
		}
	}
	else
	{
		MPI_Recv(&sondaj, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,&Stat);
		parinte = Stat.MPI_SOURCE;
		for(i=0;i<n;i++){
			if(i!=parinte && mat[rank][i]==1){
				//Send sondaj
				MPI_Send(&sondaj, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
				vecini++;
			}
		}
	}

	if( rank == 0 )
	/* Primesc matricea finala de la vecinii mei */
	{

		// for dupa vecini
		for(i = 0 ; i < n ; ++i){

			// for dupa matrice
			for (j = 0; j < n; ++j)
			{
				for (k = 0; k < n; ++k)
				{
					/*  Daca e vecin si nu e parinte recieve */
					if(mat[rank][i] == 1 && i != parinte)
					MPI_Recv(&aux, 1, MPI_INT, i, tag, MPI_COMM_WORLD,&Stat);
					/* Updatez matrice */
					if(aux == 1)
						mat[j][k] = 1;
				}
			}

		}
	}
	else
	{
		if( vecini == 0)
		/* Trimit direct matricea catre parinte */
		{
			for (i = 0; i < n; ++i){
				for(j = 0 ; j < n ; j++){
					MPI_Send(&mat[i][j], 1, MPI_INT, parinte, tag, MPI_COMM_WORLD);
				}
			}
		}
		else
		/* Intai astept de la vecini apoi trimit matricea*/
		{
			/* Astept de la vecini */

			// for dupa vecini
			for(i = 0 ; i < n ; ++i){

				// for dupa matrice
				for (j = 0; j < n; ++j)
				{
					for (k = 0; k < n; ++k)
					{
						/*  Daca e vecin si nu e parinte recieve */
						if(mat[rank][i] == 1 && i != parinte)
						MPI_Recv(&aux, 1, MPI_INT, i, tag, MPI_COMM_WORLD,&Stat);
						/* Updatez matrice */
						if(aux == 1)
							mat[j][k] = 1;
					}
				}

			}

			/* Trimit toata matricea */
			for (i = 0; i < n; ++i){
				for(j = 0 ; j < n ; j++){
					MPI_Send(&mat[i][j], 1, MPI_INT, parinte, tag, MPI_COMM_WORLD);
				}
			}
		}

	}


	if(rank==0) {
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j){
				printf("%d ", mat[i][j]);
			}
			printf("\n");
		}
		printf("Done\n");
	}
	MPI_Finalize();
	return 0;
}