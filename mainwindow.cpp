#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

Mat MainWindow::toPostfix(QString& inputStr){
	QStack<Mat> tempResult;
	QStringList opList;
	for(int i=0;i<inputStr.size();i++){
		if(inputStr[i].isDigit()){
			double num=inputStr[i].toLatin1()-48,fraction=0;
			while(inputStr[i+1].isDigit())
				num=num*10+inputStr[++i].toLatin1()-48;
			if(inputStr[i+1]=='.'){
				i++;
				int count=1;
				while(inputStr[i+1].isDigit())
					fraction+=(inputStr[++i].toLatin1()-48)/pow(10.0,count++);
				num+=fraction;
			}
			tempResult.push(Mat::identity(1)*num);
		}
		else if(inputStr[i]=='.'){
			double fraction=0;
			int count=1;
			while(inputStr[i+1].isDigit())
				fraction+=(inputStr[++i].toLatin1()-48)/pow(10.0,count++);
			tempResult.push(Mat::identity(1)*fraction);
		}
		else if(inputStr[i]=='v'){
			int indexNum,indexOffset=-97;
			while(inputStr[++i]=='z') //vza=v[25]
				indexOffset+=25;
			if(!inputStr[i].isLetter()) throw "Input Error!";
			indexNum=inputStr[i].toLatin1()+indexOffset;
			if(indexNum>=v.size()) throw "V Out Of Range!";
			tempResult.push(Mat(v[indexNum]));
			if(inputStr[i+1]=='~'){
				if(inputStr[i+2]=='v'){
					int lastIndexNum;
					indexOffset=-97;
					i+=2;
					while(inputStr[++i]=='z') //vza=v[25]
						indexOffset+=25;
					if(!inputStr[i].isLetter()) throw "Input Error!";
					lastIndexNum=inputStr[i].toLatin1()+indexOffset;
					if(lastIndexNum>=v.size()) throw "V Out Of Range!";
					for(int j=indexNum+1;j<=lastIndexNum;j++)
						tempResult.push(Mat(v[j]));
				}
				else
					throw "Input Error!";
			}
		}
		else if(inputStr[i]=='m'){
			int indexNum,indexOffset=-97;
			while(inputStr[++i]=='z') //mza=m[25]
				indexOffset+=25;
			if(!inputStr[i].isLetter()) throw "Input Error!";
			indexNum=inputStr[i].toLatin1()+indexOffset;
			if(indexNum>=m.size()) throw "M Out Of Range!";
			tempResult.push(m[indexNum]);
			if(inputStr[i+1]=='~'){
				if(inputStr[i+2]=='m'){
					int lastIndexNum;
					indexOffset=-97;
					i+=2;
					while(inputStr[++i]=='z') //mza=m[25]
						indexOffset+=25;
					if(!inputStr[i].isLetter()) throw "Input Error!";
					lastIndexNum=inputStr[i].toLatin1()+indexOffset;
					if(lastIndexNum>=v.size()) throw "M Out Of Range!";
					for(int j=indexNum+1;j<=lastIndexNum;j++)
						tempResult.push(Mat(m[j]));
				}
				else
					throw "Input Error!";
			}
		}
		else if(inputStr[i]=='+'){
			if(!(opList.isEmpty()||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="+";
		}
		else if(inputStr[i]=='-'){
			if(!(opList.isEmpty()||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="-";
		}
		else if(inputStr[i]=='*'){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="*";
		}
		else if(inputStr[i]=='/'){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="/";
		}
		else if(inputStr[i]=='(')
			opList+="(";
		else if(inputStr[i]==')'){
			if(opList.isEmpty()) throw "Input Error!";
			while(opList.last()!="("){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
				if(opList.isEmpty()) throw "Input Error! '(' Not Found";
			}
			opList.removeLast();
		}
		/////向量指令/////
		else if(inputStr.mid(i,6)=="normal"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="normal";
			i+=5;
		}
		else if(inputStr.mid(i,4)=="norm"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="norm";
			i+=3;
		}
		else if(inputStr.mid(i,5)=="cross"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="cross";
			i+=4;
		}
		else if(inputStr.mid(i,3)=="com"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="com";
			i+=2;
		}
		else if(inputStr.mid(i,4)=="proj"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="proj";
			i+=3;
		}
		else if(inputStr.mid(i,4)=="area"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="area";
			i+=3;
		}
		else if(inputStr.mid(i,6)=="ispara"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="ispara";
			i+=5;
		}
		else if(inputStr.mid(i,6)=="isorth"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="isorth";
			i+=5;
		}
		else if(inputStr.mid(i,5)=="angle"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="angle";
			i+=4;
		}
		else if(inputStr.mid(i,2)=="pn"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="pn";
			i+=1;
		}
		else if(inputStr.mid(i,4)=="isli"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="isli";
			i+=3;
		}
		else if(inputStr.mid(i,2)=="ob"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="ob";
			i+=1;
		}
		/////矩陣指令/////
		else if(inputStr.mid(i,4)=="rank"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="rank";
			i+=3;
		}
		else if(inputStr.mid(i,5)=="trans"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="trans";
			i+=4;
		}
		else if(inputStr.mid(i,3)=="ssl"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="ssl";
			i+=2;
		}
		else if(inputStr.mid(i,3)=="det"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="det";
			i+=2;
		}
		else if(inputStr.mid(i,3)=="inv"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="inv";
			i+=2;
		}
		else if(inputStr.mid(i,3)=="adj"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="adj";
			i+=2;
		}
		else if(inputStr.mid(i,4)=="null"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="null";
			i+=3;
		}
		else if(inputStr.mid(i,10)=="eigen_vals"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="eigen_vals";
			i+=9;
		}
		else if(inputStr.mid(i,10)=="eigen_vecs"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="eigen_vecs";
			i+=9;
		}
		else if(inputStr.mid(i,5)=="eigen"){//不可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="eigen";
			i+=4;
		}
		else if(inputStr.mid(i,6)=="pm_val"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="pm_val";
			i+=5;
		}
		else if(inputStr.mid(i,6)=="pm_vec"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="pm_vec";
			i+=5;
		}
		else if(inputStr.mid(i,2)=="pm"){//不可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="pm";
			i+=1;
		}
		else if(inputStr.mid(i,2)=="ls"){
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="ls";
			i+=1;
		}
		else if(inputStr.mid(i,4)=="lu_l"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="lu_l";
			i+=3;
		}
		else if(inputStr.mid(i,4)=="lu_u"){//可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="lu_u";
			i+=3;
		}
		else if(inputStr.mid(i,2)=="lu"){//不可與其他指令並用
			if(!(opList.isEmpty()||opList.last()=="+"||opList.last()=="-"||opList.last()=="*"||opList.last()=="/"||opList.last()=="(")){
				QString inst=opList.last();
				opList.removeLast();
				calc(inst,tempResult);
			}
			opList+="lu";
			i+=1;
		}
	}
	while(!opList.isEmpty()){
		QString inst=opList.last();
		opList.removeLast();
		calc(inst,tempResult);
	}
	if(tempResult.size()!=1) throw "Input Error!";
	return tempResult.pop();
}

void MainWindow::calc(QString& inst,QStack<Mat>& numStack){//不管是向量還是矩陣都用矩陣回傳(numStack)
	if(inst=="+"){
		if(numStack.size()<2) throw "Input Error powoq!";
		Mat m1=numStack.pop(),m2=numStack.pop(),result;
		result=m2+m1;
		numStack.push(result);
	}
	else if(inst=="-"){
		if(numStack.size()<2) throw "Input Error powOq!";
		Mat m1=numStack.pop(),m2=numStack.pop(),result;
		result=m2-m1;
		numStack.push(result);
	}
	else if(inst=="*"){
		if(numStack.size()<2) throw "Input Error pOwoq!";
		Mat m1=numStack.pop(),m2=numStack.pop(),result;
		if(m1.getRow()==1&&m1.getCol()==1)
			result=m2*m1.getColData(0).getData(0);
		else if(m1.getRow()==1&&m1.getCol()>1)
			result=m2*m1.trans();
		else
			result=m2*m1;
		numStack.push(result);
	}
	else if(inst=="/"){
		if(numStack.size()<2) throw "Input Error pOwOq!";
		Mat m1=numStack.pop(),m2=numStack.pop(),result;
		if(m1.getRow()==1&&m1.getCol()==1)
			result=m2/m1.getColData(0).getData(0);
		else
			throw "Input Error poWoq!";
		numStack.push(result);
	}
	/////向量指令/////
	else if(inst=="norm"){
		if(numStack.size()<1) throw "請輸入向量~";
		if(numStack.top().getRow()!=1) throw "Input Error poWOq!";
		Vec v1=numStack.pop().getRowData(0);
		numStack.push(Mat::identity(1)*v1.norm());
	}
	else if(inst=="normal"){
		if(numStack.size()<1) throw "請輸入向量~";
		if(numStack.top().getRow()!=1) throw "Input Error pOWoq!";
		Vec v1=numStack.pop().getRowData(0);
		v1=v1.normal();
		numStack.push(Mat(v1));
	}
	else if(inst=="cross"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error pOWOq!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		v1=v2.cross3(v1);
		numStack.push(Mat(v1));
	}
	else if(inst=="com"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error p0w0q!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		numStack.push(Mat::identity(1)*v2.comp(v1));
	}
	else if(inst=="proj"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error p0woq!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		v1=v2.proj(v1);
		numStack.push(Mat(v1));
	}
	else if(inst=="area"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error pow0q!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		numStack.push(Mat::identity(1)*v2.area(v1));
	}
	else if(inst=="ispara"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error p0wOq!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		numStack.push(Mat::identity(1)*v2.isParal(v1));
		ui->textBrowser->append(numStack.last().getColData(0).getData(0)?"Yes":"No");
	}
	else if(inst=="isorth"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error pOw0q!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		numStack.push(Mat::identity(1)*v2.isOrtho(v1));
		ui->textBrowser->append(numStack.last().getColData(0).getData(0)?"Yes":"No");
	}
	else if(inst=="angle"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error p0W0q!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		numStack.push(Mat::identity(1)*v2.angle_degree(v1));
	}
	else if(inst=="pn"){
		if(numStack.size()<2) throw "請輸入兩個向量~";
		Mat m1=numStack.pop(),m2=numStack.pop();
		if(m1.getRow()!=1||m2.getRow()!=1) throw "Input Error p0Woq!";
		Vec v1=m1.getRowData(0),v2=m2.getRowData(0);
		v1=v2.planeNormal(v1);
		numStack.push(Mat(v1));
	}
	else if(inst=="isli"){
		if(numStack.size()<1) throw "請輸入向量~";
		Mat m1=numStack.last();
		if(m1.getRow()!=1) throw "Input Error poW0q!";
		else if(numStack.size()<m1.getCol()) throw "請輸入N個N維向量~ pOW0q!";
		Vec *vs=new Vec[m1.getCol()];
		for(int i=0;i<m1.getCol();i++){
			vs[i]=numStack.pop().getRowData(0);
			if(vs[i].getDim()!=m1.getCol()) throw "請輸入N個N維向量~ p0WOq!";
		}
		m1=Mat(vs,m1.getCol(),m1.getCol());
		numStack.push(Mat::identity(1)*m1.isLI());
		ui->textBrowser->append(numStack.last().getColData(0).getData(0)?"Yes":"No");
		delete[] vs;
	}
	else if(inst=="ob"){
		if(numStack.size()<1) throw "請輸入向量~";
		Mat m1=numStack.last();
		if(m1.getRow()!=1) throw "Input Error!";
		else if(numStack.size()<m1.getCol()) throw "請輸入N個N維向量~";
		Vec *vs=new Vec[m1.getCol()];
		for(int i=m1.getCol()-1;i>=0;i--){
			vs[i]=numStack.pop().getRowData(0);
			if(vs[i].getDim()!=m1.getCol()) throw "請輸入N個N維向量~";
		}
		Vec::ob(vs);
		numStack.push(Mat(vs,m1.getCol(),m1.getCol()));
		delete[] vs;
	}
	/////矩陣指令/////
	else if(inst=="rank"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().rank()*Mat::identity(1));
	}
	else if(inst=="trans"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().trans());
	}
	else if(inst=="ssl"){
		if(numStack.size()<2) throw "請輸入兩個矩陣~";
		Mat mb=numStack.pop(),ma=numStack.pop();
		numStack.push(ma.solveSquareLinearSys(mb));
	}
	else if(inst=="det"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().det()*Mat::identity(1));
	}
	else if(inst=="inv"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().inverse());
	}
	else if(inst=="adj"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().adj());
	}
	else if(inst=="null"){
		if(numStack.size()<1) throw "請輸入矩陣~";
		numStack.push(numStack.pop().nullspace());
	}
	else if(inst=="eigen"){//不可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop(),eVecs;
		Vec eVals;
		m1.eigen3(eVecs,eVals);
		ui->textBrowser->append("Eigen Values:\n"+QString::fromStdString(eVals.toString()));
		ui->textBrowser->append("Eigen Vectors:\n"+QString::fromStdString(eVecs.toString()));
		throw "";
	}
	else if(inst=="eigen_vals"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop(),eVecs;
		Vec eVals;
		m1.eigen3(eVecs,eVals);
		numStack.push(eVals);
	}
	else if(inst=="eigen_vecs"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop(),eVecs;
		Vec eVals;
		m1.eigen3(eVecs,eVals);
		numStack.push(eVecs);
	}
	else if(inst=="pm"){//不可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Vec initVec;
		ui->textBrowser->append("Dominant Eigen Value:\n"+QString::number(numStack.pop().powerMethod(initVec)));
		ui->textBrowser->append("Dominant Eigen Vector:\n"+QString::fromStdString(initVec.toString()));
		throw "";
	}
	else if(inst=="pm_val"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Vec initVec;
		numStack.push(numStack.pop().powerMethod(initVec)*Mat::identity(1));
	}
	else if(inst=="pm_vec"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Vec initVec;
		numStack.pop().powerMethod(initVec);
		numStack.push(initVec);
	}
	else if(inst=="ls"){
		if(numStack.size()<2) throw "請輸入兩個矩陣~";
		Mat mb=numStack.pop(),ma=numStack.pop();
		numStack.push(ma.LS(mb));
	}
	else if(inst=="lu"){//不可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop();
		Mat l,u;
		int count;
		m1.LU(l,u,count);
		ui->textBrowser->append("L:\n"+QString::fromStdString(l.toString()));
		ui->textBrowser->append("U:\n"+QString::fromStdString(u.toString()));
		throw "";
	}
	else if(inst=="lu_l"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop();
		Mat l,u;
		int count;
		m1.LU(l,u,count);
		numStack.push(l);
	}
	else if(inst=="lu_u"){//可與其他指令並用
		if(numStack.size()<1) throw "請輸入矩陣~";
		Mat m1=numStack.pop();
		Mat l,u;
		int count;
		m1.LU(l,u,count);
		numStack.push(u);
	}
	/////其他/////
	else
		ui->textBrowser->append("Abuse "+inst+" Instructions!");
}

