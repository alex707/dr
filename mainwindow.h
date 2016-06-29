#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMap>

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
    void on_pushButton_ReadData_clicked();
    void on_pushButton_ClearData_clicked();
    void on_pushButton_ToPCMode_clicked();
    void on_pushButton_FromPCMode_clicked();
    void on_pushButton_GetLastValue_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *dp;

    QStringList measureList;
    QStringList resultList;
    QMap<QString, double> tableOfMeasures;
    QByteArray parseToByteArray(unsigned char *, int);
    QByteArray writeToChannel(unsigned char *, int, int);
};

#endif // MAINWINDOW_H
