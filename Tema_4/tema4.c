/*
* Nume: Flavius Tirnacop
* Grupa: 341C1
* Tema: Tema4 APD
*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAXVAL 999
#define part3 0

// alocare in zona continua de memorie
int **allocMat(int w, int h)
{
    int **mat;
    int *buf;
    buf = (int *) calloc(h * w, sizeof(int));
    int i;
    mat = (int **) calloc(h, sizeof(int *));
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}

int main (int argc, char* argv[]){

	MPI_Status Stat;
	int i,j,k;
	int rank,numtasks,rc,tag=1,	// Necesare pentru MPI
		sondaj=1,
		vecini=0,
		parinte=-1,
		aux=-1,
		end=-2,
		wakeup=1,
		next_hop[100];
	FILE *f,*g;
	char buff[256];


/****************************End Sectiune Declarari****************************/
/*****************************Sectiune Initializari****************************/
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Declar matrice pentru distante
	int d[numtasks][numtasks];

	// Deschid fisier de citire topologie
	f = fopen(argv[1], "r");
	// Deschid fisier scriere output
	sprintf(buff,"proc%d_out",rank);
	g = fopen(buff,"w");

	// Initializare seed random
	srand(time(NULL)*rank);

	// Initializam matrici topologie
	int **topologie,**topologie_null,**topologie_aux;
	topologie 		= allocMat(numtasks+1,numtasks+1);
	topologie_null 	= allocMat(numtasks+1,numtasks+1);
	topologie_aux 	= allocMat(numtasks+1,numtasks+1);

	// Initializam vectori vot de lungime numtasks+1
	// in ultimul camp vom pune date aditionale pentru a nu fi nevoiti sa
	// trimitem doua mesaje ci doar unul
	int *vot,*vot_null,*vot_aux;
	vot = (int *) calloc(numtasks+1, sizeof(int));
	vot_null = (int *) calloc(numtasks+1, sizeof(int));
	vot_aux = (int *) calloc(numtasks+1, sizeof(int));
	int *rezultat_vot = (int *) calloc(2, sizeof(int));
	rezultat_vot[0] = -1;
	rezultat_vot[1] = -1;

	for(i=0 ; i < numtasks ; i++){
		for(j=0 ; j < numtasks ; j++){
			topologie[i][j]=0;
			topologie_null[i][j]=0;
			topologie_aux[i][j]=0;
		}
	}

	/* Start Citire date din fisier */
	while(!feof(f)){
		int tmp;
		fscanf(f,"%d",&tmp);
		// citesc doar pentru rank-ul meu
		if(tmp == rank){
			// citesc pana la sfarsitul liniei celelalte numere
			char c = '\0';
			// sar peste " - "
			fscanf(f, "%c", &c);
			fscanf(f, "%c", &c);
			fscanf(f, "%c", &c);
			// citesc pana la sf liniei
			while(c!='\n' && !feof(f)) {
				fscanf(f,"%d",&tmp);
				topologie[rank][tmp]=1;
				fscanf(f, "%c", &c);
			}
			break;
		}
		else{
			// sar peste restul liniei
			char c = '\0';
			while(c != '\n') fscanf(f, "%c", &c);
		}
	}
	fclose(f);
	/* End Citire date din fisier */

