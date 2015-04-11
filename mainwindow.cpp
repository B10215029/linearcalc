#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Vec.h"
#include "Mat.h"
#include <QStack>
#include <QMap>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->comboBox->addItem("選擇向量");
	ui->comboBox_2->addItem("選擇矩陣");
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	try{
		Vec v,v2(1);
		v.setData(1,1);
		v=v+v2;
		ui->textBrowser->insertPlainText(QString::fromStdString(v.toString()));
	}
	catch(const char* e){
		ui->textBrowser->insertPlainText(e);
	}
/*
	QString inputStr = ui->lineEdit->text();
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
			while(operatorPriority.value(QString(operatorStack.top()))>=operatorPriority.value(QString(inputStr[i])))
				outputStr += operatorStack.pop();
			operatorStack.push(inputStr[i]);
		}
	}
	while(!operatorStack.empty())
		outputStr += operatorStack.pop();

	outputStr+='\n';
	ui->textBrowser->insertPlainText(outputStr);
	*/
}
