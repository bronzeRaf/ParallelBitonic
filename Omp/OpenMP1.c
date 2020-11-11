//Mprouzos Rafail
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const int ASCENDING  = 1;
const int DESCENDING = 0;

//glabal metablites
struct timeval startwtime, endwtime;
double seq_time;

int q,p;

int *a,N,NUM_THREADS;

//arxiki dilwsi sunartisewn gia na einai orates
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
  //dilswseis metavlitwn
  int i;
  
  if (argc != 3) {
    printf("Usage: %s q\n  where n=2^q is problem size (power of two)\n", 
	   argv[0]);
    exit(1);
  }
  
  //N=2^q
  q=atoi(argv[1]);
  N = 1<<atoi(argv[1]);
  //NUM_THREADS=2^p
  p=atoi(argv[2]);
  NUM_THREADS = 1<<atoi(argv[2]);
  //dimiourgia pinaka
  a = (int *) malloc(N * sizeof(int));
  
  printf("q---->%d   p---->%d \n________________\n",q,p);
    
  //ektelesi seiriakou algori8mou
  init();
  gettimeofday (&startwtime, NULL);
  sort();
  gettimeofday (&endwtime, NULL);

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("Recursive wall clock time = %f\n", seq_time);

  test();

  //ektelesi parallilou algorithmou
  init();
  gettimeofday (&startwtime, NULL);
  ParSort();
  gettimeofday (&endwtime, NULL);

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("OpenMP wall clock time = %f\n", seq_time);

  test();

  //quick sort ektelesi
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

//domi me stoixeia pou 8a perasoun san 1 orisma stis function pointer pou ektelountai apo threads
typedef struct{
  //lo,cnt,dir tis bitonic, kai vathos tou thread sto dentro (gia ka8e anadromi pame pio vathia) deep.
  int lo, cnt, dir, deep;
} targ;

void ParSort(){
  //arxikopoiw orisma gia (master) thread function
  targ arg;
  arg.lo = 0;
  arg.cnt = N;
  arg.dir = ASCENDING;
  arg.deep = 0;
  ParBitonicSort(&arg);
}

//Parallel bitonic sort with Pthreads
void *ParBitonicSort(void *arg){
  int k;
  //eksagw orisma
  int lo = ( ( targ * ) arg ) -> lo;
  int cnt = ( ( targ * ) arg ) -> cnt;
  int dir = ( ( targ * ) arg ) -> dir;
  int deep = ( ( targ * ) arg ) -> deep;
  if ( cnt > 1 ) {
    k = cnt / 2;
    if(deep>=p){
    //seiriaki sunexeia:megistos ari8mos thread
      qsort( a + lo, k, sizeof( int ), asc );
      qsort( a + ( lo + k ) , k, sizeof( int ), desc );
	}
    else{
      //ftiaxnw orisma gia thread function (deksi paidi sto dentro)
      targ argr;
      argr.lo = lo;
      argr.cnt = k;
      argr.dir = ASCENDING;
      //to thread pleon ena epipedo pio va8ia (to exei kalesei apo deep=1 kai meta thread)
      argr.deep = deep + 1;
      //ftiaxnw orisma gia to trexon thread (aristero paidi sto dentro)
      targ argl;
      argl.lo = lo + k;
      argl.cnt = k;
      argl.dir = DESCENDING;
      argl.deep = deep + 1;
      
      #pragma omp parallel
      #pragma omp single nowait
	  {
	  //prwti anadromi  
	    #pragma omp task
	      {ParBitonicSort(&argr);}
	  //2i anadromi
		#pragma omp task
		  {ParBitonicSort(&argl);}
	    
        #pragma omp taskwait
      }
    }
    //ftiaxnw orisma gia thread function (sti merge) 
    targ argm;
    argm.lo = lo;
    argm.cnt = cnt;
    argm.dir = dir;
    //ka8e anadromi kai ena thread, 1) mpainei pio vathia kai 2) ftiaxnei ena akoma thread, ara threads epi 2 ka8e fora, ara epitrepto va8os p (2^p threads)
    //ka8e fora kalw merge me ta upoloipa dia8esima threads pou dn xreaizete i sort apo ta sunolika
	argm.deep = p - deep;
    ParBitonicMerge( &argm );
  }
  return 0;
}

//Parallel bitonic merge with Pthreads
void *ParBitonicMerge(void *arg){
  int k;
  //eksagw orisma
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
   	//seiriaki sunexeia:megistos ari8mos thread
      bitonicMerge( lo, k, dir );
      bitonicMerge( lo + k, k, dir );
      return 0;
    }
    //ftiaxnw orisma gia thread function (deksi paidi sto dentro)
    targ argmr;
    argmr.lo = lo;
    argmr.cnt = k;
    argmr.dir = dir;
	//epitrepto va8os pou apomenei meiwnete kata 1
    argmr.deep = deep - 1;
    
    //ftiaxnw orisma gia to trexon thread (aristero paidi sto dentro)
    targ  argml;
    argml.lo = lo + k;
    argml.cnt = k;
    argml.dir = dir;
    argml.deep = deep - 1;

    //tin mia anadromi tn kanei neo thread enw parallila tin alli to trexon thread
    #pragma omp parallel
    #pragma omp single nowait
	{
	  //1i anadromi
	  #pragma omp task
	    {ParBitonicMerge(&argmr);}
	  //2i anadromi
	  #pragma omp task
	    {ParBitonicMerge(&argml);}
	  #pragma omp taskwait
    }    
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

//compare functions for qsort
//source: https://github.com/Theodosis/parallel-programming/blob/master/pthreads/bitonic/pbitonic.c
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
