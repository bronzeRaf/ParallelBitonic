/*
 Pthreads1.c 

 This file contains a parallel implementation of the recursive bitonic sort,
 using POSIX Threds to spawn parallel threads for recursive calls.
        
 */

/* 
------- ---------------------- 
   Brouzos Rafael 	rnm1816@gmail.com	www.github.com/bronzeRaf
-----------------------------
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const int ASCENDING  = 1;
const int DESCENDING = 0;

//global variables
struct timeval startwtime, endwtime;
double seq_time;

int q,p;

int *a,N,NUM_THREADS;

//declare functions
void init(void);
void print(void);
void test(void);
inline void exchange(int i, int j);
void compare(int i, int j, int dir);
void sort(void);
void bitonicMerge(int lo, int cnt, int dir);
void recBitonicSort(int lo, int cnt, int dir);
void ParSort(void);
void *ParBitonicSort(void *arg);
void *ParBitonicMerge(void *arg);
int desc( const void *a, const void *b );
int asc( const void *a, const void *b );


int main(int argc, char **argv){
  int i; //index
  
  //check arguments
  if (argc != 3) {
    printf("Usage: %s q\n  where n=2^q is problem size (power of two)\n",argv[0]);
    exit(1);
  }
  
  //N=2^q
  q=atoi(argv[1]);
  N = 1<<atoi(argv[1]);
  //NUM_THREADS=2^p
  p=atoi(argv[2]);
  NUM_THREADS = 1<<atoi(argv[2]);
  //memory allocation
  a = (int *) malloc(N * sizeof(int));
  
  printf("q---->%d   p---->%d \n________________\n",q,p);
    
  //fire up the serial algorithm
  init();
  gettimeofday (&startwtime, NULL);
  sort();
  gettimeofday (&endwtime, NULL);

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("Recursive wall clock time = %f\n", seq_time);

  test();

  //fire up the parallel algorithm
  init();
  gettimeofday (&startwtime, NULL);
  ParSort();
  gettimeofday (&endwtime, NULL);

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("Pthread wall clock time = %f\n", seq_time);

  test();

  //fire up the quick sort 
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

//arguments for the function pointers in the threads
typedef struct{
  //lo,cnt,dir from bitonic
  //for every recursive call we add 1 in the deep variable
  int lo, cnt, dir, deep;
} targ;

/**initializes the master thread's arguments**/
void ParSort(){
  targ arg;
  arg.lo = 0;
  arg.cnt = N;
  arg.dir = ASCENDING;
  arg.deep = 0;
  ParBitonicSort(&arg);
}

/**
Parallel bitonic sort with Pthreads
**/
void *ParBitonicSort(void *arg){
  int k;
  //obtain arguments
  int lo = ( ( targ * ) arg ) -> lo;
  int cnt = ( ( targ * ) arg ) -> cnt;
  int dir = ( ( targ * ) arg ) -> dir;
  int deep = ( ( targ * ) arg ) -> deep;
  if ( cnt > 1 ) {
    k = cnt / 2;
    if(deep>=p){
    //reached the maximun number of threads! We continue in serial
      qsort( a + lo, k, sizeof( int ), asc );
      qsort( a + ( lo + k ) , k, sizeof( int ), desc );
	}
    else{
      //arguments for new thread
      targ argr;
      pthread_t threadr;
      argr.lo = lo;
      argr.cnt = k;
      argr.dir = ASCENDING;
      //add 1 to go deeper
      argr.deep = deep + 1;
      //arguments for current thread
      targ argl;
      argl.lo = lo + k;
      argl.cnt = k;
      argl.dir = DESCENDING;
      argl.deep = deep + 1;
      
      //the new call is assigned in the new thread
      pthread_create( &threadr, NULL, ParBitonicSort, &argr );
      ParBitonicSort(&argl);
      
      pthread_join( threadr, NULL );
    }
    //argument for thread function
    targ argm;
    argm.lo = lo;
    argm.cnt = cnt;
    argm.dir = dir;
    //every recursive call is gives a new thread, one level deeper
	argm.deep = p - deep;
    //merge
    ParBitonicMerge( &argm );
  }
  return 0;
}

