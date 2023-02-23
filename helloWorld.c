// Simple 'Hello World' program for OpenMP.
// Compile with '-fopenmp', i.e.: gcc -fopenmp <other options as usual>

#include <stdio.h>
#include <omp.h>		// Required for run-time OpenMP library routines

int main()
{
	// Tells the compiler to parallelise the next bit; the scope after this pragma is in parallel
#pragma omp parallel
	{
		// Get the thread number, and the maximum number of threads which depends on the target architecture.
		int threadNum = omp_get_thread_num();
		int maxThreads = omp_get_max_threads();

		// Simple message to stdout
		printf("Hello from thread %i of %i!\n", threadNum, maxThreads);
	}

	return 0;
}