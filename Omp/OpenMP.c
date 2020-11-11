//Mprouzos Rafail
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

struct timeval startwtime, endwtime;
double seq_time;

int *a;
int N;
int NUM_THREADS;

//arxiki dilwsi sunartisewn
void print(void);
void test(void);
inline void exchange(int i, int j);
void PimpBitonicSort(void);
void init(void);
void impBitonicSort(void);
int desc( const void *a, const void *b );
int asc( const void *a, const void *b );

int main(int argc, char **argv){
  //dilswseis metavlitwn
  int i;
  
  if (argc != 3) {
    printf("Usage: %s q\n  where n=2^q is problem size (power of two)\n", 
	   argv[0]);
    exit(1);
  }
  
  //N=2^q
  N = 1<<atoi(argv[1]);
  //NUM_THREADS=2^p
  NUM_THREADS = 1<<atoi(argv[2]);
  //dimiourgia pinaka
  a = (int *) malloc(N * sizeof(int));

  //ektelesi parallilou algorithmou
  init();
  gettimeofday (&startwtime, NULL);
  PimpBitonicSort();
  gettimeofday (&endwtime, NULL);
  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);
  printf("Parallel (openmp) Imperative wall clock time = %f\n", seq_time);
  test();
  
  //ektelesi seiriakou algori8mou
  init();
  gettimeofday (&startwtime, NULL);
  impBitonicSort();
  gettimeofday (&endwtime, NULL);
  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);
  printf("Imperative wall clock time = %f\n", seq_time);
  test();
  
  init();
  gettimeofday (&startwtime, NULL);
  qsort(a,N,sizeof(int),asc);
  gettimeofday (&endwtime, NULL);
  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);
  printf("Quick sort wall clock time = %f\n", seq_time);
  test();
  
  free(a);
}


//source: https://github.com/Theodosis/parallel-programming/blob/master/pthreads/bitonic/pbitonic.c
//parallel imperative version of bitonic sort with OMP
void PimpBitonicSort() {
    omp_set_num_threads( NUM_THREADS );
    int i,j,k=0;
    for (k = 2; k <= N; k *= 2 ) {
        for (j=k>>1; j>0; j=j>>1) {
        	   	#pragma omp parallel for
                for (i=0; i<N; i++) {
                    int ij=i^j;
                    if ((ij)>i) {
                        if ((i&k)==0 && a[i] > a[ij]) {
                            exchange(i,ij);
                        }
                        if ((i&k)!=0 && a[i] < a[ij]){
                            exchange(i,ij);
                        }
                    }
                }
        }
    }
}

/*
  imperative version of bitonic sort
*/
void impBitonicSort() {

  int i,j,k;
  
  for (k=2; k<=N; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
      for (i=0; i<N; i++) {
	    int ij=i^j;
	    if ((ij)>i) {
	      if ((i&k)==0 && a[i] > a[ij]) 
	        exchange(i,ij);
	      if ((i&k)!=0 && a[i] < a[ij])
	        exchange(i,ij);
	   }
      }
    }
  }
}

/** -------------- SUB-PROCEDURES  ----------------- **/
/** procedure init() : initialize array "a" with data **/
void init() {
  int i;
  for (i = 0; i < N; i++) {
    a[i] = rand() % N; // (N - i);
  }
}

/** INLINE procedure exchange() : pair swap **/
inline void exchange(int i, int j) {
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}

/** procedure  print() : print array elements **/
void print() {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d\n", a[i]);
  }
  printf("\n");
}

/** procedure test() : verify sort results **/
void test() {
  int pass = 1;
  int i;
  for (i = 1; i < N; i++) {
    pass &= (a[i-1] <= a[i]);
  }

  printf(" TEST %s\n",(pass) ? "PASSed" : "FAILed");
}

//compare functions for qsort
int desc( const void *a, const void *b ){
  int* arg1 = (int *)a;
  int* arg2 = (int *)b;
  if( *arg1 > *arg2 ) return -1;
  else if( *arg1 == *arg2 ) return 0;
  return 1;
}

int asc( const void *a, const void *b ){
  int* arg1 = (int *)a;
  int* arg2 = (int *)b;
  if( *arg1 < *arg2 ) return -1;
  else if( *arg1 == *arg2 ) return 0;
  return 1;
}
