#include "helper.h" // Also includes OpenCL .
int main()
{
    // Get context and device for a GPU.
    cl_device_id device;
    cl_context context = simpleOpenContext_GPU(&device);

    // Open a command queue to it.
    cl_int status;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &status);

    // Use the GPU through ’queue ’.
    // At end of program .
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}