#include "mpi.h"
#include <stdio.h>

#define MAX(A,B)       ((A)>(B)?(A):(B))

int main(int argc, char *argv[]){

	int k,j,pas;
	int numtasks,rank,dest,source,rc,count,tag = 1,i;
	int out_number,in_number;
	int a[] = {1,2,7,3,4,18,10,-11,23,2};
	int b[] = {0,0,0,0,0,0,0,0,0,0};
	int n = 10;
	int element;
	int aux;
	int ca=0, cb=0;
	MPI_Status Stat;

	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Scatter(&a,1,MPI_INT,&element,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank == 0){
		printf("Vector nesortat: ");
		for (i = 0; i<n;i++)
		{
			printf(" %d", a[i]);
		}
		printf("\n");
	}
	/* ca - ceasul meu */
	/* cb - ceasul primit */

	for(pas = 0 ; pas< n ; pas++){
		for(j= pas%2; j<n-1;j+=2){
				//printf("pas = %d j = %d k= %d\n",pas,j,k );
			if(rank == j){

				MPI_Send(&element, 1, MPI_INT, j+1, tag, MPI_COMM_WORLD);
				ca++;
				MPI_Send(&ca, 1, MPI_INT, j+1, tag, MPI_COMM_WORLD);

				MPI_Recv(&aux, 1, MPI_INT, j+1, tag, MPI_COMM_WORLD,&Stat);
				ca++;
				MPI_Recv(&cb, 1, MPI_INT, j+1, tag, MPI_COMM_WORLD,&Stat);
				ca = MAX(ca,cb) + 1;
				if(aux<element) {
					element = aux;
					ca++;
				}

			}
			if(rank == j+1){
				MPI_Recv(&aux, 1, MPI_INT, j, tag, MPI_COMM_WORLD,&Stat);
				ca++;
				MPI_Recv(&cb, 1, MPI_INT, j, tag, MPI_COMM_WORLD,&Stat);

				ca = MAX(ca,cb) + 1;
				MPI_Send(&element, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
				ca++;
				MPI_Send(&ca, 1, MPI_INT, j, tag, MPI_COMM_WORLD);


				if(aux>element) {
					element = aux;
					ca++;
				}
				// printf("Rank %d ca=%d cb=%d\n", rank, ca, cb);
			}
			printf("Rank %d ca=%d cb=%d\n", rank, ca, cb);
			MPI_Barrier(MPI_COMM_WORLD);

		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Gather(&element,1,MPI_INT,&b,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank == 0){
		printf("Vector Sortat: \t");
		for (i = 0; i<n;i++)
		{
			printf(" %d", b[i]);
		}
		printf("\n");
	}


	MPI_Finalize();
}