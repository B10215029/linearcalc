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

QString MainWindow::toPostfix(QString &inputStr){
	QString outputStr;
	QStack<QChar> operatorStack;
	QMap<QString,int> operatorPriority;
	operatorPriority["("] = 1;
	operatorPriority[")"] = 1;
	operatorPriority["+"] = 2;
	operatorPriority["-"] = 2;
	operatorPriority["*"] = 3;
	operatorPriority["/"] = 3;

	for(int i=0;i<inputStr.size();++i){
		if(inputStr[i].isDigit() || inputStr[i]=='.') //number  //QRegExp("\\-*[1-9]+\\.*\\d*")
			outputStr += inputStr[i];
		else if(inputStr[i].isLetter()) //letter  //QRegExp("V[A-Z]|M[A-Z]")
			outputStr += inputStr[i];
		else if(inputStr[i] == '(')
			operatorStack.push(inputStr[i]);
		else if(inputStr[i] == ')'){
			while(operatorStack.top()!='(')
				outputStr += operatorStack.pop();
			operatorStack.pop();
		}
		else if(QString("+*-/").indexOf(inputStr[i])!=-1){
			while(!operatorStack.isEmpty() && //有東西的話
				  operatorPriority.value(QString(operatorStack.top())) >=
				  operatorPriority.value(QString(inputStr[i]))) //裡面的東西比較大的話
				outputStr += operatorStack.pop(); //就把他丟出來
			operatorStack.push(inputStr[i]);  //最後再把自己丟進去
		}
	}

	while(!operatorStack.empty())
		outputStr += operatorStack.pop();
	return outputStr;
}
//只計算+*-/等operator
//不管是向量還是矩陣都用矩陣回傳
Mat MainWindow::calc(QString &inputStr){
	QStack<MatXChar> stack;//Mat不管向量還是矩陣都能裝，MatXChar的c用來分辨是向量、矩陣還是常數
	for(int i=0;i<inputStr.size();++i){
		if(inputStr[i]=='V'){
			//toLatin1可以把QChar轉成char
			//Latin-1編碼前127字類似Ascii
			MatXChar mxc;
			int indexOffset=-65;
			while(inputStr[++i]=='Z') //VZA=v[25]
				indexOffset+=25;
			if(inputStr[i].toLatin1()+indexOffset>=v.size()) throw "POwOq";
			mxc.m=Mat(v[inputStr[i].toLatin1()+indexOffset]);
			mxc.c='V';
			stack.push(mxc);
		}
		else if(inputStr[i]=='M'){
			MatXChar mxc;
			int indexOffset=-65;
			while(inputStr[++i]=='Z')
				indexOffset+=25;
			if(inputStr[i].toLatin1()+indexOffset>=m.size()) throw "POwOq";
			mxc.m=Mat(m[inputStr[i].toLatin1()+indexOffset]);
			mxc.c='M';
			stack.push(mxc);
		}
		else if(inputStr[i].isDigit()){
			double num = inputStr[i].toLatin1()-48;
			while(inputStr[i+1].isDigit())  //整數部分
				num = num*10+inputStr[++i].toLatin1()-48;
			if(inputStr[i+1]=='.'){  //小數部分
				i++;
				double pointNum=0;
				while(inputStr[i+1].isDigit())
					pointNum = (pointNum+inputStr[++i].toLatin1()-48)/10.;
				num+=pointNum;
			}
			MatXChar mxc;
			mxc.m=Mat::identity(1)*num;
			mxc.c='C';
			stack.push(mxc);
		}

		else if(inputStr[i]=='+'){
			if(stack.size()<2) throw "POwOq";
			MatXChar mxc1=stack.pop(),mxc2=stack.pop(),result;
			if(mxc1.c!=mxc2.c) throw "Input Error!";
			result.m=mxc1.m+mxc2.m;
			result.c=mxc1.c;
			stack.push(result);
		}
		else if(inputStr[i]=='-'){	//負數?
			if(stack.size()<2) throw "POwOq";
			MatXChar mxc1=stack.pop(),mxc2=stack.pop(),result;
			if(mxc1.c!=mxc2.c) throw "Input Error!";
			result.m=mxc1.m-mxc2.m;
			result.c=mxc1.c;
			stack.push(result);
		}

		else if(inputStr[i]=='*'){
			if(stack.size()<2) throw "POwOq";
			MatXChar mxc1=stack.pop(),mxc2=stack.pop(),result;
			int r1=mxc1.m.getRow(),r2=mxc2.m.getRow(),c1=mxc1.m.getCol(),c2=mxc2.m.getCol();
			if((r1!=r2||c1!=c2) && mxc1.c!='M'){
				if(r1==1&&c1==1){
					result.m = mxc2.m * mxc1.m.getRowData(0).getData(0);
					result.c = mxc2.c;
				}
				else if(r2==1&&c2==1){
					result.m =  mxc2.m.getRowData(0).getData(0) * mxc1.m;
					result.c = mxc1.c;
				}
				else if(r1==1){
					result.m =  mxc2.m * mxc1.m.trans();
					result.c = 'M';
				}
//				else if(r2==1){
//					result.m =  mxc2.m * mxc1.m;
//					result.c = 'M';
//				}
				else
					throw "Input Error!";
			}
			else{
				if(r1==1){
					result.m = mxc2.m * mxc1.m.trans();
					result.c = 'C';
				}
				else if(r1!=1||c1!=1){
					result.m = mxc2.m * mxc1.m;
					result.c = 'M';
				}
//				else if(r1==1&&c1==1){	//不能兩個常數運算
//					result.m = mxc2.m * mxc1.m;
//					result.c = 'C';
//				}
			}
			stack.push(result);
		}

		else if(inputStr[i]=='/'){
			if(stack.size()<2) throw "POwOq";
			MatXChar mxc1=stack.pop(),mxc2=stack.pop(),result;
			if(mxc1.c=='C'){
				result.m = mxc2.m / mxc1.m.getRowData(0).getData(0);
				result.c = mxc2.c;
			}
			else throw "Input Error!";
			stack.push(result);
		}
	}
	if(stack.size()<1) throw "POwOq";
	return stack.pop().m;
}

