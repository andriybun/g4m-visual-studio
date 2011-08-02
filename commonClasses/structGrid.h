#ifndef STRUCT_GRID_H_
#define STRUCT_GRID_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>
#include <typeinfo>

using namespace std;

template <class T>
class structGrid
{
private:
	int HorResolution;
	int VerResolution;
	int HorNeigh;
	int VerNeigh;
	T *grid, *gridPrev;  
public:
	structGrid(int HR, int VR, T val);
	structGrid();
	structGrid(const structGrid& g);
	structGrid & operator = (const structGrid& g);
	~structGrid();
	//void ShowArray();                       // prints array
	//void PrintToFile(string fileName, string rastrType = "ESRI");      // print array to file
	//void PrintToFilePrev(string fileName, string rastrType = "ESRI");  // print previous year array to file
	//void ShowArrayPrev();                   // prints array for the previous year
	void update();                          // updates values for previous year to current values
	void set(int x, int y, T val);     // assigns value val to cell [x][y]
	void setPrev(int x, int y, T val); // assigns previous year value val to cell [x][y]
	void inc(int x, int y, T val);     // adds value val to the existing value in cell [x][y]
	T get(int x, int y);               // returns value stored in cell [x][y]
	T getPrev(int x, int y);           // returns value for the previous year stored in cell [x][y]
	void SetNeighNum(int n, int m);     // sets number of neighbour cells to be considered
	T GetMax(int x, int y);            // returns maximum value of all neighbours for the previous year
	T GetMin(int x, int y);            // returns minimum value of all neighbours for the previous year
	T GetAvg(int x, int y);            // returns average value for the previous year
};

