#include "ipol.h"

namespace g4m
{
	//////////////////////////////////////////////////////////
	// Interpolation
	//////////////////////////////////////////////////////////

	// Class default constructor
	template <class idxT, class realT>
	ipol<idxT, realT>::ipol() { }

	// Class copy constructor
	template <class idxT, class realT>
	ipol<idxT, realT>::ipol(const ipol<idxT, realT> & g)
	{
		aMap = g.aMap;
	}

	template <class idxT, class realT>
	void ipol<idxT, realT>::clear()
	{
		aMap.clear();
	}

	template <class idxT, class realT>
	void ipol<idxT, realT>::insert(idxT i, realT v)
	{
		aMap.erase(i);
		aMap.insert(std::make_pair(i, v));
	}
	  
	template <class idxT, class realT>
	void ipol<idxT, realT>::inc(idxT i, realT v)
	{
		v += aMap[i];
		aMap.erase(i);
		aMap.insert(std::make_pair(i, v));
	}

	template <class idxT, class realT>
	realT ipol<idxT, realT>::ip(const idxT i, const idxT i0, const idxT i1, const realT v0, const realT v1)
	{
		//interpolate/extrapolate the value for index i
		realT y;
		if(i0 == i1)
		{
			y = (v0 + v1) / 2.;
		}
		else
		{
			y = v0 + (i-i0) / (i1-i0) * (v1-v0);
		}
		return(y);
	}

	template <class idxT, class realT>
	realT ipol<idxT, realT>::v(const idxT i)
	{
		typename std::map<idxT,realT>::iterator lo, up;
		realT y = 0;
		if (aMap.size() > 0)
		{
			up = aMap.lower_bound(i);
			if(up == aMap.end())
			{
				--up;
				lo = up;
			}
			else
			{
				lo = up;
				--lo;
				if(up == aMap.begin())
				{
					lo = aMap.begin();
				}
			}
			y = ip(i, lo->first, up->first, lo->second, up->second);
		}
		return(y);
	}

	template <class idxT, class realT>
	void ipol<idxT, realT>::operator*=(const realT x)
	{
		typename std::map<idxT,realT>::iterator iter;
		iter = aMap.begin();
		while(iter != aMap.end())
		{
		  iter->second *= x;
		  ++iter;
		}
	}

	template <class idxT, class realT>
	realT ipol<idxT, realT>::operator[](const idxT i)
	{
		return(v(i));
	}

	//////////////////////////////////////////////////////////
	// Multidimensional interpolation
	//////////////////////////////////////////////////////////

	template <class idxT, class realT>
	void ipol<std::vector<idxT>, realT>::clear()
	{
		aMap.clear();
	}

	template <class idxT, class realT>
	void ipol<std::vector<idxT>, realT>::insert(std::vector<idxT> i, realT v)
	{
		aMap.insert(make_pair(i, v));
	}

	template <class idxT, class realT>
    realT ipol<std::vector<idxT>, realT>::v(const std::vector<idxT> i)
	{
		typename std::map<std::vector<idxT>, realT>::iterator iter;
		unsigned int regions = 1;
		for(unsigned int j=0; j<i.size(); ++j)
		{
			regions *= 2;
		}
		std::vector<realT>* y = new std::vector<realT>[regions];
		realT* dist = new realT[regions]; //shortest distance in region
		for(unsigned int j=0; j < regions; ++j)
		{
			dist[j] = -1.;
		}
		for(iter = aMap.begin(); iter != aMap.end(); ++iter)
		{
			realT d=0.;
			int pos=0;
			int mul=1;
			for(unsigned int j=0; j < iter->first.size() && j < i.size(); ++j) 
			{
				realT tmp = iter->first[j] - i[j];
				if(tmp > 0.) {pos += mul;}
				//d += tmp * tmp; //Geometric interpolation
				d += std::fabs(tmp);  //Manhaten distance
				mul *= 2;
			}
			if(d <= dist[pos] || dist[pos]<0.)
			{
				if(d < dist[pos] || dist[pos]<0.)
				{
					y[pos].clear(); dist[pos] = d;
				}
				y[pos].push_back(iter->second); 
			}
		}
		for(unsigned int j=0; j < regions; ++j)
		{
			if(dist[j] > 0.)
			{
				//dist[j] = std::sqrt(dist[j]); //Geometric
				dist[j] = dist[j]; //Manhaten distance
			}
		}
		realT ip = 0.;
		realT distsum = 0.;
		int n = 0;
		for(unsigned int j=0; j < regions; ++j)
		{
			if(dist[j] > 0. && n==0)
			{
				for(unsigned int k=0; k<y[j].size(); ++k)
				{
					ip += y[j][k] * 1./dist[j];
					distsum += 1./dist[j];
				}
			}
			else if(dist[j] == 0.)
			{
				if(n==0) {ip = 0.;}
				for(unsigned int k=0; k<y[j].size(); ++k)
				{
					ip += y[j][k];
					++n;
				}
			}
		}
		if(n > 0)
		{
			ip /= n;
		}
		else if(distsum > 0.)
		{
			ip /= distsum;
		}
		else
		{
			ip = 0.;
		}
		realT ret = ip;
		delete[] y;
		delete[] dist;
		return(ret);
	}

