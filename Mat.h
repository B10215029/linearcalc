#ifndef MAT_H
#define MAT_H
#include "Vec.h"
#include <iostream>
using namespace std;

class Mat{
protected:
	double** data;
	int row,col;
	inline void initData(int r,int c);
public:
	Mat();
	Mat(int r,int c);
	Mat(double** d,int r,int c);
	Mat(Vec* v,int r,int c);
	~Mat();
////////// ////////// ////////// //////////
	inline Mat operator+(const Mat& m);
	inline Mat operator-(const Mat& m);
	inline Mat operator*(const Mat& m);
	inline Mat operator=(const Mat& m);
	friend ostream& operator<<(ostream&,const Mat&);
////////// ////////// ////////// //////////
	inline void setO();
	inline void setI();
	inline void setData(double** d,int r,int c);
	inline void setData(Vec* v,int r,int c);
	inline void setData(double d,int r,int c);
	inline Vec getRowData(int r);
	inline Vec getColData(int c);
	inline int getRow();
	inline int getCol();
};

#endif
