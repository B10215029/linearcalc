#ifndef VEC_H
#define VEC_H
#include <iostream>
#include <sstream>
using namespace std;

class Vec{
protected:
	double* data;
	int dim;
public:
	Vec(void);
	Vec(int s);
	Vec(double* d,int s);
	Vec(const Vec &v);
	~Vec();
	Vec operator+(const Vec& v);
	Vec operator-(const Vec& v);
	friend Vec operator*(const double,const Vec&);
	Vec operator*(const double c);
	Vec operator/(const double c);
	bool operator==(Vec& v);
	bool operator!=(Vec& v);
	Vec operator=(const Vec& v);
	friend ostream& operator<<(ostream&,const Vec&);
	void setO();
	void setI();
	void setData(double* d,int l);
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
	//////////
	string toString(){
		ostringstream out;
		for(int i=0;i<dim;++i){
			if(i==0)
				out<<"(";
			if(i!=dim-1)
				out<<data[i]<<", ";
			else
				out<<data[i]<<")";
		}
		return out.str();
	}

};




#endif
