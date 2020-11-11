
# ParallelBitonic
A collection of serial and parallel implementations of the Bitonic sort algorithm in C. The project is based on the recursive and the imperative implementations of the Bitonic sort algorithm referring to the resources [[1]](http://www.cag.lcs.mit.edu/streamit/results/bitonic/code/c/bitonic.c), [[2]](http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm) and [[3]](http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm ). 

Bitonic sort is based on the Bitonic sequence. A sequence is called Bitonic if it is first increasing, then decreasing. In other words, an array arr[0..n-i] is Bitonic if there exists an index i where 0<=i<=n-1 such that:

```x(0) <= x(1) ... <= x(i) and  x(i) >= x(i+1) ... >= x(n-1)```

A sequence, sorted in increasing order is considered Bitonic, with the decreasing part as empty. Similarly, a decreasing order sequence is considered Bitonic too, with the increasing part as empty.

Bitonic sort works with arrays that their size is a power of 2 (size = 2<sup>N</sup>). First of all we split an array in 2 segments, ordering the first half ascending and the second half descending, creating a Bitonic sequence. Afterwards, we combine the segments comparing first element of first half with first element of second half, then second element of first half with second element of second and so on. We exchange elements if an element of the second half is smaller. After the above step, we got all the elements in the first half smaller than all the elements in the second half. The compare and swap results into the two sequences of size/2 length each. We repeat the process performed in the second step recursively onto every sequence until we get a single sorted sequence of length size.

This sort has a O(log<sup>2</sup>size) complexity. Obviously, both the iterative and the recursive versions of the sort could be implemented through parallel programming. The parallel iterative version assumes that some iterations of the loops could be done in parallel. The parallel recursive versions assumes that every recursive call could be executed in a parallel thread.

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
The "parallel" folder contains three parallel implementations of the Bitonic sort, based on the serial one mentioned above. The "CilkPlus" folder contains a parallel implementation of the sort using [Cilk Plus](https://www.cilkplus.org/), the "Omp" folder contains two parallel implementations of the sort using [OpenMP](https://www.openmp.org/) and the "Pthreads" folder contains a parallel implementation of the sort using [POSIX Threads](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html).

### CilkPlus
This is an implementation of the imperative Bitonic sort, using the parallel for loops of CilkPlus.

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
There are two implementations, one of the imperative Bitonic sort, using the parallel for loops of OpenMP in the "OpenMP.c" and another one of the recursive Bitonic sort, using the OpenMP task to spawn parallel threads in the "OpenMP1.c".

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
This is an implementation of the recursive Bitonic sort, using parallel thread spawns of POSIX Threads, for every recursive call of the sort.

#### Compile
- To compile the source code open a terminal in the same folder with "bitonic.c" and run:
```$ gcc bitonic.c -o executable-file-name```
	- executable-file-name = the name of the final executable

#### Run
- To run the bitonic sort for a random array of the desired size open a terminal in the same folder with the executable and run:
```./ executable-file-name N```
	- executable-file-name = the name of the final executable
	- N = the superscript of 2 for the array size as: size = 2 <sup>N</sup>

## Results
In the "results" folder you can find the runtime of each implementation. For a complete comparison the results present the runtimes of any implementation for several maximum threads (p) and for several superscripts of 2 for the array size (q). In any experiment, the implementation is compared with qsort and with the serial implementation runtimes. In every execution the sorts are verified by a test function. All the timing presented in the text files is counted in seconds.
