#include <tchar.h>
#include "tests.h"
#include <iostream>

#include "staticArrays.h"
#include "dynamicArrays.h"

int _tmain(int argc, _TCHAR* argv[])
{
	dynamicArray<double> dar;
	//dar.allocate(10);
	dar[1] = 9;
	std::cout << dar[1] << std::endl;

	//testGriddata2();
	testCountryData();
	//testVector2d();
	return 0;
}