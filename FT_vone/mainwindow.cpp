#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setText("Serial Connection: \tDisconnected\nDatabase Connection: \tDisconnected");
}
MainWindow::~MainWindow()
{
    delete ui;
    serial.close();
    db.close();
}
void MainWindow:: delay(int sec)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::on_pushButton_clicked()
{    
    initialize_serialcom();
    //this is called when datas are received
    QObject::connect(&serial, &QSerialPort::readyRead, [&]
    {
        update_comstatus();
        datas = serial.readAll();
        //Split parts with "\n" and ignore empty strings
        str = datas.split(("\n"), QString::SkipEmptyParts);
        delay(1);
        //Print strings per line in console
        for (int strlength = 0; strlength < str.length(); strlength++) {
            std::cout << str.at(strlength).toStdString() << std::endl;
            if(str[strlength].contains("error:")){
                msgbox.critical(nullptr,"ERROR!",str[strlength].remove("error: "));
            }

            else
            {}
        }
        //Read Vone Values when Extract Button is clicked (in Database Page)
        if (extractButton_clicked){
            str.filter(datas);
            for (int i=0; i<str.length();i++){
                get_calibration(i);
            }
        }
        if(probepinsButton_clicked)
        {
            str.filter(datas);
            for (int i=0; i<str.length();i++){
                check_probepins(i);
            }
        }
        else{}
    }
    );
    datas.clear();
    //this is called when serial com error occurs
    QObject::connect(&serial,static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>
                     (&QSerialPort::error),
                     [&](QSerialPort::SerialPortError error)
    {
        qDebug() << "An error occured: " << error;
        serialcom=false;
        update_comstatus();
    });

}
void MainWindow::on_BSaveButton_clicked()
{
   QString bridgeline= ui->BridgeSN->text();
   int i=0;
   int errorID[5];

   int xlim = ui->xlim_checkBox->checkState();
   int probepins = ui->probepins_checkBox->checkState();
   int checkbox = ui->spine_checkBox->checkState();
   int zaxis = ui->zaxis_checkBox->checkState();
   int wharness = ui->wharness_checkBox->checkState();
   int othersB = ui->othersB_checkBox->checkState();

int status[5]= {xlim,probepins,checkbox,zaxis,wharness};

for(int x=0; x<5;x++){
    if(status[x] == 1){
        errorID[i]= x+1000;
        qDebug()<<errorID[i]<<endl;
        i++;

    }
    else{}
}
}
