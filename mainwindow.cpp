#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QPixmap>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_toolButton_clicked()
{
//QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
//QPixmap pix(fileName);
//ui->label_pic->setPixmap(pix);
//QMessageBox::information(this,tr("File Name"), fileName);

       //Initialization code here (& other tasks)
  QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a Directory"),QDir::currentPath() );
       // Create a QDir that points out to your path in the resources folder
  QDir directory(dirname);
       // Load all files with the *.PNG extension
  QStringList imagesList = directory.entryList(QStringList("*.jpg"));
//    QStringList filters;
//     filters << "*.png" << "*.jpg" << "*.bmp";
//     fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
  foreach(QString itm, imagesList){
    QString ts = dirname + "/" + itm;
    QPixmap pix(ts);
    ui->label_pic->setPixmap(pix);
  QMessageBox::information(this,tr("File Name"), dirname+itm);
  }
}
