#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QSerialPort>
#include "serialportwrap.h"
#include "configvar.h"

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
    void on_btnUpdataSerialPort_clicked();

    void on_openSerial_clicked();

    void on_closeSerial_clicked();

    void onLog(QString log);

    void onReceivedData(QByteArray arr);

    void on_sendDataBtn_clicked();

    void on_sendDataBtn_2_clicked();

    void on_actionAbout_triggered();

    void on_btnClear_clicked();

    void on_checkBox_clicked(bool checked);

    QByteArray sendMsg();

    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

private:
    Ui::MainWindow *ui;
    QSerialPort *_serialPort=nullptr;
    SerialPortWrap *_wrap=nullptr;
    QTimer *_timer=nullptr;

    void updateSerialPort();
    void loadConfig();
    void saveConfig();

    void loadSendConfig();
    void saveSendConfig();

    QString getFormatString(QByteArray arr);
    QStringList getAvailablePortsString();

    void undateLanguage();
    void switchLanguage(QString path);
};

#endif // MAINWINDOW_H