	template <class idxT, class realT>
    realT ipol<std::vector<idxT>, realT>::operator[](const std::vector<idxT> i)
	{
		return(v(i));
	}


	//////////////////////////////////////////////////////////
	// Fast interpolation
	//////////////////////////////////////////////////////////

	template <class realT>
    unsigned int fipol<realT>::gs()
	{
		return(n[0]);
	}

	template <class realT>
    fipol<realT>::fipol() : dim(1)
	{
		n = new unsigned int[1];
		n[0] = 1;
		aMap = new realT[n[0]];
	}

	template <class realT>
    fipol<realT>::fipol(unsigned int an) : dim(1)
	{
		n = new unsigned int[dim];
		n[0] = an;
		aMap = new realT[n[0]];
	}

	template <class realT>
    fipol<realT>::fipol(const unsigned int* an, const unsigned int adim)
	{
		dim = adim;
		n = new unsigned int[dim];
		unsigned int slots = n[0] = an[0];
		for(unsigned int i=1; i<dim; ++i)
		{
			n[i] = an[i];
			slots *= n[i];
		}
		aMap = new realT[slots];
	}

	template <class realT>
    fipol<realT>::fipol(std::vector<unsigned int> an) : dim(an.size())
	{
		n = new unsigned int[dim];
		unsigned int slots = n[0] = an[0];
		for(unsigned int i=1; i<dim; ++i)
		{
			n[i] = an[i];
			slots *= n[i];
		}
		aMap = new realT[slots];
	}

	template <class realT>
    fipol<realT>::~fipol()
	{
		delete[] n;
		delete[] aMap;
	}

	template <class realT>
    void fipol<realT>::clear(unsigned int an)
	{
		dim = 1;
		delete[] n;
		delete[] aMap;
		n = new unsigned int[dim];
		n[0] = an;
		aMap = new realT[n[0]];
	}

	template <class realT>
    void fipol<realT>::clear(unsigned int* an, unsigned int adim)
	{
		dim = adim;
		delete[] n;
		delete[] aMap;
		n = new unsigned int[dim];
		unsigned int slots = n[0] = an[0];
		for(unsigned int i=1; i<dim; ++i)
		{
			n[i] = an[i];
			slots *= n[i];
		}
		aMap = new realT[slots];
	}

	template <class realT>
	void fipol<realT>::clear(std::vector<unsigned int> an)
	{
		dim = an.size();
		delete[] n;
		delete[] aMap;
		n = new unsigned int[dim];
		unsigned int slots = n[0] = an[0];
		for(unsigned int i=1; i<dim; ++i)
		{
			n[i] = an[i];
			slots *= n[i];
		}
		aMap = new realT[slots];
	}

	template <class realT>
	void fipol<realT>::fill(realT x)
	{
		unsigned int slots = n[0];
		for(unsigned int i=1; i<dim; ++i)
		{
			slots *= n[i];
		}
		for(unsigned int i=0; i<slots; ++i)
		{
			aMap[i] = x;
		}
	}

	template <class realT>
	bool fipol<realT>::insert(unsigned int i, realT v)
	{
		bool ret = false;
		if(i < n[0])
		{
			aMap[i] = v;
			ret = true;
		}
		return(ret);
	}

