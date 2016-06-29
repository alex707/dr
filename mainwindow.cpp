#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    qDebug() << "START MainWindow::MainWindow";
    ui->setupUi(this);

    tableOfMeasures.insert("00000000", 0.);
    tableOfMeasures.insert("6edf233c", 0.01);
    tableOfMeasures.insert("6edfa33c", 0.02);
    tableOfMeasures.insert("25cff53c", 0.03);
    tableOfMeasures.insert("6edf233d", 0.04);
    tableOfMeasures.insert("49d74c3d", 0.05);
    tableOfMeasures.insert("25cf753d", 0.06);
    tableOfMeasures.insert("80638f3d", 0.07);
    tableOfMeasures.insert("6edfa33d", 0.08);
    tableOfMeasures.insert("5c5bb83d", 0.09);
    tableOfMeasures.insert("49d7cc3d", 0.1);
    tableOfMeasures.insert("3753e13d", 0.11);
    tableOfMeasures.insert("25cf753d", 0.12);
    tableOfMeasures.insert("8925053e", 0.13);
    tableOfMeasures.insert("80630f3e", 0.14);
    tableOfMeasures.insert("77a1193e", 0.15);
    tableOfMeasures.insert("6edf233e", 0.16);
    tableOfMeasures.insert("651d2e3e", 0.17);
    tableOfMeasures.insert("5c5b383e", 0.18);
    tableOfMeasures.insert("5299423e", 0.19);
    tableOfMeasures.insert("49d74c3e", 0.2);
    tableOfMeasures.insert("4015573e", 0.21);
    tableOfMeasures.insert("3753613e", 0.22);
    tableOfMeasures.insert("2e916b3e", 0.23);
    tableOfMeasures.insert("25cf753e", 0.24);
    tableOfMeasures.insert("8e06803e", 0.25);
    tableOfMeasures.insert("8925853e", 0.26);
    tableOfMeasures.insert("85448a3e", 0.27);
    tableOfMeasures.insert("80638f3e", 0.28);
    tableOfMeasures.insert("7b82943e", 0.29);
    tableOfMeasures.insert("77A1993e", 0.30);
    tableOfMeasures.insert("72c09e3e", 0.31);
    tableOfMeasures.insert("6edfa33e", 0.32);
    tableOfMeasures.insert("69fea83e", 0.33);
    tableOfMeasures.insert("651dae3e", 0.34);
    tableOfMeasures.insert("603cb33e", 0.35);
    tableOfMeasures.insert("5c5bb83e", 0.36);
    tableOfMeasures.insert("577abd3e", 0.37);
    tableOfMeasures.insert("5299c23e", 0.38);
    tableOfMeasures.insert("4eb8c73e", 0.39);
    tableOfMeasures.insert("49d7cc3e", 0.4);
    tableOfMeasures.insert("45f6d13e", 0.41);
    tableOfMeasures.insert("4015d73e", 0.42);
    tableOfMeasures.insert("3c34dc3e", 0.43);
    tableOfMeasures.insert("3753e13e", 0.44);
    tableOfMeasures.insert("3272e63e", 0.45);
    tableOfMeasures.insert("2e91eb3e", 0.46);
    tableOfMeasures.insert("29b0f03e", 0.47);
    tableOfMeasures.insert("25cff53e", 0.48);
    tableOfMeasures.insert("20eefa3e", 0.49);
    tableOfMeasures.insert("8e06003f", 0.5);
    tableOfMeasures.insert("0b96023f", 0.51);
    tableOfMeasures.insert("8925053f", 0.52);

    dp = new QSerialPort;

    dp->setPortName("COM5");
    //Скорость передачи 56000
    quint32 baud = 56000;
    dp->setBaudRate(baud);
    //RTS включен
    qDebug() << "setRequestToSend: " << dp->setRequestToSend(true);
    //DTR включен
    qDebug() << "setDataTerminalReady: " << dp->setDataTerminalReady(true);

    //Биты данных=8
    qDebug() << "setDataBits: " << dp->setDataBits(QSerialPort::Data8);
    //Стоповые биты=1
    qDebug() << "setStopBits: " << dp->setStopBits(QSerialPort::OneStop);
    //Четность=None
    qDebug() << "setParity: " << dp->setParity(QSerialPort::NoParity);

    //Служ. символы: Eof=0x00, Error=0x00, Break=0x00, Event=0x00, Xon=0x11, Xoff=0x13
    //Контроль передачи: ControlHandShake=(DTR_CONTROL), Замена=(TRANSMIT_TOGGLE, RTS_CONTROL, XOFF_CONTINUE), Лимит Xon=2048, Лимит Xoff=512
    //Таймауты: ReadInterval=100, ReadTotalTimeoutMultiplier=10, ReadTotalTimeoutConstant=10, WriteTotalTimeoutMultiplier=10, WriteTotalTimeoutConstant=10

    if(!dp->open(QIODevice::ReadWrite)){
        qDebug() << "Failed to open port COM5, error: "  << dp->error() << endl;
    }

    qDebug() << "END MainWindow::MainWindow";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dp;
}

