#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(600,800);
    this->setWindowTitle("Basic");
    console=new Console(this);
    console->setGeometry(0,0,600,800);
}

MainWindow::~MainWindow()
{
    delete ui;
}
