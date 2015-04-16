#include "Mat.h"

void Mat::initData(int r,int c){
	row=r;
	col=c;
	data=new double*[r];
	for(int i=0;i<r;i++)
		data[i]=new double[c];
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=0.;
}
void Mat::deleteData(){
	if(data){
		for(int i=0;i<row;i++)
			delete[] data[i];
		delete[] data;
	}
	row=0;
	col=0;
}
Mat::Mat(){
	initData(1,1);
}
Mat::Mat(int r,int c){
	initData(r,c);
}
Mat::Mat(double** d,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=d[i][j];
}
Mat::Mat(Vec* v,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=v[i].getData(j);
}
Mat::Mat(Vec& v){
	initData(1,v.getDim());
	for(int i=0;i<v.getDim();i++)
		data[0][i]=v.getData(i);
}
Mat::Mat(const Mat& m){
	initData(m.row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=m.data[i][j];
}
Mat::~Mat(){
	deleteData();
}
////////// ////////// ////////// //////////
Mat Mat::operator+(const Mat& m){//operator override +
	if(row!=m.row||col!=m.col) throw "+失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]+m.data[i][j];
	return re;
}
Mat Mat::operator-(const Mat& m){//operator override -
	if(row!=m.row||col!=m.col) throw "-失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]-m.data[i][j];
	return re;
}
Mat Mat::operator*(const Mat& m){//operator override *
	if(col!=m.row) throw "*失敗，維度不同!";
	Mat re(row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<m.col;j++)
			for(int k=0;k<col;k++)
				re.data[i][j]+=data[i][k]*m.data[k][j];
	return re;
}
Mat Mat::operator*(const double c){//operator override *
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]*c;
	return re;
}
Mat Mat::operator=(const Mat& m){//operator override =
	deleteData();
	initData(m.row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=m.data[i][j];
	return *this;
}
//ostream& operator<<(ostream& out,const Mat& m){//operator override <<
//	for(int i=0;i<m.row;i++){
//		for(int j=0;j<m.col;j++) out<<m.data[i][j]<<" ";
//		out<<"\n";
//	}
//	return out;
//}
////////// ////////// ////////// //////////
void Mat::setO(){//----0
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=0.;
}
void Mat::setI(){//----1
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=1.;
}
void Mat::setData(double** d,int r,int c){
	deleteData();
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=d[i][j];
}
void Mat::setData(Vec* v,int r,int c){
	deleteData();
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=v[i].getData(j);
}
void Mat::setData(double d,int r,int c){
	if((r>=row||r<0)||(c>=col||c<0)) throw "setData失敗，超出範圍!";
	data[r][c]=d;
}
Vec Mat::getRowData(int r){
	if(r>=row||r<0) throw "getRowData失敗，超出範圍!";
	Vec v(data[r],col);
	return v;
}
Vec Mat::getColData(int c){
	if(c>=col||c<0) throw "getColData失敗，超出範圍!";
	Vec v(row);
	for(int i=0;i<row;i++)
		v.setData(data[i][c],i);
	return v;
}
int Mat::getRow(){
	return row;
}
int Mat::getCol(){
	return col;
}
Mat Mat::identity(int s){
	Mat m(s,s);
	for(int i=0;i<m.row;i++)
		m.data[i][i]=1;
	return m;
}
int Mat::Rank(){//so hard...
	return row;
}
Mat Mat::trans(){
	Mat m(col,row);
	for(int i=0;i<m.row;i++)
		for(int j=0;j<m.col;j++)
			m.data[i][j]=data[j][i];
	return m;
}
Mat Mat::minor_mat(int r,int c){
	Mat m(row-1,col-1);
	for(int i=0,mi=0;i<row;i++){
		if(i==r) continue;
		for(int j=0,mj=0;j<col;j++){
			if(j==c) continue;
			m.setData(data[i][j],mi,mj++);
		}
		mi++;
	}
	return m;
}
double Mat::cofactor(int r, int c){
	return pow(-1,r+c)*minor_mat(r,c).det();
}
double Mat::det(){
	if(row!=col)
		throw "det失敗，not square matrix!";
	else if(row==3)
		return
				data[0][0]*data[1][1]*data[2][2]+
				data[0][1]*data[1][2]*data[2][0]+
				data[0][2]*data[1][0]*data[2][1]-

				data[0][2]*data[1][1]*data[2][0]-
				data[0][0]*data[1][2]*data[2][1]-
				data[0][1]*data[1][0]*data[2][2];
	else if(row==2)
		return data[0][0]*data[1][1]-data[0][1]*data[1][0];
	else if(row==1)
		return data[0][0];
	double x=0;
	for(int i=0;i<col;i++)
		x+=data[0][i]*pow(-1,0+i)*minor_mat(0,i).det();
	return x;
}
std::string Mat::toString(){
	std::ostringstream out;
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			out<<data[i][j]<<" ";
		out<<"\n";
	}
	return out.str();
}