void MainWindow::on_pushButton_clicked()
{
	try{
		QString inStr=ui->lineEdit->text();
		ui->textBrowser->append(">>>"+inStr);
		inStr=inStr.toLower();
		Mat result=toPostfix(inStr);
		ui->textBrowser->append(QString::fromStdString(result.toString()));
	}
	catch(const char* e){
		ui->textBrowser->append(QString::fromLocal8Bit(e));
	}
	catch(const int e){
		ui->textBrowser->append(QString::number(e));
	}
}

void MainWindow::on_actionOpen_triggered()//Qt讀檔方式
{
	//Open Dialog and Return files Directory
	QStringList fnames=QFileDialog::getOpenFileNames(this,"Open Files" ,
													 "../" , "Text(*.txt);;All(*)");

	for(int ni=0;ni<fnames.size();ni++){
		QFile f(fnames[ni]);

		//Open File and test whether it is error
		if(!f.open(QIODevice::ReadOnly)) ui->textBrowser->append(fnames[ni]+"[Open File Error]");

		//to stream like c++ cin
		QTextStream in(&f);
		in.setCodec("UTF-8");

		//first input is always int
		int num;
		in>>num;

		//Read dataType and Set v, m
		QString dataType;
		for(int i=num;i>0;i--){
			in>>dataType;

			if(dataType=="V"){
				in>>num;
				Vec vv(num);
				for(int j=0;j<num;j++){
					double d;
					in>>d;
					vv.setData(d,j);
				}
				v.push_back(vv);

				//加到下拉選單中
				//QString("Z").repeated(N)把Z重複N次
				//(v.size()-1)/25 每25個加一次
				//97+((v.size()-1)%25 96('a')加上v.size減1(a是0)餘25(a~y共25個)
				ui->comboBox->addItem(QString("V%1").arg(QString("Z").repeated((v.size()-1)/25)+(97+((v.size()-1)%25))));
			}
			else if(dataType=="M"){
				int col;
				in>>num;
				in>>col;
				Mat mm(num,col);
				for(int r=0;r<num;r++){
					for(int c=0;c<col;c++){
						double d;
						in>>d;
						mm.setData(d,r,c);
					}
				}
				m.push_back(mm);

				//加到下拉選單中
				ui->comboBox_2->addItem(QString("M%1").arg(QString("Z").repeated((m.size()-1)/25)+(97+((m.size()-1)%25))));
			}
		}
		//Output to TextBrowser
		if(fnames[ni]!=NULL) ui->textBrowser->append(fnames[ni]+"\n");
	}
}

