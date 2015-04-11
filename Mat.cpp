#include"Mat.h"

void Mat::initData(int r,int c){
	row=r;
	col=c;
	data=new double*[r];
	for(int i=0;i<r;i++) data[i]=new double[c];
	for(int i=0;i<r;i++) for(int j=0;j<c;j++) data[i][j]=0.;
}
Mat::Mat(){
	initData(1,1);
}
Mat::Mat(int r,int c){
	initData(r,c);
}
Mat::Mat(double** d,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++) for(int j=0;j<c;j++) data[i][j]=d[i][j];
}
Mat::Mat(Vec* v,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++)for(int j=0;j<c;j++)data[i][j]=v[i].getData(j);
}
Mat::~Mat(){
	for(int i=0;i<row;i++) delete[] data[i];
	delete[] data;
}
////////// ////////// ////////// //////////
Mat Mat::operator+(const Mat& m){//operator override +
	if(row!=m.row||col!=m.col) throw "+失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++) for(int j=0;j<col;j++) re.data[i][j]=data[i][j]+m.data[i][j];
	return re;
}
Mat Mat::operator-(const Mat& m){//operator override -
	if(row!=m.row||col!=m.col) throw "-失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++) for(int j=0;j<col;j++) re.data[i][j]=data[i][j]-m.data[i][j];
	return re;
}
Mat Mat::operator*(const Mat& m){//operator override *
	if(col!=m.row) throw "*失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++) for(int j=0;j<m.col;j++) for(int k=0;k<col;k++) re.data[i][j]+=data[i][k]*m.data[k][j];
	return re;
}
Mat Mat::operator=(const Mat& m){//operator override =
	if(data){
		for(int i=0;i<row;i++) delete[] data[i];
		delete[] data;
	}
	initData(m.row,m.col);
	for(int i=0;i<row;i++) for(int j=0;j<col;j++) data[i][j]=m.data[i][j];
	return *this;
}
/*ostream& operator<<(ostream& out,const Mat& m){//operator override <<
	for(int i=0;i<m.row;i++){
		for(int j=0;j<m.col;j++) out<<m.data[i][j]<<" ";
		out<<"\n";
	}
	return out;
}*/
////////// ////////// ////////// //////////
void Mat::setO(){//----0
	for(int i=0;i<row;i++) for(int j=0;j<col;j++) data[i][j]=0.;
}
void Mat::setI(){//----1
	for(int i=0;i<row;i++) for(int j=0;j<col;j++) data[i][j]=1.;
}
void Mat::setData(double** d,int r,int c){
	if(data){
		for(int i=0;i<row;i++) delete[] data[i];
		delete[] data;
	}
	initData(r,c);
	for(int i=0;i<r;i++) for(int j=0;j<c;j++) data[i][j]=d[i][j];
}
void Mat::setData(Vec* v,int r,int c){
	if(data){
		for(int i=0;i<row;i++) delete[] data[i];
		delete[] data;
	}
	initData(r,c);
	for(int i=0;i<r;i++) for(int j=0;j<c;j++) data[i][j]=v[i].getData(j);
}
void Mat::setData(double d,int r,int c){
	data[r][c]=d;
}
Vec Mat::getRowData(int r){
	if(r>=row||r<0) throw "getRowData失敗，超出範圍!";
	Vec v(data[r],col);
	return v;
}
Vec Mat::getColData(int c){
	if(c>=col||c<0) throw "getColData失敗，超出範圍!";
	double *d=new double[row];
	for(int i=0;i<row;i++) d[i]=data[i][c];
	Vec v(d,row);
	delete[] d;
	return v;
}
int Mat::getRow(){
	return row;
}
int Mat::getCol(){
	return col;
}
