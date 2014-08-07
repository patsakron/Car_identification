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

    //Polamin >> Clear Old List and List count
    imagesList.clear();
    imagesCount = 0;


    //Initialization code here (& other tasks)
    dirname = QFileDialog::getExistingDirectory(this, tr("Select a Directory"),QDir::currentPath() );
    //Create a QDir that points out to your path in the resources folder
    QDir directory(dirname);
    //Load all files with the *.PNG extension
    imagesList = directory.entryList(QStringList("*.jpg"));
    //QStringList filters;
    //filters << "*.png" << "*.jpg" << "*.bmp";
    //fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);


    if(!imagesList.empty())
    {
        //Polamin >> If found image Enable next Button
        if(imagesList.size()>1)
            ui->btNext->setEnabled(true);

        QString ts = dirname + "/" + imagesList[0];
        QPixmap pix(ts);
        ui->label_pic->setPixmap(pix);
    }


    /*
    foreach(QString itm, imagesList)
    {
        QString ts = dirname + "/" + itm;
        QPixmap pix(ts);
        ui->label_pic->setPixmap(pix);
        QMessageBox::information(this,tr("File Name"), dirname+itm);
    }
    */
}

void MainWindow::on_btPrevious_clicked()
{
    //Enable Next Button
    ui->btNext->setEnabled(true);

    imagesCount--;
    showImage(imagesList[imagesCount]);

    //Check if first image
    if(imagesCount-1 == 0)
        ui->btPrevious->setEnabled(false);
}

void MainWindow::on_btNext_clicked()
{
    //Enable Previous Button
    ui->btPrevious->setEnabled(true);

    imagesCount++;
    showImage(imagesList[imagesCount]);

    //Check if last image
    if(imagesCount+1 == imagesList.size())
        ui->btNext->setEnabled(false);
}

void MainWindow::showImage(QString path)
{
    //This function just show Image
    QString ts = dirname + "/" + path;
    QPixmap pix(ts);
    ui->label_pic->setPixmap(pix);
}
