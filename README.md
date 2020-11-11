# ParallelBitonic
A collection of serial and parallel implementations of the Bitonic sort algorithm in C. The project is based on the recursive and the imperative implementations of the Bitonic sort algorithm referring to the resources [[1]](http://www.cag.lcs.mit.edu/streamit/results/bitonic/code/c/bitonic.c), [[2]](http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm) and [[3]](http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm ).

## Serial Implementations
The "serial" folder contains two serial implementations of the Bitonic sort, based on the work of Nikos Pitsianis, Duke CS. The bitonic.c contains functions for sorting the elements of an array in ascending or descending order, for printing out the elements of the array and for testing the result. There is an imperative as long as a recursive implementation of the Bitonic sort available.

The user is able to define the problem size, using  a command line argument N, where size = 2 <sup>N</sup>.

#### Compile
- To compile the source code open a terminal in the same folder with "bitonic.c" and run:
```$ gcc bitonic.c -o executable-file-name```
	- executable-file-name = the name of the final executable

#### Run
- To run the bitonic sort for a random array of the desired size open a terminal in the same folder with the executable and run:
```./ executable-file-name N```
	- executable-file-name = the name of the final executable
	- N = the superscript of 2 for the array size as: size = 2 <sup>N</sup>


## Parallel Implementations
The "parallel" folder contains three parallel implementations of the Bitonic sort, based on the serial one mentioned above. The "CilkPlus" folder contains a parallel implementation of the sort using [Cilk Plus](https://www.cilkplus.org/), the "Omp" folder contains a parallel implementation of the sort using [OpenMP](https://www.openmp.org/) and the "Pthreads" folder contains a parallel implementation of the sort using [POSIX Threads](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html).

### CilkPlus
The user is able to define the problem size, using  a command line argument N, where size = 2 <sup>N</sup>.

#### Compile
- To compile the source code open a terminal in the same folder with "bitonic.c" and run:
```$ gcc bitonic.c -o executable-file-name```
	- executable-file-name = the name of the final executable

#### Run
- To run the bitonic sort for a random array of the desired size open a terminal in the same folder with the executable and run:
```./ executable-file-name N```
	- executable-file-name = the name of the final executable
	- N = the superscript of 2 for the array size as: size = 2 <sup>N</sup>

### OpenMP
The user is able to define the problem size, using  a command line argument N, where size = 2 <sup>N</sup>.

#### Compile
- To compile the source code open a terminal in the same folder with "bitonic.c" and run:
```$ gcc bitonic.c -o executable-file-name```
	- executable-file-name = the name of the final executable

#### Run
- To run the bitonic sort for a random array of the desired size open a terminal in the same folder with the executable and run:
```./ executable-file-name N```
	- executable-file-name = the name of the final executable
	- N = the superscript of 2 for the array size as: size = 2 <sup>N</sup>

### POSIX Threads
The user is able to define the problem size, using  a command line argument N, where size = 2 <sup>N</sup>.

#### Compile
- To compile the source code open a terminal in the same folder with "bitonic.c" and run:
```$ gcc bitonic.c -o executable-file-name```
	- executable-file-name = the name of the final executable

#### Run
- To run the bitonic sort for a random array of the desired size open a terminal in the same folder with the executable and run:
```./ executable-file-name N```
	- executable-file-name = the name of the final executable
	- N = the superscript of 2 for the array size as: size = 2 <sup>N</sup>
