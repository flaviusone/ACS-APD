void print_consola(unsigned short** mat, char mode,
		 	unsigned int W_h, unsigned int H_h, unsigned int W, unsigned int H){
	int i,j;
	printf("%c %u %u %u %u\n", mode, W_h, H_h, W, H);
	for(i = 0 ; i < H+2 ; i++){
		for(j = 0 ; j < W+2 ; j++){
			printf("%hu ",mat[i][j]);
		}
		printf("\n");
	}
}

void print_fisier(FILE* f, unsigned short** mat, char mode,
		 	unsigned int W_h, unsigned int H_h, unsigned int W, unsigned int H){
	int i,j;
	fprintf(f,"%c %u %u %u %u\n", mode, W_h, H_h, W, H);
	for(i = 1 ; i < H_h+1 ; i++){
		for(j = 1 ; j < W_h+1 ; j++){
			fprintf(f,"%hu ",mat[i][j]);
		}
		fprintf(f,"\n");
	}
}

// alocare in zona continua de memorie
unsigned short **allocMat(int w, int h)
{
    unsigned short **mat;
    unsigned short *buf;
    buf = (unsigned short *) calloc(h * w, sizeof(unsigned short));
    int i;
    mat = (unsigned short **) calloc(h, sizeof(unsigned short *));
    #pragma omp parallel for private(i) shared(buf, w, h) schedule(static)
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}