void MainWindow::on_pushButton_clicked()
{
	QString inputStr= ui->lineEdit->text(),s;
	QStringList args= inputStr.split(' ');
	QString inst = args[0].toLower();
	for(int i=1;i<args.size();++i)
		s+=args[i];
	s=s.toUpper();
	args = s.split(',');

	try{
		/////通用指令/////
		if(inst=="print"){
			args[0]=toPostfix(args[0]);
			ui->textBrowser->append(args[0]);
			ui->textBrowser->append(QString::fromStdString(calc(args[0]).toString()));
		}
		else if(inst=="info"){//顯示矩陣的資訊(行數列數)
			args[0] = toPostfix(args[0]);
			Mat m = calc(args[0]);
			ui->textBrowser->append(QString("%1 Row:%2 Col:%3 First data:%4").arg(args[0]).arg(m.getRow()).
					arg(m.getCol()).arg(m.getRowData(0).getData(0)));
		}
		else if(inst=="new"){//手動新增變數(格式如輸入檔案)(沒有輸入錯誤的判斷)
			args= inputStr.split(' ');//用空白隔開方便輸入
			if(args[1].toUpper()=="V"){
				Vec vv(args[2].toInt());
				for(int i=0;i<vv.getDim();i++)
					vv.setData(args[3+i].toDouble(),i);
				v.push_back(vv);
				ui->comboBox->addItem(QString("V%1").arg(QString("Z").repeated((v.size()-1)/25)+(97+((v.size()-1)%25))));
				ui->textBrowser->append(QString("new %1").arg(ui->comboBox->itemText(ui->comboBox->count()-1)));
			}
			else if(args[1].toUpper()=="M"){
				Mat mm(args[2].toInt(),args[3].toInt());
				int k=0;
				for(int r=0;r<mm.getRow();r++)
					for(int c=0;c<mm.getCol();c++)
						mm.setData(args[4+k++].toDouble(),r,c);
				m.push_back(mm);
				ui->comboBox_2->addItem(QString("M%1").arg(QString("Z").repeated((m.size()-1)/25)+(97+((m.size()-1)%25))));
				ui->textBrowser->append(QString("new %1").arg(ui->comboBox_2->itemText(ui->comboBox_2->count()-1)));
			}
		}
		else if(inst=="cls"){//清除輸出畫面
			ui->textBrowser->setText("");
		}
		/////向量指令/////
		else if(inst=="norm"){
			if(args.size()<1) throw "請輸入向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Vec v = calc(args[0]).getRowData(0);
			ui->textBrowser->append(QString("%1").arg(v.norm()));
		}
		else if(inst=="normal"){
			if(args.size()<1) throw "請輸入向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Vec v = calc(args[0]).getRowData(0);
			ui->textBrowser->append(QString::fromStdString(v.normal().toString()));
		}
		else if(inst=="cross"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString::fromStdString(v1.cross3(v2).toString()));
		}
		else if(inst=="com"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString("%1").arg(v1.comp(v2)));
		}
		else if(inst=="proj"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString::fromStdString(v1.proj(v2).toString()));
		}
		else if(inst=="area"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString("%1").arg(v1.Area(v2)));
		}
		else if(inst=="ispara"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(v1.isParal(v2)?"Yes":"No");
		}
		else if(inst=="isorth"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(v1.isOrtho(v2)?"Yes":"No");
		}
		else if(inst=="angle"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString("%1").arg(v1.angle_degree(v2)));
		}
		else if(inst=="pn"){
			if(args.size()<2) throw "請輸入兩個向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Vec v1 = calc(args[0]).getRowData(0);
			Vec v2 = calc(args[1]).getRowData(0);
			ui->textBrowser->append(QString::fromStdString(v1.planeNormal(v2).toString()));
		}
		else if(inst=="isli"){
			if(args.size()<1) throw "請輸入向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Vec vi = calc(args[0]).getRowData(0);
			Vec *va=new Vec[vi.getDim()];
			va[0]=vi;
			if(args.size()<vi.getDim()) throw "請輸入N個N維向量~";
			for(int i=1;i<vi.getDim();i++){
				args[i] = toPostfix(args[i]);
				va[i]=calc(args[i]).getRowData(0);
				if(va[i].getDim()!=vi.getDim()) throw "請輸入N個N維向量~";
			}
			Mat mm(va,vi.getDim(),vi.getDim());
			ui->textBrowser->append(mm.IsLI()?"Yes":"No");
		}
		else if(inst=="ob"){
			if(args.size()<1) throw "請輸入向量~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Vec vi = calc(args[0]).getRowData(0);
			Vec *va=new Vec[vi.getDim()];
			va[0]=vi;
			if(args.size()<vi.getDim()) throw "請輸入N個N維向量~";
			for(int i=1;i<vi.getDim();i++){
				args[i] = toPostfix(args[i]);
				va[i]=calc(args[i]).getRowData(0);
				if(va[i].getDim()!=vi.getDim()) throw "請輸入N個N維向量~";
			}
			Vec::ob(va);
			for(int i=0;i<vi.getDim();i++){
				ui->textBrowser->append(QString::fromStdString(va[i].toString()));
			}
		}
		/////矩陣指令/////
		else if(inst=="rank"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString("%1").arg(mm.Rank()));
		}
		else if(inst=="trans"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString::fromStdString(mm.trans().toString()));
		}
		else if(inst=="ssl"){
			if(args.size()<2) throw "請輸入兩個矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Mat m1 = calc(args[0]);
			Mat m2 = calc(args[1]);
			ui->textBrowser->append(QString::fromStdString(m1.SolveSquareLinearSys(m2).toString()));
		}
		else if(inst=="det"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString("%1").arg(mm.det()));
		}
		else if(inst=="inv"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString::fromStdString(mm.Inverse().toString()));
		}
		else if(inst=="adj"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString::fromStdString(mm.Adj().toString()));
		}
		else if(inst=="null"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			ui->textBrowser->append(QString::fromStdString(mm.nullspace().toString()));
		}
		else if(inst=="eigen"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			Vec ev;
			Mat rm;
			mm.eigen3(rm,ev);
			ui->textBrowser->append(QString::fromStdString(rm.toString()));
			ui->textBrowser->append(QString::fromStdString(ev.toString()));
		}
		else if(inst=="pm"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			Vec ev;
			ui->textBrowser->append(QString("%1").arg(mm.PowerMethod(ev)));
			ui->textBrowser->append(QString::fromStdString(ev.toString()));
		}
		else if(inst=="ls"){
			if(args.size()<2) throw "請輸入兩個矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			args[1] = toPostfix(args[1]);
			Mat m1 = calc(args[0]);
			Mat m2 = calc(args[1]);
			ui->textBrowser->append(QString::fromStdString(m1.LS(m2).toString()));
		}
		else if(inst=="rref"){
			if(args.size()<1) throw "請輸入矩陣~";
			ui->textBrowser->append(inputStr);
			args[0] = toPostfix(args[0]);
			Mat mm = calc(args[0]);
			Mat l;
			Mat u;
			int a;
			mm.LU(l,u,a);
			ui->textBrowser->append(QString::fromStdString(l.toString()));
			ui->textBrowser->append(QString::fromStdString(u.toString()));
		}
		/////其他/////
		else
			ui->textBrowser->append("No such instructions!");
	}
	catch(const char* e){
		ui->textBrowser->append(e);
	}
