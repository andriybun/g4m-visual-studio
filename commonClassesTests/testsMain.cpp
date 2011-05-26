#include <tchar.h>
#include "tests.h"
#include <iostream>

#include "staticArrays.h"
#include "dynamicArrays.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//array(int, arr, 100);
	//arr[100] = 99;
	//std::cout << arr[100] << std::endl;

	//dynamicArray<double> dar;
	//dar.allocate(10);
	//dar[1] = 9;
	//std::cout << dar[1] << std::endl;

	//testGriddata2();
	//testCountryData();
	testVector2d();
	return 0;
}