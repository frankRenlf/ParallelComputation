//
// Simple C code for generating the Mandelbrot set, with parallelisation in OpenMP.
// A makefile it provided to aid compilation; just type 'make' and execute with './Mandelbrot'
// Purely meant for demonstration purposes and the OpenGL in particular could be optimised.
// - DAH/28/11/2017.
//
// Updated to use GLFW (rather than GLUT) for both Linux and Macs.
// - DAH/26/11/2019.
//

// Standard includes.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// For OpenGL windows. Should run on Linux (after loading the glfw module), or Macs (once glfw installed via homebrew),
// but may require changes for specific installations of glfw.
#include <GLFW/glfw3.h>

//
// Window and view parameters and variables
//
const int windowSize_x = 600;
const int windowSize_y = 600;

// Number of pixels to calculate; can be less than the window size.
#define numPixels_x 600
#define numPixels_y 600

// The maximum number iterations per pixel. Small values result in faster code but less well defined images.
const int maxIters = 10000;

// The colour arrays.
float red[numPixels_x][numPixels_y];
float green[numPixels_x][numPixels_y];
float blue[numPixels_x][numPixels_y];

//
// Compute-intensive routine that determines the colour of the specified pixel and stores in the global 'red',
// 'green' and 'blue' arrays.
//
void setPixelColour(int i, int j)
{
    // Check the pixel requested is in range.
    if (i < 0 || j < 0 || i >= windowSize_x || j >= windowSize_y)
        return;

    // Initialise the variables (would be complex variables c and z).
    float
        cx = -2.0f + 4.0f * i / numPixels_x,
        cy = -2.0f + 4.0f * j / numPixels_y,
        zx = 0.0f,
        zy = 0.0f,
        ztemp;

    // The main loop.
    int numIters = 0;
    do
    {
        ztemp = zx * zx - zy * zy + cx;
        zy = 2 * zx * zy + cy;
        zx = ztemp;
    } while (++numIters < maxIters && zx * zx + zy * zy < 4.0f);

    // Colour based on the number of iterations. Cycle through RGB at different rates.
    if (numIters < maxIters)
    {
        red[i][j] = 0.1f * (numIters % 11);
        green[i][j] = 0.05f * (numIters % 21);
        blue[i][j] = 0.02f * (numIters % 51);
    }
}

//
// Generates the next line of the image, and stores in the colour in the global 'red', 'green' and 'blue' arrays.
//
void generateImage(void)
{
    int i, j;
    omp_set_num_threads(4);
    // Set the image to black before starting.
    for (j = 0; j < numPixels_y; j++)
        for (i = 0; i < numPixels_x; i++)
            red[i][j] = green[i][j] = blue[i][j] = 0.0f;

    // Time the calculations using clock() from time.h
    printf("Generating the image of %dx%d pixels, with maxIters=%d ...\n", numPixels_x, numPixels_y, maxIters);
    double startTime = omp_get_wtime(); // Get the "wall clock" time, i.e. the time that a clock on the wall would measure.
// This is the same starting point (i.e. the first attempt to parallelise) as in the lecture notes, except the extra scope.
#pragma omp parallel for private(i)
    for (j = 0; j < numPixels_y; j++)
    {
        for (i = 0; i < numPixels_x; i++)
        {
            // Set the colour of pixel (i,j), i.e. modify the values of red[i][j], green[i][j], and/or blue[i][j].
            setPixelColour(i, j);
        }
    }

    // Display time taken.
    printf("Total time take for the calculations: %g secs.\n", omp_get_wtime() - startTime);
}

//
// Below here is fairly basic OpenGL/GLFW that has nothing to do with parallelisation.
//

void displayImage(void)
{
    // Clear the display
    glClear(GL_COLOR_BUFFER_BIT);

    // Step sizes. The actual image is fixed at [-1,1] in both directions, but for the Mandelbrot set we want the range [-2,2].
    float dx = 2.0f / numPixels_x, dy = 2.0f / numPixels_y;

    // Display the image using a very basic double loop.
    int i, j;
    for (i = 0; i < numPixels_x; i++)
        for (j = 0; j < numPixels_y; j++)
        {
            glColor3f(red[i][j], green[i][j], blue[i][j]);
            glBegin(GL_POLYGON);
            glVertex3f(-1.0f + i * dx, -1.0f + j * dy, 0.0f);
            glVertex3f(-1.0f + (i + 1) * dx, -1.0f + j * dy, 0.0f);
            glVertex3f(-1.0f + (i + 1) * dx, -1.0f + (j + 1) * dy, 0.0f);
            glVertex3f(-1.0f + i * dx, -1.0f + (j + 1) * dy, 0.0f);
            glEnd();
        }
}

//
// Call back functions for GLFW.
//
void graphicsErrorCallBack(int errorCode, const char *message)
{
    printf("Error with OpenGL/GLFW: code %i, message %s\n", errorCode, message);
}

void keyboardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Close if the escape key or 'q' is pressed.
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

//
// Main.
//
int main(int argc, char **argv)
{
    GLFWwindow *window;

    if (!glfwInit())
        return EXIT_FAILURE;

    window = glfwCreateWindow(windowSize_x, windowSize_y, "Mandelbrot set generator: 'q' or <ESC> to quit", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(graphicsErrorCallBack);
    glfwSetKeyCallback(window, keyboardCallBack);
    glfwMakeContextCurrent(window);

    // Generate the image.
    generateImage();

    // Display the image until quitting.
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        displayImage();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}