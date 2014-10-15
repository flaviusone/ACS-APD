void print_consola(unsigned short** mat, unsigned int W, unsigned int H){
	int i,j;
	for(i = 0 ; i < H ; i++){
		for(j = 0 ; j < W ; j++){
			printf("%hu",mat[i][j]);
		}
		printf("\n");
	}
}

void print_fisier(FILE* f, unsigned short** mat, unsigned int W, unsigned int H){
	int i,j;
	for(i = 0 ; i < H ; i++){
		for(j = 0 ; j < W ; j++){
			fprintf(f,"%hu",mat[i][j]);
		}
		fprintf(f,"\n");
	}
}