void MainWindow::on_lineEdit_returnPressed()//在指令輸入按下Enter鍵
{
	this->on_pushButton_clicked();//等同按下RUN鍵
}

void MainWindow::on_comboBox_activated(const QString &arg1)//選擇向量
{
	if(ui->comboBox->currentIndex())
		ui->lineEdit->insert(arg1);
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)//選擇矩陣
{
	if(ui->comboBox_2->currentIndex())
		ui->lineEdit->insert(arg1);
}

void MainWindow::on_comboBox_3_activated(const QString &arg1)//選擇常數
{
	if(ui->comboBox_3->currentIndex())
		ui->lineEdit->insert(arg1);
}

void MainWindow::on_pushButton_2_clicked()//print:用於只有+*-/的運算式或是直接輸出某個變數
{
	ui->lineEdit->setText("Print ");
}

void MainWindow::on_pushButton_3_clicked()//清除輸出
{
	ui->textBrowser->setText("");
}

void MainWindow::on_pushButton_4_clicked()//清除輸入
{
	ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_5_clicked()//清除資料
{
	v.clear();
	m.clear();
	ui->comboBox->clear();
	ui->comboBox->addItem(QString::fromLocal8Bit("選擇向量"));
	ui->comboBox_2->clear();
	ui->comboBox_2->addItem(QString::fromLocal8Bit("選擇矩陣"));
	ui->textBrowser->append("Clean data");
}

void MainWindow::on_actionSave_triggered()
{
	//把輸出存成檔案(不重要，有空再做)
}