QByteArray MainWindow::parseToByteArray(unsigned char *dataToByteArray, int length)
{
    QByteArray byteArray;
//    qDebug() << "_dataToByteArray: " << dataToByteArray;
//    qDebug() << "_sizeof(dataToByteArray): " << sizeof(dataToByteArray);
//    qDebug() << "_byteArray: " << byteArray.toHex();
    for (int i = 0; i < length; i++)
        byteArray.append(dataToByteArray[i]);

//    qDebug() << "sizeof(dataToByteArray): " << sizeof(dataToByteArray);
//    qDebug() << "byteArray: " << byteArray.toHex();
    return byteArray;
}

QByteArray MainWindow::writeToChannel(unsigned char *sourceRequest, int lengthOfRequest, int lengthOfRespond)
{
    QByteArray request, respond;
    qint64 length;
    for (int i = 0; i < lengthOfRequest; i++)
        request.append(sourceRequest[i]);

    length = dp->write(request, lengthOfRequest);

    while((respond.length() != lengthOfRespond) && dp->waitForReadyRead(500))
    {
        respond.append(dp->read(lengthOfRespond).toHex());
        qDebug() << "  res    :" << respond;
    }

    qDebug() << "  respond:" << respond;

    dp->clear(QSerialPort::Input);
    return respond;
}

