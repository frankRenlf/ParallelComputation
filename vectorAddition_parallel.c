//
// Vector addition in parallel. Compile with: gcc -fopenmp -Wall -o vectorAddition_parallel vectorAddition_parallel.c 
//

// Includes. Don't need <omp.h> for this example.
#include <stdio.h>
#include <stdlib.h>

// n is the size of all three vectors. For simplicity, use statically allocated arrays
// [rather than dynamically allocated using malloc()/free()]
#define n 20

int main()
{
	float a[n], b[n], c[n];
	int i;

	// For this example, assign arbitrary numbers to vectors a and b.
	for( i=0; i<n; i++ ) a[i] = b[i] = i;

	// Vector addition in parallel
	#pragma omp parallel for
	for( i=0; i<n; i++ ) c[i] = a[i] + b[i];

	// Check the answer: For this simple example, c[i] should be 2i.
	for( i=0; i<n; i++ )
		if( c[i] != 2*i )
		{
			printf( "Addition failed.\n" );
			return EXIT_FAILURE;
		}

	printf( "Addition successful.\n" );
	return EXIT_SUCCESS;
}