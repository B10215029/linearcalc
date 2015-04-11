#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Vec.h"
#include "Mat.h"
#include <QStack>
#include <QMap>
#include <QFileDialog>
#include <QTextStream>

QString toPostfix(QString&inputStr);

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

void MainWindow::on_pushButton_clicked()
{
	QString inputStr = ui->lineEdit->text();
	QString arg0=inputStr.split(' ')[0].toLower();
	if(arg0=="print")
		ui->textBrowser->insertPlainText(toPostfix(inputStr.split(' ')[1])+'\n');
	else if(arg0=="cls")
		ui->textBrowser->setText(QString());
	else if(arg0=="add")
		ui->textBrowser->insertPlainText(inputStr+'\n');


//	try{
//		Vec v,v2(1);
//		v.setData(1,1);
//		v=v+v2;
//		ui->textBrowser->insertPlainText(QString::fromStdString(v.toString()));
//	}
//	catch(const char* e){
//		ui->textBrowser->insertPlainText(e);
//	}
}

QString toPostfix(QString &inputStr){
	QString outputStr;
	QStack<QChar> operatorStack;
	QMap<QString,int> operatorPriority;
	operatorPriority["("] = 1;
	operatorPriority[")"] = 1;
	operatorPriority["+"] = 2;
	operatorPriority["-"] = 2;
	operatorPriority["*"] = 3;
	operatorPriority["/"] = 3;

	for(int i=0;i<inputStr.length();++i){
		if(inputStr[i] == 32)
			continue;
		else if(inputStr[i]>=48&&inputStr[i]<=57)
			outputStr += inputStr[i];
		else if(inputStr[i]>=65&&inputStr[i]<=90)
			outputStr += inputStr[i];
		else if(inputStr[i]>=97&&inputStr[i]<=122)
			outputStr += inputStr[i].toUpper();

		else if(inputStr[i] == '(')
			operatorStack.push(inputStr[i]);
		else if(inputStr[i] == ')'){
			while(operatorStack.top()!='(')
				outputStr += operatorStack.pop();
			operatorStack.pop();
		}
		else if(QString("+-/ *").indexOf(inputStr[i])!=-1){
			while(!operatorStack.isEmpty()&&operatorPriority.value(QString(operatorStack.top()))>=operatorPriority.value(QString(inputStr[i]))){
				outputStr += operatorStack.pop();
			}
			operatorStack.push(inputStr[i]);
		}
	}

	while(!operatorStack.empty())
		outputStr += operatorStack.pop();
	return outputStr;
}

void MainWindow::on_actionOpen_triggered()
{
	//Open Dialog and Return file Directory
	QString fname=QFileDialog::getOpenFileName(this,"Open File" , "../" , "Text(*.txt);;All(*)");

	QFile f(fname);

	//Open File and test whether it is error
	if(!f.open(QIODevice::ReadOnly)) ui->textBrowser->append(fname+"[Open File Error]");

	//to stream like c++ cin
	QTextStream in(&f);
	in.setCodec("UTF-8");

	//first line is always int
	int num;
	in>>num;

	//Read line and Set v, m
	QString line;
	for(int i=num;i>0;i--){
		in>>line;

		if(line=="V"){
			in>>num;
			Vec vv(num);
			for(int j=0;j<num;j++){
				double d;
				in>>d;
				vv.setData(d,j);
			}
			this->v.push_back(vv);
		}
		else if(line=="M"){
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
			this->m.push_back(mm);
		}
	}
	if(fname!=NULL){
		ui->textBrowser->append(fname+"\n"+QString::fromStdString(this->v[0].toString()));
	}
}

void MainWindow::on_lineEdit_returnPressed()
{
	this->on_pushButton_clicked();
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
	if(ui->comboBox->currentIndex())
		ui->lineEdit->setText(ui->lineEdit->text()+arg1);
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
	if(ui->comboBox_2->currentIndex())
		ui->lineEdit->setText(ui->lineEdit->text()+arg1);
}

void MainWindow::on_comboBox_3_activated(const QString &arg1)
{
	if(ui->comboBox_3->currentIndex())
		ui->lineEdit->setText(ui->lineEdit->text()+arg1);
}

void MainWindow::on_pushButton_2_clicked()
{
	ui->lineEdit->insert("+");
}

void MainWindow::on_pushButton_3_clicked()
{
	ui->lineEdit->insert("-");
}

void MainWindow::on_pushButton_4_clicked()
{
	ui->lineEdit->insert("*");
}

void MainWindow::on_pushButton_5_clicked()
{
	ui->lineEdit->insert("/");
}

void MainWindow::on_pushButton_6_clicked()
{
	ui->lineEdit->setText("Print ");
}

void MainWindow::on_pushButton_7_clicked()
{
	ui->textBrowser->setText("");
}

void MainWindow::on_pushButton_8_clicked()
{

}

void MainWindow::on_pushButton_9_clicked()
{

}
