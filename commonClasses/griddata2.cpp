#include "griddata2.h"

template <class T>
void griddata2<T>::ShowArray()
{
	T val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) ||
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	for (int j = 0; j < VerResolution; j++)
	{
		cout << j << "|\t";
		for (int i = 0; i < HorResolution; i++)
		{
			if (isChar)
				cout << (int)grid[j*HorResolution+i] << "\t";
			else
				cout << grid[j*HorResolution+i] << "\t";
		}
		cout << endl;  
	}   
}

template <class T>
void griddata2<T>::ShowArrayPrev()
{
	T val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) ||
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	for (int j = 0; j < VerResolution; j++)
	{
		cout << j << "|\t";
		for (int i = 0; i < HorResolution; i++)
		{
			if (isChar)
				cout << (int)gridPrev[j*HorResolution+i] << "\t";
			else
				cout << gridPrev[j*HorResolution+i] << "\t";
		}
		cout << endl;  
	}   
}

template <class T>
void griddata2<T>::PrintToFile(string fileName, string rastrType)
{
	T val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) ||
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	ofstream f;
	fileName = "output\\" + fileName + ".asc";
	f.open(fileName.c_str(),ios::out);
	if (f.is_open()) {
	/////////////////////// Select grid type:
	if (rastrType == "GRASS")
	{
		f << "cols: " << HorResolution << endl;
		f << "rows: " << VerResolution << endl;
		f << "west: -180" << endl;
		f << "south: -90" << endl;
		f << "north: 90" << endl;
		f << "east: 180" << endl;
	}
	else if (rastrType == "ESRI")
	{
		//----------------------------    
		// ESRI ascii Grid
		f << "NCOLS " << HorResolution << endl;
		f << "NROWS " << VerResolution << endl;
		f << "XLLCORNER -180" << endl;
		f << "YLLCORNER -90" << endl;
		f << "CELLSIZE " << 360./HorResolution  << endl;
		f << "NODATA_VALUE -9999" << endl;
	}
	else
	{
		cout<<"griddata23 error message: Specify correct rastrType: ESRI or GRASS"<<endl;
	}
	//-----------------------------        
    for (int j = 0; j < VerResolution; j++)
    {
		for (int i = 0; i < HorResolution; i++)
		{
			if (isChar)
				f << (int)grid[(VerResolution-j-1)*HorResolution+i] << " ";
			else
				f << grid[(VerResolution-j-1)*HorResolution+i] << " ";
		}
		f << endl;  
    }
    f.close(); 
	}
	else
	{
		cout << "Unable to save to file!" << endl;
	}
}

template <class T>
void griddata2<T>::PrintToFilePrev(string fileName, string rastrType)
{
	T val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) ||
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	ofstream f;
	fileName = "output\\" + fileName + ".asc";
	f.open(fileName.c_str(),ios::out);
	if (f.is_open())
	{
		/////////////////////// Select grid type:
		//GRASS ascii Grid
		if (rastrType == "GRASS")
		{
			f << "cols: " << HorResolution << endl;
			f << "rows: " << VerResolution << endl;
			f << "west: -180" << endl;
			f << "south: -90" << endl;
			f << "north: 90" << endl;
			f << "east: 180" << endl;
		}
		else if (rastrType == "ESRI")
		{
			//----------------------------    
			// ESRI ascii Grid
			f << "NCOLS " << HorResolution << endl;
			f << "NROWS " << VerResolution << endl;
			f << "XLLCORNER -180" << endl;
			f << "YLLCORNER -90" << endl;
			f << "CELLSIZE " << 360./HorResolution  << endl;
			f << "NODATA_VALUE -9999" << endl;
		}
		else
		{
			cout<<"griddata23 error message: Specify correct rastrType: ESRI or GRASS"<<endl;
		}
		//-----------------------------        
		for (int j = 0; j < VerResolution; j++)
		{
			for (int i = 0; i < HorResolution; i++)
			{
				if (isChar)
					f << (int)gridPrev[(VerResolution-j-1)*HorResolution+i] << " ";
				else
					f << gridPrev[(VerResolution-j-1)*HorResolution+i] << " ";
			}
			f << endl;  
		}
		f.close(); 
	}
	else
	{
		cout << "Unable to save to file!" << endl;
	}
} 

template <class T>
void griddata2<T>::set(int x, int y, T val)
{
	grid[y*HorResolution+x] = val;
}

template <class T>
void griddata2<T>::setPrev(int x, int y, T val)
{
	gridPrev[y*HorResolution+x] = val;
}

template <class T>
void griddata2<T>::inc(int x, int y, T val)
{
	grid[y*HorResolution+x] += val;
}

template <class T>
T griddata2<T>::get(int x, int y)
{
	return (grid[y*HorResolution+x]);
}

template <class T>
T griddata2<T>::getPrev(int x, int y)
{
	return (gridPrev[y*HorResolution+x]);
}

template <class T>
void griddata2<T>::update()
{
	memcpy(gridPrev,grid,VerResolution*HorResolution*sizeof(T));
}

template <class T>
void griddata2<T>::SetNeighNum(int n, int m)
{
	HorNeigh = n;
	VerNeigh = m;
}

template <class T>
T griddata2<T>::GetMax(int x, int y)
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
			if ((gridPrev[j*HorResolution+ii] > maxv) && !((ii == x) && (j == y)))
				maxv = gridPrev[j*HorResolution+ii];
		}
	}  
	return(maxv);
}

template <class T>
T griddata2<T>::GetMin(int x, int y)
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
			if ((gridPrev[j*HorResolution+ii] < minv) && !((ii == x) && (j == y)))
				minv = gridPrev[j*HorResolution+ii];
		}
	}  
	return(minv);
}

template <class T>
T griddata2<T>::GetAvg(int x, int y)
{
	int count = 0;
	int tmpx = x - HorNeigh;
	if (tmpx < 0) tmpx = HorResolution + tmpx;
	int tmpy = y - VerNeigh;
	if (tmpy < 0) tmpy = 0;
	T sumv = 0;
	for (int j = tmpy; j <= y + VerNeigh; j++)
	{
		if (j >= VerResolution) break;
		for (int i = -HorNeigh; i <= HorNeigh; i++)
		{
			int ii = x + i;
			if (ii >= HorResolution) ii -= HorResolution;
			else if (ii < 0) ii += HorResolution;
			count++;
			sumv += gridPrev[j*HorResolution+ii];
		}
	}  
	return(sumv/count);
}

// Class constructor
template <class T>
griddata2<T>::griddata2(int HR, int VR, T val)
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
griddata2<T>::griddata2()
{
	HorResolution = 720;
	VerResolution = 360;
	HorNeigh = 1;
	VerNeigh = 1;
	T val = 0;
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

// Copy constructor
template <class T>
griddata2<T>::griddata2(const griddata2 & g)
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
griddata2<T> & griddata2<T>::operator = (const griddata2 & g)
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
griddata2<T>::~griddata2()
{
	delete []grid;
	delete []gridPrev;
}

template class griddata2<char>;
template class griddata2<unsigned char>;
template class griddata2<int>;
template class griddata2<long long>;
template class griddata2<float>;
template class griddata2<double>;