/**
Parallel bitonic merge with Pthreads
**/
void *ParBitonicMerge(void *arg){
  int k;
  //obtain arguments
  int lo = ( ( targ * ) arg ) -> lo;
  int cnt = ( ( targ * ) arg ) -> cnt;
  int dir = ( ( targ * ) arg ) -> dir;
  int deep = ( ( targ * ) arg ) -> deep;
  if( cnt > 1 ){
    k = cnt / 2;
    int i;
    for( i = lo; i < lo + k; ++i ){
      compare( i, i + k, dir );
    }
    if( deep <= 0 ){
   	//reached the maximun number of threads! We continue in serial
      bitonicMerge( lo, k, dir );
      bitonicMerge( lo + k, k, dir );
      return 0;
    }
    //arguments for new thread
    targ argmr;
    pthread_t threadmr;
    argmr.lo = lo;
    argmr.cnt = k;
    argmr.dir = dir;
	//add 1 to go deeper
    argmr.deep = deep - 1;
    
    //arguments for current thread
    targ  argml;
    argml.lo = lo + k;
    argml.cnt = k;
    argml.dir = dir;
    argml.deep = deep - 1;

    //every recursive call is gives a new thread, one level deeper
    pthread_create( &threadmr, NULL, ParBitonicMerge, &argmr );
    ParBitonicMerge(&argml);
    
    pthread_join( threadmr, NULL );
  }
  return 0;
}



/** function sort() 
   Caller of recBitonicSort for sorting the entire array of length N 
   in ASCENDING order
 **/
void sort() {
  recBitonicSort(0, N, ASCENDING);
}

/** Procedure bitonicMerge() 
   It recursively sorts a bitonic sequence in ascending order, 
   if dir = ASCENDING, and in descending order otherwise. 
   The sequence to be sorted starts at index position lo,
   the parameter cbt is the number of elements to be sorted. 
 **/
void bitonicMerge(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    int i;
    for (i=lo; i<lo+k; i++)
      compare(i, i+k, dir);
    bitonicMerge(lo, k, dir);
    bitonicMerge(lo+k, k, dir);
  }
}



/** function recBitonicSort() 
    first produces a bitonic sequence by recursively sorting 
    its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order 
 **/
void recBitonicSort(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    recBitonicSort(lo, k, ASCENDING);
    recBitonicSort(lo+k, k, DESCENDING);
    bitonicMerge(lo, cnt, dir);
  }
}

/** -------------- SUB-PROCEDURES  ----------------- **/ 

/** procedure test() : verify sort results **/
void test() {
  int pass = 1;
  int i;
  for (i = 1; i < N; i++) {
    pass &= (a[i-1] <= a[i]);
  }

  printf(" TEST %s\n",(pass) ? "PASSed" : "FAILed");
}


/** procedure init() : initialize array "a" with data **/
void init() {
  int i;
  for (i = 0; i < N; i++) {
    a[i] = rand() % N; // (N - i);
  }
}

/** procedure  print() : print array elements **/
void print() {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d\n", a[i]);
  }
  printf("\n");
}


/** INLINE procedure exchange() : pair swap **/
inline void exchange(int i, int j) {
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}



/** procedure compare() 
   The parameter dir indicates the sorting direction, ASCENDING 
   or DESCENDING; if (a[i] > a[j]) agrees with the direction, 
   then a[i] and a[j] are interchanged.
**/
inline void compare(int i, int j, int dir) {
  if (dir==(a[i]>a[j])) 
    exchange(i,j);
}

/**
descending compare function for qsort
source: https://github.com/Theodosis/parallel-programming/blob/master/pthreads/bitonic/pbitonic.c
**/
int desc( const void *a, const void *b ){
  int* arg1 = (int *)a;
  int* arg2 = (int *)b;
  if( *arg1 > *arg2 ) return -1;
  else if( *arg1 == *arg2 ) return 0;
  return 1;
}

/**
ascending compare function for qsort
source: https://github.com/Theodosis/parallel-programming/blob/master/pthreads/bitonic/pbitonic.c
**/
int asc( const void *a, const void *b ){
  int* arg1 = (int *)a;
  int* arg2 = (int *)b;
  if( *arg1 < *arg2 ) return -1;
  else if( *arg1 == *arg2 ) return 0;
  return 1;
}
