#include<cmath>
#include<iostream>
using namespace std;
class Vec{
protected:
	double* data;
	int dim;
public:
	Vec(){
		data=new double[1];
		dim=1;
		data[0]=0.;
	}
	Vec(int s):dim(s){
		data=new double[s];
		for(int i=0;i<s;i++) data[i]=0.;
	}
	Vec(double* d,int s):dim(s){
		data=new double[s];
		for(int i=0;i<s;i++) data[i]=d[i];
	}
	Vec(const Vec &v){//----Asign
		dim=v.dim;
		data=new double[dim];
		for(int i=0;i<dim;i++) data[i]=v.data[i];
	}
	~Vec(){
		delete[] data;//delete heap裡的
	}
////////// ////////// ////////// //////////
	inline Vec operator+(const Vec& v){//----Addition
		if(dim!=v.dim) throw "+失敗，維度不同!";
		Vec re(dim);
		for(int i=0;i<dim;i++) re.data[i]=data[i]+v.data[i];
		return re;
	}
	inline Vec operator-(const Vec& v){//----Subtraction
		if(dim!=v.dim) "-失敗，維度不同!";
		Vec re(dim);
		for(int i=0;i<dim;i++) re.data[i]=data[i]-v.data[i];
		return re;
	}
	friend inline Vec operator*(const double,const Vec&);//第1參數非Vec,不能是memberFunc,有其它memberFunc用到,so要宣告,不加friend會當作memberFunc 
	inline Vec operator*(const double c){//----Scalar multiplication
		Vec v(dim);
		for(int i=0;i<dim;i++) v.data[i]=c*data[i];
		return v;
	}
	inline Vec operator/(const double c){//----Scalar division
		if(c==0.) throw "不可除以0";
		Vec v(dim);
		for(int i=0;i<dim;i++) v.data[i]=data[i]/c;
		return v;
	}
	inline bool operator==(Vec& v){//----Equality
		return isEqual(v);
	}
	inline bool operator!=(Vec& v){//----Not Equality
		return !(isEqual(v));
	}
	inline Vec operator=(const Vec& v){//----Asign
		if(data) delete[] data;
		dim=v.dim;
		data=new double[dim];
		for(int i=0;i<dim;i++) data[i]=v.data[i];
		return *this;
	}
	friend ostream& operator<<(ostream&,const Vec&);//加friend才能存取protected
////////// ////////// ////////// //////////
	inline void setO(){//----0
		for(int i=0;i<dim;i++) data[i]=0.;
	}
	inline void setI(){//----1
		for(int i=0;i<dim;i++) data[i]=1.;
	}
	inline void setData(double* d,int l){
		if(data) delete[] data;
		dim=l;
		data=new double[dim];
		for(int i=0;i<dim;i++) data[i]=d[i];
	}
	inline void setData(double d,int i){
		if(i>=dim||i<0) throw "setData失敗，超出範圍!";
		data[i]=d;
	}
	inline double getData(int i){
		if(i>=dim||i<0) throw "getData失敗，超出範圍!";
		return data[i];
	}
	inline int getDim(){
		return dim;
	}
	inline bool isEqual(const Vec& v){//----Equality
		if(dim!=v.dim) return false;
		for(int i=0;i<dim;i++) if(data[i]!=v.data[i]) return false;
		return true;
	}
	inline double distance(const Vec& v){//----Distance Between Two Vectors
		if(dim!=v.dim) throw "distance失敗，維度不同!";
		double d=0.;
		for(int i=0;i<dim;i++) d+=pow(data[i]-v.data[i],2);
		return sqrt(d);
	}
	inline double norm(){//----norm,magnitude,length
		double d=0.;
		for(int i=0;i<dim;i++) d+=pow(data[i],2);
		return sqrt(d);
	}
	inline Vec normal(){//----normal
		Vec v(dim);
		for(int i=0;i<dim;i++) data[i]=v.data[i]/norm();
		return v;
	}
	inline void normalize(){//----normalize
		for(int i=0;i<dim;i++) data[i]=data[i]/norm();
	}
	inline double dot(const Vec& v){//----Dot Product
		if(dim!=v.dim) throw "dot失敗，維度不同!";
		double d=0.;
		for(int i=0;i<dim;i++) d+=data[i]*v.data[i];
		return d;
	}
	inline bool isOrtho(const Vec& v){//----Orthogonal
		if(dim!=v.dim) throw "isOrtho失敗，維度不同!";
		else if(dot(v)==0.) return true;
		else return false;
	}
	inline double angle_cos(Vec& v){//----angle between
		if(dim!=v.dim) throw "angle_cos失敗，維度不同!";
		return dot(v)/norm()/v.norm();
	}
	inline double angle_radian(Vec& v){//----angle between
		if(dim!=v.dim) throw "angle_radian失敗，維度不同!";
		return acos(dot(v)/norm()/v.norm());
	}
	inline double angle_degree(Vec& v){//----angle between
		if(dim!=v.dim) throw "angle_degree失敗，維度不同!";
		return acos(dot(v)/norm()/v.norm())*(180/M_PI);
	}
	inline double comp(Vec& v){//----Component
		if(dim!=v.dim) throw "comp失敗，維度不同!";
		return dot(v)/v.norm();
	}
	inline Vec proj(Vec& v){//----Projection
		if(dim!=v.dim) throw "proj失敗，維度不同!";
		Vec re=comp(v)*(v/v.norm());
		return re;
	}
	inline Vec cross3(const Vec& v){
		if(dim!=3||v.dim!=3) throw "cross3失敗，維度不是3!";
		Vec re(3);
		re.data[0]=data[1]*v.data[2]-data[2]*v.data[1];
		re.data[1]=data[0]*v.data[2]-data[2]*v.data[0];
		re.data[2]=data[0]*v.data[1]-data[1]*v.data[0];
		return re;
	}
};
inline Vec operator*(const double c,const Vec& v1){//----Scalar multiplication
	Vec v(v1.dim);
	for(int i=0;i<v1.dim;i++) v.data[i]=c*v1.data[i];
	return v;
}
inline ostream& operator<<(ostream& out,const Vec& v){//----output <<
	for(int i=0;i<v.dim;++i){
		if(i==0) out<<"(";
		if(i!=v.dim-1) out<<v.data[i]<<",";
		else out<<v.data[i]<<")";
	}
	return out;
}/*
int main(){
	double d1[3]={1,2,3},d2[3]={4,5,6},d3[3]={7,8,9};
	Vec v1;
	v1.setData(d3,3);
	Vec v2(v1);
	v2.setData(d2,1);
	//cout<<v.getData(1)+v.getData(2);
	try{
		//v2+v1;
		cout<<v1.distance(v2);
		cout<<"A";
	}
	catch(const char* m){
		cout<<m<<endl;
	}
	return 0;
}*/
