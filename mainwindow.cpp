#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Vec.h"
#include "Mat.h"

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
	try{
		Vec v,v2(1);
		v.setData(1,1);
		v=v+v2;
		ui->textBrowser->insertPlainText(QString::fromStdString(v.toString()));
	}
	catch(const char* e){
		ui->textBrowser->insertPlainText(e);
	}

}