	template <class realT>
	bool fipol<realT>::insert(unsigned int* i, realT v)
	{
		bool ret = true;
		unsigned int idx = i[0];
		unsigned int mul = n[0];
		for(unsigned int j=1; j<dim; ++j)
		{
			if(i[j] >= n[j])
			{
				ret = false;
			}
			idx += i[j] * mul;
			mul *= n[j];
		}
		if(ret == true)
		{
			aMap[idx] = v;
		}
		return(ret);
	}

	template <class realT>
    bool fipol<realT>::insert(std::vector<unsigned int> i, realT v)
	{
		bool ret = true;
		unsigned int idx = i[0];
		unsigned int mul = n[0];
		for(unsigned int j=1; j<dim && j<i.size(); ++j)
		{
			if(i[j] >= n[j])
			{
				ret = false;
			}
			idx += i[j] * mul;
			mul *= n[j];
		}
		if(ret == true)
		{
			aMap[idx] = v;
		}
		return(ret);
	}
  
	template <class realT>
	realT fipol<realT>::ip(const realT i, const realT i0, const realT i1, const realT v0, const realT v1)
	{
		//interpolate/extrapolate the value for index i
		realT y;
		if(i0 == i1)
		{
			y = (v0 + v1) / 2.;
		}
		else
		{
		  y = v0 + (i-i0) / (i1-i0) * (v1-v0);
		}
		return(y);
	}

	template <class realT>
    realT fipol<realT>::g(const realT i)
	{
		realT y;
		if(i >= n[0])
		{
			y = aMap[n[0]-1];
		}
		else if(i <= 0)
		{
			y = aMap[0];
		}
		else
		{
			unsigned int i0 = (unsigned int)std::floor(i);
			unsigned int i1 = (unsigned int)std::ceil(i);
			y = ip(i, i0, i1, aMap[i0], aMap[i1]);
		}
		return(y);
	}

	template <class realT>
    realT fipol<realT>::g(realT* i)
	{
		//Test if index is in the possible range
		for(unsigned j = 1; j < dim; ++j)
		{
			if(i[j] >= n[j])
			{
				i[j] = n[j]-1;
			}
			if(i[j] < 0)
			{
				i[j] = 0;
			}
		}
		unsigned int sur = (unsigned int)2 << (dim - 1);
		unsigned int* idx = new unsigned int[sur];
		realT* dist = new realT[sur];
		for(unsigned j = 0; j < sur; ++j)
		{
			idx[j] = -1; dist[j] = -1;
		}
		idx[0] = (unsigned int)std::floor(i[0]);
		idx[1] = (unsigned int)std::ceil(i[0]);
		//dist[0] = std::pow(i[0] - std::floor(i[0]), 2); //Geometric
		//dist[1] = std::pow(i[0] - std::ceil(i[0]), 2);  //Geometric
		dist[0] = std::fabs(i[0] - std::floor(i[0])); //Manhaten distance
		dist[1] = 1. - dist[0];                       //Manhaten distance
		unsigned int mul = n[0];
		for(unsigned j = 1; j < dim; ++j)
		{
			unsigned int t = (unsigned int)2 << (j - 1);
			unsigned int uc = (unsigned int)std::ceil(i[j]) * mul;
			unsigned int uf = (unsigned int)std::floor(i[j]) * mul;
			//realT dc = std::pow(i[j] - std::ceil(i[j]), 2);  //Geometric
			//realT df = std::pow(i[j] - std::floor(i[j]), 2); //Geometric
			realT dc = std::fabs(i[j] - std::ceil(i[j]));  //Manhaten distance
			realT df = 1. - dc;                            //Manhaten distance
			for(unsigned int k=0; k<t; ++k)
			{
				idx[k+t] = idx[k] + uc;
				idx[k] += uf;
				dist[k+t] = dist[k] + dc;
				dist[k] += df;
			}
			mul *= n[j];
		}
		//for(unsigned j = 0; j < sur; ++j) { //Geometric
		//  if(dist[j] > 0.) {dist[j] = std::sqrt(dist[j]);}
		//}
		realT sdist = 0.;
		realT sval = 0.;
		for(unsigned j=0; j<sur; ++j)
		{
			if(idx[j] >= 0)
			{
				if(dist[j] > 0.)
				{
					sval += aMap[idx[j]] / dist[j];
					sdist += 1./dist[j];
				}
				else
				{
					sval = aMap[idx[j]];
					sdist = 1.;
					break;
				}
			}
		}
		realT ret = 0.;
		if(sdist > 0.)
		{
			ret = sval / sdist;
		}
		delete[] idx;
		delete[] dist;
		return(ret);
	}

