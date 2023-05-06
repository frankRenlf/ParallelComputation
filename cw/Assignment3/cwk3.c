//
// Starting point for the OpenCL coursework for XJCO3221 Parallel Computation.
//
// On cloud-hpc1.leeds.ac.uk you should compile with:
// nvcc -lOpenCL -o cwk3 cwk3.c
//
// Once compiled, execute with the size of the vector, N, which is also the number of both
// rows and columns of the (square) matrix. N should be a power of 2, otherwise an error
// will be returned. Example usage is provided in the batch script "cwk3-batch.sh" which
// should be executed on the batch queue
//
// sbatch cwk3-batch.sh
//
// This assumes that the executable is called "cwk3" and will display a randomly-generated
// 8x8 matrix, a randonly-generated 8-vector, and the output 8-vector which currently is
// incorrect. You need to implement OpenCL code to perform the operation correctly.
//

//
// Includes.
//
#include <stdio.h>
#include <stdlib.h>

// For this coursework, the helper file has 4 routines in addition to simpleOpenContext_GPU() and compileKernelFromFile():
// - getCmdLineArgs()         : Gets the command line argument and checks they are valid.
// - displayMatrixAndVector() : Displays the matrix and the vector, or just the top-left corner if it is too large.
// - fillMatrixAndVector()    : Fills the matrix with random values.
// - displaySolution()        : Displays the solution vector, i.e. the matrix multiplied by the initial vector.
// Do not alter these routines, as they will be replaced with different versions for assessment.
#include "helper_cwk.h"
#define BLOCK_SIZE 16
//
// Main.
//
int main(int argc, char **argv)
{

    //
    // Parse command line arguments and check they are valid. Handled by a routine in the helper file.
    //
    int N;
    getCmdLineArgs(argc, argv, &N);

    //
    // Initialisation.
    //

    // Set up OpenCL using the routines provided in helper_cwk.h.
    cl_device_id device;
    cl_context context = simpleOpenContext_GPU(&device);

    // Open up a single command queue, with the profiling option off (third argument = 0).
    cl_int status;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &status);

    // Allocate memory for the vector and the matrix.
    float *hostMatrix = (float *)malloc(N * N * sizeof(float));
    float *hostVector = (float *)malloc(N * sizeof(float));
    float *hostSolution = (float *)malloc(N * sizeof(float));

    // Fill the matrix and vector with random values, and display.
    fillMatrixAndVector(hostMatrix, hostVector, N);
    displayMatrixAndVector(hostMatrix, hostVector, N); // DO NOT ALTER: Your solution MUST call this function at the start of your calculation.

    // Initialise the solution vector to zero.
    int i;
    for (i = 0; i < N; i++)
        hostSolution[i] = 0.0f;

    //
    // Perform the calculation on the GPU.
    //

    // Your solution should mainly go here and in the associated .cl file,
    // but you are free to make changes elsewhere in this code.

    // Create buffers for the matrix, vector, and solution on the device.
    cl_mem matrix = clCreateBuffer(context, CL_MEM_READ_ONLY, N * N * sizeof(float), hostMatrix, &status);
    cl_mem vector = clCreateBuffer(context, CL_MEM_READ_ONLY, N * sizeof(float), hostVector, &status);
    cl_mem solution = clCreateBuffer(context, CL_MEM_WRITE_ONLY, N * sizeof(float), NULL, &status);

    // Write the matrix and vector to the device.
    // status = clEnqueueWriteBuffer(queue, matrix, CL_TRUE, 0, N * N * sizeof(float), hostMatrix, 0, NULL, NULL);
    // status = clEnqueueWriteBuffer(queue, vector, CL_TRUE, 0, N * sizeof(float), hostVector, 0, NULL, NULL);

    // Compile the kernel.
    cl_kernel kernel = compileKernelFromFile("cwk3.cl", "matrixVectorMul", context, device);

    // Set the kernel arguments.
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &matrix);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &vector);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &solution);
    // status = clSetKernelArg(kernel, 3, sizeof(int), &N);
    if (status != CL_SUCCESS)
    {
        printf("Error setting kernel arguments: %d\n", status);
        exit(-1);
    }

    // Define the global and local work sizes.
    size_t globalWorkSize[1] = {N};
    size_t localWorkSize[1] = {256}; // 256 is the maximum local work size for some GPUs.

    // Execute the kernel.
    status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    // if (status != CL_SUCCESS)
    // {
    //     printf("Error enqueueing kernel: %d\n", status);
    //     exit(-1);
    // }

    // Wait for the kernel to finish executing.
    clFinish(queue);

    // Read the solution vector back from the device.
    status = clEnqueueReadBuffer(queue, solution, CL_TRUE, 0, N * sizeof(float), hostSolution, 0, NULL, NULL);
    if (status != CL_SUCCESS)
    {
        printf("Error reading solution: %d\n", status);
        exit(-1);
    }

    //
    // Display the final result.
    //
    displaySolution(hostSolution, N); // DO NOT ALTER: Your solution MUST call this function at the end of your calculation.

    //
    // Release all resources.
    //
    clReleaseMemObject(matrix);
    clReleaseMemObject(vector);
    clReleaseMemObject(solution);

    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(hostMatrix);
    free(hostVector);
    free(hostSolution);

    return EXIT_SUCCESS;
}
