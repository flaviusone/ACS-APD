#include "mpi.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int rc, numtasks, rank, tag;
	int token=0;
	int element;
	rc = MPI_Init(&argc,&argv);
	MPI_Status Stat;

	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if ( rank == 1 )
	{
		token = 1;
	}

	while(1)
	{
		/* Helperi */
		if( rank % 2 == 1)
		{
			if(token==0){
				//fac recieve
				if(rank==1){
					MPI_Recv(&token, 1, MPI_INT, numtasks-1, tag, MPI_COMM_WORLD,&Stat);
				}else{
					//daca sunt 0 fac de la rank maxim
					MPI_Recv(&token, 1, MPI_INT, rank-2, tag, MPI_COMM_WORLD,&Stat);
				}
			}

			printf("Sunt helper %d si trimit ok userului meu\n", rank);
			sleep(1);
			element = 5;
			MPI_Send(&element, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD);
			MPI_Recv(&element, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD,&Stat);


			if(rank==numtasks-1){
				/* Daca sunt 9 trimit lui 1 */
				MPI_Send(&token, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

			}else{
				/* Trimit la rank + 2 */
				MPI_Send(&token, 1, MPI_INT, rank+2, tag, MPI_COMM_WORLD);
			}
			token=0;

		}
		/* Useri */
		else
		{
			MPI_Recv(&element, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD,&Stat);
			printf("Sunt proces cu rank %d si fac stuff\n",rank);
			sleep(1);
			MPI_Send(&element, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);

		}
	}

	// printf("Rank %d\n", rank);

	MPI_Finalize();
}