//	catch(...){
//		ui->textBrowser->append("Pardon?");
//	}
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

void MainWindow::on_pushButton_2_clicked()// '+'
{
	ui->lineEdit->insert("+");
}

void MainWindow::on_pushButton_3_clicked()// '-'
{
	ui->lineEdit->insert("-");
}

void MainWindow::on_pushButton_4_clicked()// '*'
{
	ui->lineEdit->insert("*");
}

void MainWindow::on_pushButton_5_clicked()// '/'
{
	ui->lineEdit->insert("/");
}

void MainWindow::on_pushButton_6_clicked()//print:用於只有+*-/的運算式或是直接輸出某個變數
{
	ui->lineEdit->setText("Print ");
}

void MainWindow::on_pushButton_7_clicked()//清除輸出
{
	ui->textBrowser->setText("");
}

void MainWindow::on_pushButton_8_clicked()//清除輸入
{
	ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_9_clicked()//未定義
{
	//test
	try{
//		Mat l,u;
//		int swapCount;
//		m[2].LU(l,u,swapCount);
//		ui->textBrowser->append(QString::fromStdString((l*u).toString())+"\n");
//		ui->textBrowser->append(QString::fromStdString(l.toString())+"\n");
//		ui->textBrowser->append(QString::fromStdString(u.toString())+"\n");

//		ui->textBrowser->append(QString::number(m[2].det())+"\n");
//		ui->textBrowser->append(QString::number(l.det())+"\n");
//		ui->textBrowser->append(QString::number(u.det())+"\n");

//		ui->textBrowser->append(QString::fromStdString(m[2].Inverse().toString())+"\n");

//		ui->textBrowser->append(QString::fromStdString((l*u-m[0]).toString())+"\n");

//		ui->textBrowser->append(QString::fromStdString((m[2]*m[2].Inverse()).toString())+"\n");
//		ui->textBrowser->append(QString::fromStdString((m[2].Inverse()*m[2]).toString())+"\n");
/////////////////////////////////////
//		ui->textBrowser->append(QString::number(m[0].Rank())+"\n");
//		ui->textBrowser->append(QString::number(m[1].Rank())+"\n");
//		ui->textBrowser->append(QString::number(m[2].Rank())+"\n");
/////////////////////////////////////
//		ui->textBrowser->append(QString::number(v[0].dot(v[1]))+"\n");
//		ui->textBrowser->append(QString::number(v[2].dot(v[3]))+"\n");
//		ui->textBrowser->append(QString::number(v[4].dot(v[5]))+"\n");
/////////////////////////////////////
		//ui->textBrowser->append(QString::number(v[6].isOrtho(v[7]))+"\n");
/////////////////////////////////////
//		ui->textBrowser->append(QString::fromStdString(m[0].SolveSquareLinearSys(m[1]).toString())+"\n");
//		ui->textBrowser->append(QString::fromStdString(m[2].SolveSquareLinearSys(m[3]).toString())+"\n");
//		ui->textBrowser->append(QString::fromStdString(m[4].SolveSquareLinearSys(m[5]).toString())+"\n");
/////////////////////////////////////
		Mat eiV;
		Vec vv;
		m[0].eigen3(eiV,vv);
		ui->textBrowser->append(QString::fromStdString(vv.toString())+"\n");
		ui->textBrowser->append(QString::fromStdString(eiV.toString())+"\n");
		m[1].eigen3(eiV,vv);
		ui->textBrowser->append(QString::fromStdString(vv.toString())+"\n");
		ui->textBrowser->append(QString::fromStdString(eiV.toString())+"\n");
/////////////////////////////////////
//		Vec x;
//		ui->textBrowser->append(QString::number(m[0].PowerMethod(x))+"\n");
//		ui->textBrowser->append(QString::fromStdString(x.toString())+"\n");
//		ui->textBrowser->append(QString::number(m[1].PowerMethod(x))+"\n");
//		ui->textBrowser->append(QString::fromStdString(x.toString())+"\n");
//		ui->textBrowser->append(QString::number(m[2].PowerMethod(x))+"\n");
//		ui->textBrowser->append(QString::fromStdString(x.toString())+"\n");
	}
	catch(const char* e){
		ui->textBrowser->append(e);
	}
}

void MainWindow::on_pushButton_10_clicked()//未定義
{
	v.clear();
	m.clear();
	ui->comboBox->clear();
	ui->comboBox->addItem("select vec");
	ui->comboBox_2->clear();
	ui->comboBox_2->addItem("select mat");
	ui->textBrowser->append("Clear data");
//	try{
//		ui->textBrowser->append("=3=");
//	}
//	catch(const char* e){
//		ui->textBrowser->append(e);
//	}
}

void MainWindow::on_actionSave_triggered()
{
	//把輸出存成檔案(不重要，有空再做)
}
