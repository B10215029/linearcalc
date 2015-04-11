#ifndef MAT_H
#define MAT_H

#include "Vec.h"

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
	Mat operator+(const Mat& m);
	Mat operator-(const Mat& m);
	Mat operator*(const Mat& m);
	Mat operator=(const Mat& m);
	//cout<< 可能用不到
	//friend ostream& operator<<(ostream&,const Mat&);
////////// ////////// ////////// //////////
	void setO();
	void setI();
	void setData(double** d,int r,int c);
	void setData(Vec* v,int r,int c);
	void setData(double d,int r,int c);
	Vec getRowData(int r);
	Vec getColData(int c);
	int getRow();
	int getCol();
};
#endif
