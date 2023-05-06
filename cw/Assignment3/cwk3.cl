// Kernel for matrix transposition.
__kernel void matrixVectorMul( __global const float *matrix, __global const float *vector, __global float *solution, const int N )
{
    int i = get_global_id(0);
    if( i < N ) {
        float sum = 0.0f;
        for( int j=0; j<N; j++ ) {
            sum += matrix[i*N+j] * vector[j];
        }
        solution[i] = sum;
    }
}