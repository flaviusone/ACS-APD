/*
Calcul polinom in banda de asamblare
*/
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv){

	int numtasks,rank,dest,source,rc,count,tag = 1;
	int x = 3;
	int a = 2,
		b = 4,
		c = 5,
		d = 6,
		e = 7;
	int out_number,in_number;
	MPI_Status Stat;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0){
		out_number = a*x;
		dest = rank+1;
		rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}else if(rank == 1){
		source = rank -1;
		dest = rank+1;
		rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
		out_number = (in_number+b)*x;
		rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}else if(rank == 2){
		source = rank -1;
		dest = rank+1;
		rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
		out_number = (in_number+c)*x;
		rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}else if(rank == 3){
		source = rank -1;
		dest = rank+1;
		rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
		out_number = (in_number+d)*x;
		rc = MPI_Send(&out_number, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}else if(rank == 4){
		source = rank -1;
		dest = rank+1;
		rc = MPI_Recv(&in_number, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&Stat);
		out_number = in_number+e;
		printf("Rezultat %d\n", out_number);
	}
	MPI_Finalize();
}