#include <stdio.h>

#include "intf.h"
#include "model.h"
#include "threading.h"

int main(int argc, char * argv[])
{
	int inArr[MAX_ARRAY_SIZE];
	int outArr[MAX_ARRAY_SIZE];

	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		inArr[i] = i + 1 + i * i;
	}

	printf("Model:\n");
	parallelExecute<int, int>(&modelFunc<int, int>, inArr, outArr, MAX_ARRAY_SIZE);

	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		printf("\t%d\n", outArr[i]);
	}

	return 0;
}