//template <class T>
//void structGrid<T>::ShowArray()
//{
//	T val;
//	int isChar = 0;
//	char t1;
//	unsigned char t2;
//	if ((typeid(val).name() == typeid(t1).name()) ||
//		(typeid(val).name() == typeid(t2).name())) isChar = 1;
//	for (int j = 0; j < VerResolution; j++)
//	{
//		cout << j << "|\t";
//		for (int i = 0; i < HorResolution; i++)
//		{
//			if (isChar)
//				cout << (int)grid[j*HorResolution+i] << "\t";
//			else
//				cout << grid[j*HorResolution+i] << "\t";
//		}
//		cout << endl;  
//	}   
//}
//
//template <class T>
//void structGrid<T>::ShowArrayPrev()
//{
//	T val;
//	int isChar = 0;
//	char t1;
//	unsigned char t2;
//	if ((typeid(val).name() == typeid(t1).name()) ||
//		(typeid(val).name() == typeid(t2).name())) isChar = 1;
//	for (int j = 0; j < VerResolution; j++)
//	{
//		cout << j << "|\t";
//		for (int i = 0; i < HorResolution; i++)
//		{
//			if (isChar)
//				cout << (int)gridPrev[j*HorResolution+i] << "\t";
//			else
//				cout << gridPrev[j*HorResolution+i] << "\t";
//		}
//		cout << endl;  
//	}   
//}
//
//template <class T>
//void structGrid<T>::PrintToFile(string fileName, string rastrType)
//{
//	T val;
//	int isChar = 0;
//	char t1;
//	unsigned char t2;
//	if ((typeid(val).name() == typeid(t1).name()) ||
//		(typeid(val).name() == typeid(t2).name())) isChar = 1;
//	ofstream f;
//	fileName = "output\\" + fileName + ".asc";
//	f.open(fileName.c_str(),ios::out);
//	if (f.is_open()) {
//		/////////////////////// Select grid type:
//		if (rastrType == "GRASS")
//		{
//			f << "cols: " << HorResolution << endl;
//			f << "rows: " << VerResolution << endl;
//			f << "west: -180" << endl;
//			f << "south: -90" << endl;
//			f << "north: 90" << endl;
//			f << "east: 180" << endl;
//		}
//		else if (rastrType == "ESRI")
//		{
//			//----------------------------    
//			// ESRI ascii Grid
//			f << "NCOLS " << HorResolution << endl;
//			f << "NROWS " << VerResolution << endl;
//			f << "XLLCORNER -180" << endl;
//			f << "YLLCORNER -90" << endl;
//			f << "CELLSIZE " << 360./HorResolution  << endl;
//			f << "NODATA_VALUE -9999" << endl;
//		}
//		else
//		{
//			cout<<"structGrid3 error message: Specify correct rastrType: ESRI or GRASS"<<endl;
//		}
//		//-----------------------------        
//		for (int j = 0; j < VerResolution; j++)
//		{
//			for (int i = 0; i < HorResolution; i++)
//			{
//				if (isChar)
//					f << (int)grid[(VerResolution-j-1)*HorResolution+i] << " ";
//				else
//					f << grid[(VerResolution-j-1)*HorResolution+i] << " ";
//			}
//			f << endl;  
//		}
//		f.close(); 
//	}
//	else
//	{
//		cout << "Unable to save to file!" << endl;
//	}
//}
//
//template <class T>
//void structGrid<T>::PrintToFilePrev(string fileName, string rastrType)
//{
//	T val;
//	int isChar = 0;
//	char t1;
//	unsigned char t2;
//	if ((typeid(val).name() == typeid(t1).name()) ||
//		(typeid(val).name() == typeid(t2).name())) isChar = 1;
//	ofstream f;
//	fileName = "output\\" + fileName + ".asc";
//	f.open(fileName.c_str(),ios::out);
//	if (f.is_open())
//	{
//		/////////////////////// Select grid type:
//		//GRASS ascii Grid
//		if (rastrType == "GRASS")
//		{
//			f << "cols: " << HorResolution << endl;
//			f << "rows: " << VerResolution << endl;
//			f << "west: -180" << endl;
//			f << "south: -90" << endl;
//			f << "north: 90" << endl;
//			f << "east: 180" << endl;
//		}
//		else if (rastrType == "ESRI")
//		{
//			//----------------------------    
//			// ESRI ascii Grid
//			f << "NCOLS " << HorResolution << endl;
//			f << "NROWS " << VerResolution << endl;
//			f << "XLLCORNER -180" << endl;
//			f << "YLLCORNER -90" << endl;
//			f << "CELLSIZE " << 360./HorResolution  << endl;
//			f << "NODATA_VALUE -9999" << endl;
//		}
//		else
//		{
//			cout<<"structGrid3 error message: Specify correct rastrType: ESRI or GRASS"<<endl;
//		}
//		//-----------------------------        
//		for (int j = 0; j < VerResolution; j++)
//		{
//			for (int i = 0; i < HorResolution; i++)
//			{
//				if (isChar)
//					f << (int)gridPrev[(VerResolution-j-1)*HorResolution+i] << " ";
//				else
//					f << gridPrev[(VerResolution-j-1)*HorResolution+i] << " ";
//			}
//			f << endl;  
//		}
//		f.close(); 
//	}
//	else
//	{
//		cout << "Unable to save to file!" << endl;
//	}
//} 

template <class T>
void structGrid<T>::set(int x, int y, T val)
{
	grid[y*HorResolution+x] = val;
}

template <class T>
void structGrid<T>::setPrev(int x, int y, T val)
{
	gridPrev[y*HorResolution+x] = val;
}

template <class T>
void structGrid<T>::inc(int x, int y, T val)
{
	grid[y*HorResolution+x] += val;
}

template <class T>
T structGrid<T>::get(int x, int y)
{
	return (grid[y*HorResolution+x]);
}

template <class T>
T structGrid<T>::getPrev(int x, int y)
{
	return (gridPrev[y*HorResolution+x]);
}

template <class T>
void structGrid<T>::update()
{
	memcpy(gridPrev,grid,VerResolution*HorResolution*sizeof(T));
}

template <class T>
void structGrid<T>::SetNeighNum(int n, int m)
{
	HorNeigh = n;
	VerNeigh = m;
}

template <class T>
T structGrid<T>::GetMax(int x, int y)
{
	int tmpx = x - HorNeigh;
	if (tmpx < 0) tmpx = HorResolution + tmpx;
	int tmpy = y - VerNeigh;
	if (tmpy < 0) tmpy = 0;
	T maxv = gridPrev[tmpy*HorResolution+tmpx];
	for (int j = tmpy; j <= y + VerNeigh; j++)
	{
		if (j >= VerResolution) break;
		for (int i = -HorNeigh; i <= HorNeigh; i++)
		{
			int ii = x + i;
			if (ii >= HorResolution) ii -= HorResolution;
			else if (ii < 0) ii += HorResolution;
			if (!((ii == x) && (j == y)))
			{
				maxv = max(maxv, gridPrev[j*HorResolution+ii]);
			}
		}
	}  
	return(maxv);
}

