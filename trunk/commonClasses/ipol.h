#ifndef IPOL_H_
#define IPOL_H_

#include <map>
#include <vector>
#include <cmath>

template <class realT>
class countryData;

//template class countryData<float>;
//template class countryData<double>;

namespace g4m
{
	//////////////////////////////////////////////////////////
	// Interpolation
	//////////////////////////////////////////////////////////
	template <class idxT, class realT>
	class ipol		//Interpolate: idxT .. Index, realT .. Value
	{
	public:
		template <class realT> friend class ::countryData;
		void insert(idxT, realT);
		void inc(idxT, realT);
		realT v(const idxT i = 0);   //returns the value
		ipol(const ipol<idxT, realT> & g);
		ipol();
		void clear();
		realT operator[](const idxT);
		void operator*=(const realT x);
	private:
		std::map<idxT, realT> aMap;
		realT ip(const idxT i, const idxT i0, const idxT i1, const realT v0, const realT v1);
	};

	//////////////////////////////////////////////////////////
	// Multidimensional interpolation
	//////////////////////////////////////////////////////////
	template <class idxT, class realT>
    class ipol<std::vector<idxT>, realT>
	{
	public:
		void insert(std::vector<idxT>, realT);
		realT v(const std::vector<idxT>);   //returns the value
		void clear();
		realT operator[](const std::vector<idxT> i);
	private:
		std::map<std::vector<idxT>, realT> aMap; //Data Map
	};


	//////////////////////////////////////////////////////////
	// Fast Interploation where the steps of the index are 1 and starting at 0
	// and the size of the data array is known in advance
	// and complete filled with values
	// Interpolate: idxT .. Index, realT .. Value
	//////////////////////////////////////////////////////////
	template <class realT>
    class fipol
	{
	public:
		bool insert(unsigned int, realT);
		bool insert(unsigned int*, realT);
		bool insert(std::vector<unsigned int>, realT);
		realT g(const realT);   //returns the value
		realT g(realT*);
		realT g(std::vector<realT>);
		void fill(realT);
		//realT operator[](const realT);
		//void operator*=(const realT x);
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
		realT* aMap;
		unsigned int* n;
		unsigned int dim;
		realT ip(const realT i, const realT i0, const realT i1, const realT v0, const realT v1);
	};
}

#endif
