#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_toolButton_clicked();

    void on_btPrevious_clicked();

    void on_btNext_clicked();

private:
    Ui::MainWindow *ui;
    QStringList     imagesList;
    unsigned int    imagesCount;
    QString         dirname;

    void showImage(QString);

};

#endif // MAINWINDOW_H