template <class T>
T structGrid<T>::GetMin(int x, int y)
{
	int tmpx = x - HorNeigh;
	if (tmpx < 0) tmpx = HorResolution + tmpx;
	int tmpy = y - VerNeigh;
	if (tmpy < 0) tmpy = 0;
	T minv = gridPrev[tmpy*HorResolution+tmpx];
	for (int j = tmpy; j <= y + VerNeigh; j++)
	{
		if (j >= VerResolution) break;
		for (int i = -HorNeigh; i <= HorNeigh; i++)
		{
			int ii = x + i;
			if (ii >= HorResolution) ii -= HorResolution;
			else if (ii < 0) ii += HorResolution;
			if (!((ii == x) && (j == y)))
			{
				minv = min(minv, gridPrev[j*HorResolution+ii]);
			}
		}
	}  
	return(minv);
}

template <class T>
T structGrid<T>::GetAvg(int x, int y)
{
	int count = 0;
	int tmpx = x - HorNeigh;
	if (tmpx < 0) tmpx = HorResolution + tmpx;
	int tmpy = y - VerNeigh;
	if (tmpy < 0) tmpy = 0;
	T sumv;
	memset(sumv, 0, sizeof(T));
	for (int j = tmpy; j <= y + VerNeigh; j++)
	{
		if (j >= VerResolution) break;
		for (int i = -HorNeigh; i <= HorNeigh; i++)
		{
			int ii = x + i;
			if (ii >= HorResolution) ii -= HorResolution;
			else if (ii < 0) ii += HorResolution;
			count++;
			sumv = sumv + gridPrev[j*HorResolution+ii];
		}
	}  
	return(sumv/count);
}

// Class constructor
template <class T>
structGrid<T>::structGrid(int HR, int VR, T val)
{
	HorResolution = HR;
	VerResolution = VR;
	HorNeigh = 1;
	VerNeigh = 1;
	grid = new T[HorResolution*VerResolution];
	gridPrev = new T[HorResolution*VerResolution];
	for (int j = 0; j < VerResolution; j++)
	{
		for (int i = 0; i < HorResolution; i++)
		{
			grid[j*HorResolution+i] = val;
			gridPrev[j*HorResolution+i] = val;
		}
	}
}

// Default constructor
template <class T>
structGrid<T>::structGrid()
{
	HorResolution = 720;
	VerResolution = 360;
	HorNeigh = 1;
	VerNeigh = 1;
	grid = new T[HorResolution*VerResolution];
	gridPrev = new T[HorResolution*VerResolution];
	memset(grid, 0, HorResolution * VerResolution * sizeof(T));
	memset(gridPrev, 0, HorResolution * VerResolution * sizeof(T));
}

// Copy constructor
template <class T>
structGrid<T>::structGrid(const structGrid & g)
{
	cout << "COPY ::>\n";
	HorResolution = g.HorResolution;
	VerResolution = g.VerResolution;
	HorNeigh = g.HorNeigh;
	VerNeigh = g.VerNeigh;
	grid = new T[HorResolution*VerResolution];
	gridPrev = new T[HorResolution*VerResolution];
	memcpy(grid,g.grid,HorResolution*VerResolution*sizeof(T));
	memcpy(gridPrev,g.gridPrev,HorResolution*VerResolution*sizeof(T));
}

// Assignment operator overloading
template <class T>
structGrid<T> & structGrid<T>::operator = (const structGrid & g)
{
	if (this != &g)
	{
		HorResolution = g.HorResolution;
		VerResolution = g.VerResolution;
		HorNeigh = g.HorNeigh;
		VerNeigh = g.VerNeigh;
		grid = new T[HorResolution*VerResolution];
		gridPrev = new T[HorResolution*VerResolution];
		memcpy(grid,g.grid,HorResolution*VerResolution*sizeof(T));
		memcpy(gridPrev,g.gridPrev,HorResolution*VerResolution*sizeof(T));
	}
	return *this;
}

// Destructor
template <class T>
structGrid<T>::~structGrid()
{
	delete []grid;
	delete []gridPrev;
}

#endif