void MainWindow::on_pushButton_ReadData_clicked()
{
    qDebug() << "START void MainWindow::on_pushButton_ReadData_clicked()";

    if (resultList.size() >= 5)
    {
        qDebug() << "Need to clear memory";
        return;
    }

    qint64 length;
    QByteArray request, respond;
    dp->clear(QSerialPort::Input);
    //----------------------------
    //11
    unsigned char request_1[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD8, 0x3F};
    //int length = 40;

    for (int i = 0; i < sizeof(request_1); i++)
        request.append(request_1[i]);
    qDebug() << request.length();

    length = dp->write(request, sizeof(request_1));
    //written: 11
    qDebug() << sizeof(request_1) << "written:" << length;

    //read: 40
    while((respond.length() != 40) && dp->waitForReadyRead(500))
    {
        //qDebug() << "01respond_length:" << dp->readAll().length();
        //считалось 40 int. в qbytearray::toHex() это 80 знаков данных.

        respond.append(dp->read(40).toHex());
        qDebug() << "01res    :" << respond;

        //respond.append(dp->readAll().toHex());
    }

    //respond.append(dp->read(40).toHex()).length = 80
    qDebug() << "01respond:" << respond << "\n";

    request.clear();
    respond.clear();
    dp->clear(QSerialPort::Input);
    //----------------------------

//    unsigned char request_1[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD8, 0x3F};
//    int outLength = 40;
//    writeToChannel (request_2, sizeof(request_2), outLength);

    unsigned char request_2[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xBB, 0xC6};
    int outLength = 52;
    writeToChannel (request_2, sizeof(request_2), outLength);

    unsigned char request_3[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0xE3, 0xB7};
    outLength = 37;
    writeToChannel (request_3, sizeof(request_3), outLength);

    unsigned char request_4[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0xFD, 0x89};
    outLength = 37;
    writeToChannel (request_4, sizeof(request_4), outLength);

    unsigned char request_5[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x3F, 0xB3};
    outLength = 40;
    writeToChannel (request_5, sizeof(request_5), outLength);

    unsigned char request_6[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x9E, 0x70};
    outLength = 37;
    writeToChannel (request_6, sizeof(request_6), outLength);

    unsigned char request_7[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x80, 0x4E};
    outLength = 27;
    writeToChannel (request_7, sizeof(request_7), outLength);

    unsigned char request_8[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x38, 0x47};
    outLength = 24;
    writeToChannel (request_8, sizeof(request_8), outLength);

    unsigned char request_9[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xF9, 0x68};
    outLength = 27;
    writeToChannel (request_9, sizeof(request_9), outLength);

    unsigned char request_10[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xD4, 0x35};
    outLength = 73;
    writeToChannel (request_10, sizeof(request_10), outLength);

    unsigned char request_11[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x00, 0xE0, 0xA2};
    outLength = 115;
    writeToChannel (request_11, sizeof(request_11), outLength);

    unsigned char request_12[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x96, 0x9B};
    outLength = 50;
    writeToChannel (request_12, sizeof(request_12), outLength);

    unsigned char request_13[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x0B, 0x24};
    outLength = 701;
    writeToChannel (request_13, sizeof(request_13), outLength);

    unsigned char request_14[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x31, 0x00, 0x01, 0x00, 0x01, 0x7E, 0xC5};
    outLength = 23;
    writeToChannel (request_14, sizeof(request_14), outLength);

    //------------------
    unsigned char request_15[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x15, 0x0B};
    outLength = 309;
    QByteArray re = writeToChannel (request_15, sizeof(request_15), outLength);

    //обработка пакета
    measureList.clear();
    resultList.clear();
    for (int i = 0; i < 10; i++)
    {
        QByteArray tmp;
        bool ok;
        tmp.append(re.mid(48 + i * 52, 52));

        qDebug() << "cur number_H: " << tmp.mid(0,4);
        qDebug() << "cur number: " << tmp.mid(0,4).toInt(&ok, 16);
        if (tmp.mid(0, 4).toInt(&ok, 16) != 0)
        {
            qDebug() << "alco: " << tmp.mid(20, 8);
            qDebug() << "res: " << tmp.mid(40, 2);
            measureList.append(tmp.mid(20, 8));
            resultList.append(tmp.mid(40, 2));
        }
        else
        {
            break;
        }
    }
    qDebug() << "data: " << measureList;
    qDebug() << "res_data:" << resultList;

    re.clear();
    //------------------

    //запросы для завершения
    unsigned char request_end_1[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x01, 0x05, 0xC6};
    dp->write(parseToByteArray(request_end_1, sizeof(request_end_1)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    unsigned char request_end_2[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x00, 0x8C, 0xD7};
    dp->write(parseToByteArray(request_end_2, sizeof(request_end_2)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    qDebug() << "END void MainWindow::on_pushButton_ReadData_clicked()";
}

void MainWindow::on_pushButton_ClearData_clicked()
{
    qDebug() << "START void MainWindow::on_pushButton_ClearData_clicked()";

    QByteArray respond;
    dp->clear(QSerialPort::Input);
    measureList.clear();
    resultList.clear();

    unsigned char request_1[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD8, 0x3F};
    dp->write(parseToByteArray(request_1, sizeof(request_1)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    unsigned char request_2[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x01, 0x05, 0xC6};
    dp->write(parseToByteArray(request_2, sizeof(request_2)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    unsigned char request_3[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0xD0, 0xA1};
    dp->write(parseToByteArray(request_3, sizeof(request_3)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    unsigned char request_4[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xCD, 0xFF};
    for (int i = 0; i < 120; i++) {
        dp->write(parseToByteArray(request_4, sizeof(request_4)));
        while(dp->waitForReadyRead(500))
          respond.append(dp->readLine().toHex());

        respond.clear();
        dp->clear(QSerialPort::Input);
    }

    unsigned char request_5[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x00, 0x8C, 0xD7};
    dp->write(parseToByteArray(request_5, sizeof(request_5)));
    while(dp->waitForReadyRead(500))
        respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    qDebug() << "END void MainWindow::on_pushButton_ClearData_clicked()";
}

void MainWindow::on_pushButton_ToPCMode_clicked()
{
    qDebug() << "START void MainWindow::on_pushButton_ToPCMode_clicked()";
    QByteArray respond;

    unsigned char writeData[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x01, 0x05, 0xC6};
    dp->write(parseToByteArray(writeData, sizeof(writeData)));

    while(dp->waitForReadyRead(500))
      respond.append(dp->readLine().toHex());
    dp->clear(QSerialPort::Input);
    qDebug() << "END void MainWindow::on_pushButton_ToPCMode_clicked()";
}

void MainWindow::on_pushButton_FromPCMode_clicked()
{
    qDebug() << "START void MainWindow::on_pushButton_FromPCMode_clicked()";
    QByteArray respond;

    unsigned char writeData_1[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x01, 0x00, 0x00, 0x8C, 0xD7};
    dp->write(parseToByteArray(writeData_1, sizeof(writeData_1)));
    while(dp->waitForReadyRead(500))
      respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    unsigned char writeData_2[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0xA2, 0x0C};
    dp->write(parseToByteArray(writeData_2, sizeof(writeData_2)));
    while(dp->waitForReadyRead(500))
      respond.append(dp->readLine().toHex());
    respond.clear();
    dp->clear(QSerialPort::Input);

    qDebug() << "END void MainWindow::on_pushButton_FromPCMode_clicked()";
}

void MainWindow::on_pushButton_GetLastValue_clicked()
{
    qDebug() << "START void MainWindow::on_pushButton_GetLastValue_clicked";
    qDebug() << "resultList.size(): " << resultList.size();
    if (resultList.size() == 0) {
        ui->textEdit_LastValue->setText("At first make a measure!");
    } else {
        qDebug() << "resultList.last(): " << resultList.last();
        qDebug() << "measureList.last(): " << measureList.last();
        if (resultList.last() != "00") {
            QString str;
            str.append("Error: ");
            str.append(resultList.last());
            ui->textEdit_LastValue->setText(str);
        } else {
            QMap<QString, double>::const_iterator i = tableOfMeasures.find(measureList.last());
            if (i != tableOfMeasures.end()) {
                qDebug() << "i.value(): " << i.value();
                ui->textEdit_LastValue->setText(QString::number(i.value()));
            }
            else
                ui->textEdit_LastValue->setText(measureList.last());
        }
    }

    qDebug() << "END void MainWindow::on_pushButton_GetLastValue_clicked";
}
