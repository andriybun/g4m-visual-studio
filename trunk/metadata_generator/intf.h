#ifndef MODEL_INTF_H_
#define MODEL_INTF_H_

#define STRUCTDEF(NAME)			struct NAME
#define CONSTDEF(NAME, VALUE)	const int NAME = VALUE;
#define array(TYPE, NAME, SIZE)	TYPE NAME[SIZE]

CONSTDEF(ARRAY_SIZE, 20)				// size of array
CONSTDEF(NEW_CONST, ARRAY_SIZE + 10)	// size of array + 10

STRUCTDEF(struct_one)
{
	int intField;
	float floatField;
	double doubleField;
	array(double, arrayField, ARRAY_SIZE);
};

template <typename realT>
STRUCTDEF(template_struct)
{
	realT realField;
	array(realT, realArray, NEW_CONST);
};

array(int, intArray, ARRAY_SIZE);

#endif