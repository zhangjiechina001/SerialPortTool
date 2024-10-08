﻿#ifndef SERIALPORTWRAP_H
#define SERIALPORTWRAP_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QSerialPort>

class SerialPortWrap : public QObject
{
    Q_OBJECT
public:
    static QStringList getParityList();

    static QMap<QString,QSerialPort::Parity> getParityMap();

    static QStringList getDataList();

    static QMap<QString,QSerialPort::DataBits> getDataMap();

    static QStringList getStopList();

    static QMap<QString,QSerialPort::StopBits> getStopMap();

    static QStringList getFlowList();

    static QMap<QString, QSerialPort::FlowControl> getFlowMap();

public:
    explicit SerialPortWrap(QObject *parent = nullptr);

    void setSerialPort(QSerialPort *port);

    bool sendNoReply(QByteArray data);

    bool sendReply(QByteArray data, QByteArray &rec);

signals:
    void receivedData(QByteArray data);

public slots:

private:
    QSerialPort *_serial=nullptr;
    QList<QByteArray> _recList;

    void waitMs(int ms);

private slots:
    void onError(QSerialPort::SerialPortError err);
    void onReadyReady();
};

#endif // SERIALPORTWRAP_H
