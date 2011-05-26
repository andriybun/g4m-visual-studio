#ifndef G4M_INTF_H_
#define G4M_INTF_H_

template <class realT>
struct inCellDataT
{
	realT STOPGAP;
};

template <class realT>
struct inCommonDataT
{
	realT STOPGAP;
};

template <class realT>
struct outCellDataT
{
	realT STOPGAP;
};

#endif