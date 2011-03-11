#ifndef IPOL_H_
#define IPOL_H_

#include <map>
#include <vector>
#include <cmath>

class countryData;

namespace g4m
{

	//////////////////////////////////////////////////////////
	// Interpolation
	//////////////////////////////////////////////////////////
	template <class IDX, class VAL>
	class ipol		//Interpolate: IDX .. Index, VAL .. Value
	{
	public:
		friend class ::countryData;
		void insert(IDX, VAL);
		void inc(IDX, VAL);
		VAL v(const IDX i = 0);   //returns the value
		ipol(const ipol<IDX, VAL> & g);
		ipol();
		void clear();
		VAL operator[](const IDX);
		void operator*=(const double x);
	private:
		std::map<IDX, VAL> aMap;
		VAL ip(const IDX i, const IDX i0, const IDX i1, const VAL v0, const VAL v1);
	};

	//////////////////////////////////////////////////////////
	// Multidimensional interpolation
	//////////////////////////////////////////////////////////
	template <class IDX, class VAL>
    class ipol<std::vector<IDX>, VAL>
	{
	public:
		void insert(std::vector<IDX>, VAL);
		VAL v(const std::vector<IDX>);   //returns the value
		void clear();
		VAL operator[](const std::vector<IDX> i);
	private:
		std::map<std::vector<IDX>, VAL> aMap; //Data Map
	};


	//////////////////////////////////////////////////////////
	// Fast Interploation where the steps of the index are 1 and starting at 0
	// and the size of the data array is known in advance
	// and complete filled with values
	// Interpolate: IDX .. Index, VAL .. Value
	//////////////////////////////////////////////////////////
	template <class VAL>
    class fipol
	{
	public:
		bool insert(unsigned int, VAL);
		bool insert(unsigned int*, VAL);
		bool insert(std::vector<unsigned int>, VAL);
		VAL g(const double);   //returns the value
		VAL g(double*);
		VAL g(std::vector<double>);
		void fill(VAL);
		VAL operator[](const double);
		void operator*=(const double x);
		fipol();
		fipol(unsigned int);
		fipol(std::vector<unsigned int>);
		fipol(const unsigned int*, const unsigned int dim);
		~fipol();
		void clear(unsigned int);
		void clear(unsigned int*, unsigned int dim);
		void clear(std::vector<unsigned int>);
		unsigned int gs();
	private:
		VAL* aMap;
		unsigned int* n;
		unsigned int dim;
		VAL ip(const double i, const double i0, const double i1, const double v0, const double v1);
	};
}

#endif