	template <class realT>
    realT fipol<realT>::g(std::vector<realT> i)
	{
		//Test if index is in the possible range
		for(unsigned j = 1; j < dim && j < i.size(); ++j)
		{
			if(i[j] >= n[j])
			{
				i[j] = n[j]-1;
			}
			if(i[j] < 0)
			{
				i[j] = 0;
			}
		}
		unsigned int sur = (unsigned int)2 << (dim - 1);
		unsigned int* idx = new unsigned int[sur];
		realT* dist = new realT[sur];
		for(unsigned j = 0; j < sur; ++j)
		{
			idx[j] = -1; dist[j] = -1;
		}
		idx[0] = (unsigned int)std::floor(i[0]);
		idx[1] = (unsigned int)std::ceil(i[0]);
		//dist[0] = std::pow(i[0] - std::floor(i[0]), 2); //Geometric
		//dist[1] = std::pow(i[0] - std::ceil(i[0]), 2);  //Geometric
		dist[0] = std::fabs(i[0] - std::floor(i[0])); //Manhaten distance
		dist[1] = 1. - dist[0];                       //Manhaten distance
		unsigned int mul = n[0];
		for(unsigned j = 1; j < dim && j < i.size(); ++j)
		{
			unsigned int t = (unsigned int)2 << (j - 1);
			unsigned int uc = (unsigned int)std::ceil(i[j]) * mul;
			unsigned int uf = (unsigned int)std::floor(i[j]) * mul;
			//realT dc = std::pow(i[j] - std::ceil(i[j]), 2);  //Geometric
			//realT df = std::pow(i[j] - std::floor(i[j]), 2); //Geometric
			realT dc = std::fabs(i[j] - std::ceil(i[j]));  //Manhaten distance
			realT df = (realT)1 - dc;                            //Manhaten distance
			for(unsigned int k=0; k<t; ++k)
			{
				idx[k+t] = idx[k] + uc;
				idx[k] += uf;
				dist[k+t] = dist[k] + dc;
				dist[k] += df;
			}
			mul *= n[j];
		}
		//for(unsigned j = 0; j < sur; ++j) { //Geometric
		//  if(dist[j] > 0.) {dist[j] = std::sqrt(dist[j]);}
		//}
		realT sdist = 0.;
		realT sval = 0.;
		for(unsigned j=0; j<sur; ++j)
		{
			if(idx[j] >= 0)
			{
				if(dist[j] > 0.)
				{
					sval += aMap[idx[j]] / dist[j];
					sdist += 1. / dist[j];
				}
				else
				{
					sval = aMap[idx[j]];
					sdist = 1.;
					break;
				}
			}
		}
		realT ret = 0.;
		if(sdist > 0.)
		{
			ret = sval / sdist;
		}
		delete[] idx;
		delete[] dist;
		return(ret);
  }

	//template <class idxT, class realT>
	//void ipol<idxT, realT>::operator*=(const realT x)
	//{
	//	typename std::map<idxT,realT>::iterator iter;
	//	iter = aMap.begin();
	//	while(iter != aMap.end() )
	//	{
	//		iter->second *= x;
	//		++iter;
	//	}
	//}

	//template <class idxT, class realT>
	//realT ipol<idxT, realT>::operator[](const idxT i)
	//{
	//	return(g(i));
	//}
	
	// Important: add explicit declarations here for the types needed
	template class ipol<double, double>;
	template class ipol<double, int>;
	template class ipol<int, double>;
	template class ipol<float, float>;
	template class ipol<float, int>;
	template class ipol<int, float>;
	template class ipol<int, int>;

	template class fipol<float>;
	template class fipol<double>;
	//template class fipol<int>;
}