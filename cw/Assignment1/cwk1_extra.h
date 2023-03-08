//
// Assessment include for Coursework 1.
//
// DO NOT ALTER OR REWRITE ANY OF THESE ROUTINES.
//

//
// Stack variablea and routines for stack management.
//

// The stack array, the maximum stack size, and the current stack size, are all global.
int *stack;
int maxStackSize = 0;
int stackSize = 0;

// Allocate memory for the stack, i.e. to be maxStackSize in size.
void allocateStack()
{
	// Initialise the stack - an array of integers.
	stack = (int *)malloc(sizeof(int) * maxStackSize);

	// Initialise the current stack size to zero.
	stackSize = 0;
}

// Finalise the stack - free memory.
void finaliseStack()
{
	free(stack);
}

// Display the stack.
void displayStack(int initOrFinal)
{
	// Check 'initOrFinal' is 0 or 1.
	if (initOrFinal != 0 && initOrFinal != 1)
	{
		printf("Error: Must call 'displayStack()' with an argument of 0 (for the initial stack) or 1 (for the final stack).\n");
		return;
	}

	// Header line for the initial or final stack.
	if (initOrFinal == 0)
		printf("Initial stack:\n");
	else
		printf("\nFinal stack:\n");

	// Simple message for an empty stack.
	if (!stackSize)
	{
		printf("<empty stack>\n");
		return;
	}

	// Display stack in full. Note need to reverse the order here, as 'top' usually means the last one added.
	printf("<top of stack>\n");
	for (int i = stackSize - 1; i >= 0; i--)
		printf("%i\n", stack[i]);
	printf("<bottom of stack>\n");
}

//
// Parse the command line and set all parameters given as function arguments - which does not include the maximum stack size,
// which is a global variable and is set here. In case of error, prints a message and returns -1.
//
int parseCommandLine(int argc, char **argv, int *initSize, int *numPop, int *invert, int *rotate)
{
	if (argc != 6)
	{
		printf("Call as\n\n./cwk1 a b c d e\n\nwhere\n\n");
		printf(" a : maximum stack size; must be positive.\n");
		printf(" b : initial stack size; cannot be negative, but can exceed the maximum.\n");
		printf(" c : number of items to 'pop' from the initial stack. Cannot be negative or exceed the initial stack size.\n");
		printf(" d : 1 to invert the stack, 0 to not invert the stack.\n");
		printf(" e : rotate the stack this number of items depth (see coursework instructions). 0 means no rotation. Cannot be negative.\n");

		return -1;
	}

	// The first argument must be positive, and is stored in the global variable 'maxStackSize'.
	maxStackSize = atoi(argv[1]);
	if (maxStackSize <= 0)
	{
		printf("Error: The maximum stack size (%i; first argument) must be positive.\n", maxStackSize);
		return -1;
	}

	// The second argument can be zero but not negative.
	*initSize = atoi(argv[2]);
	if (*initSize < 0)
	{
		printf("Error: The initial stack size (%i; second argument) cannot be negative.\n", *initSize);
		return -1;
	}

	// The third argument must be the number to remove. Cannot be negative or larger than the initial stack size.
	*numPop = atoi(argv[3]);
	if (*numPop > *initSize)
	{
		printf("Error: The number of items to pop cannot exceed the number added.\n");
		return -1;
	}
	if (*numPop < 0)
	{
		printf("Error: The number of items to 'pop' (%i; third argument) cannot be negative.\n", *numPop);
		return -1;
	}

	// The fourth argument must be zero or one.
	*invert = atoi(argv[4]);
	if (*invert != 0 && *invert != 1)
	{
		printf("Error: The 'invert stack' flag (%i; fourth argument) can only be 0 or 1.\n", *invert);
		return -1;
	}

	// The fifth and final argument cannot be negative or larger than the stack.
	*rotate = atoi(argv[5]);
	if (*rotate > *initSize - *numPop)
	{
		printf("Error: Depth of rotation cannot exceed the stack size after popping.\n");
		return -1;
	}
	if (*rotate < 0)
	{
		printf("Error: The 'rotate' value (%i; fifth/final argument) cannot be negative.\n", *rotate);
		return -1;
	}

	// Zero (or any value over than -1) means the arguments are al valid.
	return 0;
}