/**************************End Sectiune Initializari***************************/
/*************************Sectiune stabilire topologie*************************/
/*	Algoritm sondaj - ecou */
	if(rank==0){
		for(i = 1 ; i < numtasks; i++)
			if(topologie[0][i] == 1){
				// Send sondaj
				topologie_aux[numtasks][numtasks] = sondaj;
				MPI_Send(&topologie_aux[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
			 	i, tag, MPI_COMM_WORLD);
				vecini++;
			}
	}else{
		// Primesc mesaj sondaj
		MPI_Recv(&topologie_aux[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
				MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,&Stat);
		parinte = Stat.MPI_SOURCE;

		// Trimit sondaj la ceilalti vecini
		for(i = 0 ; i < numtasks ; i++){
			if(i != parinte && topologie[rank][i]  == 1){
				//Send sondaj
				MPI_Send(&topologie_aux[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
			 	i, tag, MPI_COMM_WORLD);
				vecini++;
			}
		}
	}


	//astept ecou de la toti vecinii
	k = vecini;
	while(k>0){
		// intai check daca e sondaj sau topologie
		MPI_Recv(&topologie_aux[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
				MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,&Stat);
		if(topologie_aux[numtasks][numtasks] == 1){
			// daca e sondaj trimit topologie nula
			topologie_null[numtasks][numtasks]=end;
			MPI_Send(&topologie_null[0][0], (numtasks+1)*(numtasks+1), MPI_INT,
			 	Stat.MPI_SOURCE, tag, MPI_COMM_WORLD);
		}else{
			// Update topologie
			for(i=0;i<numtasks;i++)
				for(j=0;j<numtasks;j++)
					topologie[i][j] |= topologie_aux[i][j];
			k--;
		}
	}

	if(rank!=0){
	   //trimit la parinte topologia calculata
	   topologie[numtasks][numtasks] = end;
	   MPI_Send(&topologie[0][0],(numtasks+1)*(numtasks+1),
	   	MPI_INT,parinte,tag,MPI_COMM_WORLD);
	}

	/* In momentul asta avem calculata topologia in rank 0 */


	// Daca nu sunt rank 0 primesc noua topologie
	// Programul va ramane blocat aici pana cand primeste de la cineva ceva
	if(rank!=0){
		MPI_Recv(&topologie[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
				MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,&Stat);
	}

	// Redistribui topologia mai departe
	for(i = 0 ; i < numtasks ; i++)
			if(i != parinte && topologie[rank][i]  == 1)
				MPI_Send(&topologie[0][0],(numtasks+1)*(numtasks+1),MPI_INT,
					i,tag,MPI_COMM_WORLD);

	//Daca rank 0 afiseaza intreaga matrice de adiacenta in fisier
	if (rank == 0){
		for(i=0 ; i < numtasks ; i++){
			for(j=0 ; j < numtasks ; j++)
				fprintf(g, "%d ",topologie[i][j] );
			fprintf(g, "\n");
		}
		fprintf(g, "\n");
	}

/* Urmeaza algoritm Floyd-Warshall */

	// Umplu matricea d de disntante
	for(i=0;i<numtasks;i++)
		for(j=0;j<numtasks;j++){
			d[i][j] = topologie[i][j];
			if(!topologie[i][j]&&i!=j) d[i][j]=MAXVAL;
		}

	// Aplic Floyd Warshall
	for(k = 0; k< numtasks; k++)
		for(i = 0; i< numtasks; i++)
			for(j = 0; j< numtasks; j++)
				if(d[i][j]>d[i][k]+d[k][j])
					d[i][j] = d[i][k]+d[k][j];

	// Generez vector next_hop
	for(i=0 ; i < numtasks ; i++)
		for(j=0 ; j < numtasks ; j++){
			if(i==j) next_hop[i] = 0;
	      	if (d[rank][i] == d[j][i]+1 && topologie[rank][j]) {
		       next_hop[i] = j;
		       break;
		    }
		}


	// Printam in fisier datele despre topologia proprie de routare
	for(i=0 ; i < numtasks ; i++)
		fprintf(g, "Catre %d -> %d \n",i,next_hop[i] );
/************************End sectiune stabilire topologie**********************/
/************************Start sectiune transmitere mesaje*********************/
	/*
		Codificare mesaje: (stocate in payload[0])
		- -1 Broadcast
		- 0 END message
		- 1 Mesaj normal
	*/
	int nr_msg;
	char c;
	int sursa,dest;
	char msg[255],buffer[255];

	f = fopen(argv[2],"r");
	tag++;
	fscanf(f,"%d",&nr_msg);
	fgets(buffer,255,f);

	sprintf(msg,"Incepe etapa 2\n");
	if(rank == 0)
		printf("Incepe etapa 2\n");

	tag++;

	while(nr_msg>0){
		fscanf(f,"%d",&sursa);		// citim sursa
		fscanf(f,"%c",&c);			// sarim peste spatiu
		fscanf(f,"%s",buffer);		// citim destinatia
		fgets(msg,255,f);			// citim mesajul
		msg[strlen(msg)-1] = '\0'; 	// inlocuim \n

		if(buffer[0]=='B')			// Daca este B atunci broadcast
			dest = -1;	// adica Broadcast
		else						// Altfel update dest
			sscanf(buffer,"%d",&dest);
		int *payload = (int *) calloc(3, sizeof(int));

		if(sursa == rank) // daca eu sunt sursa
		{
			if(dest==-1){
				payload[0] = -1;
				payload[1] = sursa;
				payload[2] = -1;
				// send broadcast la toti vecinii
				printf("Nod[%d] Initiez Broadcast cu mesaj %s\n",rank,msg );
				for(i = 0 ; i < numtasks ; i++)
					if(topologie[rank][i]  == 1){
						MPI_Send(payload,3,MPI_INT,i,tag,MPI_COMM_WORLD);
						// trimit si textul in sine
						MPI_Send(msg,255,MPI_CHAR,i,tag,MPI_COMM_WORLD);
					}
				// Am terminat treci la urmatorul mesaj din fisier
				nr_msg--;
				tag++;
				continue;
			}else
			{
				payload[0] = 1;
				payload[1] = sursa;
				payload[2] = dest;
				printf("Nod[%d]Trimit la %d prin %d\n",rank,dest,next_hop[dest]);
				MPI_Send(payload,3,MPI_INT,next_hop[dest],tag,MPI_COMM_WORLD);
				MPI_Send(msg,255,MPI_CHAR,next_hop[dest],tag,MPI_COMM_WORLD);
				// send message to dest
			}
		}

		while(1){
			// Recieve
			MPI_Recv(payload,3,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&Stat);
			if(payload[0]==-1) // daca primesc mesaj de Broadacst retrimit
			{
				// Recv mesaj
				MPI_Recv(msg,255,MPI_CHAR,MPI_ANY_SOURCE,tag,
					MPI_COMM_WORLD,&Stat);
				printf("Nod[%d] Broadcast from %d with message: <<%s>>."\
					"Rebroadcasting...\n",	rank,Stat.MPI_SOURCE,msg);
				// Retrimit broadcast
				// send broadcast la toti vecinii
				for(i = 0 ; i < numtasks ; i++)
					if(topologie[rank][i]  == 1){
						MPI_Send(payload,3,MPI_INT,i,tag,MPI_COMM_WORLD);
						// trimit si textul in sine
						MPI_Send(msg,255,MPI_CHAR,i,tag,MPI_COMM_WORLD);
					}
				break;
			}
			if(payload[0]==1) // daca primesc mesaj de retransmis
			{
				//Recv mesaj
				MPI_Recv(msg,255,MPI_CHAR,MPI_ANY_SOURCE,tag,
					MPI_COMM_WORLD,&Stat);


				if(payload[2]==rank){ // Daca eu sunt destinatie finala
					// Send END la toti vecinii
					payload[0] = 0;
					printf("Nod[%d] am primit de la %d mesaj <<%s>> ending\n",
						rank,Stat.MPI_SOURCE,msg );
					for(i = 0 ; i < numtasks ; i++)
						if(topologie[rank][i]  == 1)
							MPI_Send(payload,3,MPI_INT,i,tag,MPI_COMM_WORLD);
					break;
				}else{
					printf("Nod[%d] am primit de la %d, retrimit catre %d "\
					"prin %d\n",rank,Stat.MPI_SOURCE,payload[2],next_hop[dest]);
					MPI_Send(payload,3,MPI_INT,next_hop[dest],tag,MPI_COMM_WORLD);
					MPI_Send(msg,255,MPI_CHAR,next_hop[dest],tag,MPI_COMM_WORLD);
					// Trimit mai departe mesaj
				}
			}
			if(payload[0]==0){	// daca primesc mesaj de sfarsit
				// send END mai departe
				for(i = 0 ; i < numtasks ; i++)
					if(topologie[rank][i]  == 1)
						MPI_Send(payload,3,MPI_INT,i,tag,MPI_COMM_WORLD);
				break;
			}

		}// end while(1)


		nr_msg--;
		tag++;
	}

	// Am terminat de trimis toate mesajele, fac broadcast catre toata lumea
	sprintf(msg,"Am terminat etapa 2\n");
	printf("Am terminat etapa 2\n");
	MPI_Barrier(MPI_COMM_WORLD);


/************************End sectiune transmitere emsaje***********************/
/***************************Start sectiune alegere lider***********************/
	// schimbam tag pt etapa 3
	while((rezultat_vot[0] == -1) || (rezultat_vot[1] == -1)){
		tag++;
		for(i=0;i<numtasks;i++) {
			vot[i] = 0; vot_aux[i] = 0;
		}
		// Rank 0 initiaza votul
		// Trimitem mesaj de wakeup la toata lumea
		if(rank==0){
			for(i = 1 ; i < numtasks; i++)
				if(topologie[0][i] == 1){
					// Send wakeup
					vot_aux[numtasks]=wakeup;
					MPI_Send(vot_aux, numtasks+1,MPI_INT,i,tag,MPI_COMM_WORLD);
				}
		}else{
			// Primesc mesaj wakeup
			MPI_Recv(vot_aux, numtasks+1, MPI_INT, MPI_ANY_SOURCE,
						tag, MPI_COMM_WORLD,&Stat);
			parinte = Stat.MPI_SOURCE;
			// Trimit wakeup la ceilalti vecini
			for(i = 0 ; i < numtasks ; i++){
				if(i != parinte && topologie[rank][i]  == 1)
					//Send wakeup
					MPI_Send(vot_aux, numtasks+1,MPI_INT,i,tag,MPI_COMM_WORLD);
			}
		}


		//astept ecou de la toti vecinii
		k = vecini;
		while(k>0){
			// intai check daca e wakeup sau vot
			// ATENTIE
			// in vot_aux[numtasks] sunt datele aditionale (tipul mesajului)
			MPI_Recv(vot_aux, numtasks+1, MPI_INT, MPI_ANY_SOURCE,
						tag, MPI_COMM_WORLD,&Stat);
			if(vot_aux[numtasks] == 1){
				// daca e wakeup trimit vot nul
				vot_null[numtasks]=end;
				MPI_Send(vot_null, numtasks+1, MPI_INT,Stat.MPI_SOURCE,
						tag,MPI_COMM_WORLD);
			}else{
				// Update vector vot
				for(i=0;i<numtasks;i++)
					vot[i] += vot_aux[i];
				k--;
			}
		}
		if(rank!=0){
		   //trimit la parinte votul calculat calculata
			vot[rank] = rand()%numtasks;
	   		vot[numtasks]=end;
			MPI_Send(vot,numtasks+1,MPI_INT,parinte,tag,MPI_COMM_WORLD);
		}

		if(rank == 0)
		{
			vot[rank] = rand()%numtasks;
			for(i=0;i<numtasks;i++)
				printf("Nodul %d a votat %d\n",i,vot[i]);
			// Verific daca votul e valid
			int *aparitii =  (int *) calloc(numtasks, sizeof(int));
			for(i=0;i<numtasks;i++)
				aparitii[vot[i]]++;
			int max=0,candidat=-1,egali=0;
			// Vad cate voturi sunt maxime
			for(i=0;i<numtasks;i++)
				if(aparitii[i]>max)
					max = aparitii[i];
			// Verific daca sunt mai multi cu acc vot

			for(i=0;i<numtasks;i++){
				if(aparitii[i]==max){
					egali++;
					candidat=i;
				}
			}
			if(egali==1){
				//Am gasit un nou lider
				if(rezultat_vot[0]>=0){
					if(candidat!=rezultat_vot[0]){
						rezultat_vot[1] = candidat;
						printf("Nod[0] noul ADJUNCT este %d\n",candidat );
					}
				}
				else{
					rezultat_vot[0] = candidat;
					printf("Nod[0] noul LIDER este %d\n",candidat );
				}
			}else{
				printf("Votul a esuat, se reincearca\n");
			}

		}
		tag++;
		// Daca nu sunt rank 0 primesc rezultate vot
		// Programul va ramane blocat aici pana cand primeste de la cineva ceva
		if(rank!=0){
			MPI_Recv(rezultat_vot,2,MPI_INT,MPI_ANY_SOURCE,tag,
				MPI_COMM_WORLD,&Stat);
		}

		// Redistribui rezutlatele votului mai departe
		for(i = 0 ; i < numtasks ; i++)
			if(i != parinte && topologie[rank][i]  == 1)
				MPI_Send(rezultat_vot,2,MPI_INT,i,tag,MPI_COMM_WORLD);
	} // END while
	printf("Nod[%d] Rezultat final - Lider  %d  Adjunct %d \n",
		rank,rezultat_vot[0],rezultat_vot[1] );
/****************************End sectiune alegere lider************************/
	MPI_Finalize();
	return 0;
}