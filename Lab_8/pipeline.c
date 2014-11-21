#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv){
	int numtasks,rank,dest,source,rc,count,tag = 1;
	int x = 3;
	int coef[5] = {2,4,5,6,7};

	int out_number,in_number;
	MPI_Status Stat;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0){
		out_number = coef[rank];
		dest = rank+1;
		rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

	}else if(rank == 4){
		source = rank - 1;
		dest = rank + 1;
		rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
		out_number = (in_number*x+coef[rank]);
		printf("Rezultat %d\n", out_number);
	}else{
	source = rank -1;
	dest = rank + 1;
	rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
	out_number = (in_number*x+coef[rank]);
	rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}