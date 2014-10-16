#include <omp.h>
#define CHUNKSIZE 100
#define N     1000

main ()  
{

int i, chunk,tid;
float a[N], b[N], c[N];

/* Some initializations */
for (i=0; i < N; i++)
  a[i] = b[i] = i * 1.0;
chunk = CHUNKSIZE;

#pragma omp parallel shared(a,b,c,chunk) private(i,tid)
  {

  #pragma omp for schedule(dynamic,chunk) nowait
  for (i=0; i < N; i++){
  	tid = omp_get_thread_num();
    c[i] = a[i] + b[i];
    printf("Id thread = %d, i = %d \n", tid, i);
  }

  }  /* end of parallel section */

}