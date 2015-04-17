#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <sstream>

class Vec{
protected:
	double* data;
	int dim;
public:
	Vec(void);
	Vec(int s);
	Vec(double* d,int s);
	Vec(const Vec& v);//----Asign
	~Vec();
////////// ////////// ////////// //////////
	Vec operator+(const Vec& v);
	Vec operator-(const Vec& v);
	//第1參數非Vec,不能是memberFunc,有其它memberFunc用到,so要宣告,不加friend會當作memberFunc
	friend Vec operator*(const double c,const Vec& v1);
	Vec operator*(const double c);
	Vec operator/(const double c);
	bool operator==(Vec& v);
	bool operator!=(Vec& v);
	Vec operator=(const Vec& v);
	//cout<< 可能用不到
	//friend ostream& operator<<(ostream&,const Vec&);
////////// ////////// ////////// //////////
	void setO();
	void setI();
	void setData(double* d,int s);
	void setData(double d,int i);
	double getData(int i);
	int getDim();
	bool isEqual(const Vec& v);
	double distance(const Vec& v);
	double norm();
	Vec normal();
	void normalize();
	double dot(const Vec& v);
	bool isOrtho(const Vec& v);
	double angle_cos(Vec& v);
	double angle_radian(Vec& v);
	double angle_degree(Vec& v);
	double comp(Vec& v);
	Vec proj(Vec& v);
	Vec cross3(const Vec& v);
	double cross3_norm(const Vec& v);
	bool isParallel(const Vec& v);
	bool isOrthogonal(const Vec& v);
	bool IsLI(const Vec& v);
////////// ////////// ////////// //////////
	std::string toString();
};

#endif // VEC_H
