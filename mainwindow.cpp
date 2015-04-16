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
//		if(inputStr[i] == 32)
//			continue;
		if(inputStr[i]>=48&&inputStr[i]<=57)//0~9
			outputStr += inputStr[i];
		else if(inputStr[i]>=65&&inputStr[i]<=90)//A~Z
			outputStr += inputStr[i];
		else if(inputStr[i]>=97&&inputStr[i]<=122)//a~z
			outputStr += inputStr[i].toUpper();

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

			operatorStack.push(inputStr[i]);
		}
	}

	while(!operatorStack.empty())
		outputStr += operatorStack.pop();
	return outputStr;
}
//只計算+*-/等operator
//不管是向量還是矩陣都用矩陣回傳
Mat MainWindow::calc(QString &inputStr){
	QStack<Mat> stack;//Mat不管向量還是矩陣都能裝
	for(int i=0;i<inputStr.length();++i){
		if(inputStr[i]=='V')
			//這樣的寫法只能讀10個值
			//toLatin1可以把QChar轉成char
			//Latin-1編碼前127字類似Ascii
			//我找不到toAscii
			stack.push(Mat(v[inputStr[++i].toLatin1()-49]));

		else if(inputStr[i]=='M')
			stack.push(m[inputStr[++i].toLatin1()-49]);
		//這個也只能判斷0~9並且常數只能在要乘的東西後面
		else if(inputStr[i].isDigit())
			stack.push(Mat::identity(stack.top().getCol())*(inputStr[i].toLatin1()-48));
		else if(inputStr[i]=='+')
			stack.push(stack.pop()+stack.pop());
		else if(inputStr[i]=='-')
			stack.push(stack.pop()-stack.pop());
		else if(inputStr[i]=='*'){
			Mat m=stack.pop();
			stack.push(stack.pop()*m);
		}
	}
	return stack.pop();
}

void MainWindow::on_pushButton_clicked()
{
	QString inputStr= ui->lineEdit->text(), arg0, arg1;
	QStringList args= inputStr.split(' ');

	arg0= args[0].toLower();
	for(int i=0;i<args.size();i++)
		arg1+=args[i];

	if(arg0=="print"){
		QString s=toPostfix(arg1);
		ui->textBrowser->append(s+'\n');
		try{
			ui->textBrowser->append(QString::fromStdString(calc(s).toString()));
		}
		catch(const char* e){
			ui->textBrowser->append(e);
		}

		//之後判斷是何種func
//		for(int i=0;i<s.size();i++){
//			if(s[i]=="V"){
//				int x=1;
//				QString n;
//				while(s[i+x]>=48&&s[i+x]<=57){

//					n+=s[i+x];
//					x++;
//				}
//				bool ok;
//				int nn=s.toInt(&ok);
//				if(!ok){
//					ui->textBrowser->append("fail to int");
//					return;
//				}
//				nn--;

//			}
//			if(s[i]=="+"){

//			}
//			else if(s[i]=="-"){

//			}
//			else if(s[i]=="*"){

//			}
//			else if(s[i]=="/"){

//			}
//		}
		//
	}
	else if(arg0=="cls")
		ui->textBrowser->setText("");
	else if(arg0=="指令A")
		ui->textBrowser->append(inputStr+'\n');
	else if(arg0=="指令B")
		ui->textBrowser->append(inputStr+'\n');

}

void MainWindow::on_actionOpen_triggered()//Qt讀檔方式
{
	//Open Dialog and Return file Directory
	QString fname=QFileDialog::getOpenFileName(this,"Open File" , "../" , "Text(*.txt);;All(*)");

	QFile f(fname);

	//Open File and test whether it is error
	if(!f.open(QIODevice::ReadOnly)) ui->textBrowser->append(fname+"[Open File Error]");

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
			ui->comboBox->addItem(QString("V%1").arg(v.size()));
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
			ui->comboBox_2->addItem(QString("M%1").arg(m.size()));
		}
	}
	//Output to TextBrowser
	if(fname!=NULL) ui->textBrowser->append(fname+"\n");
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
	ui->textBrowser->append(QString::number(m[2].det()));
}

void MainWindow::on_pushButton_10_clicked()//未定義
{
	//我只是拿來測試一下東西
//	QString ostr;
//	for(int i=0;i<v.size();i++)
//		ostr+=QString::fromStdString(v[i].toString())+"\n";
//	for(int i=0;i<m.size();i++)
//		ostr+=QString::fromStdString(m[i].toString())+"\n";
//	ui->textBrowser->append(ostr);
	Mat a(10,2);
	Mat b=a.trans();
	b.setI();
	b.setData(b.getRow(),0,0);
	Vec va(10);
	va.setI();
	b=b*Mat(va).trans();
	//ui->textBrowser->append(QString::fromStdString(a.toString()));
	ui->textBrowser->append(QString::fromStdString(b.toString()));
}
