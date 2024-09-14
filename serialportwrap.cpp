#include <QMap>
#include <QDebug>
#include <QEventLoop>
#include <QElapsedTimer>
#include <QTimer>
#include "serialportwrap.h"

QStringList SerialPortWrap::getParityList()
{
    return QStringList{
        "NoPraity",
        "EvenParity",
        "OddParity",
        "SpaceParity",
        "MarkParity"
    };
}

QMap<QString, QSerialPort::Parity> SerialPortWrap::getParityMap()
{
    QMap<QString,QSerialPort::Parity> parityMap={
        {"NoPraity",QSerialPort::NoParity},
        {"EvenParity",QSerialPort::EvenParity},
        {"OddParity",QSerialPort::OddParity},
        {"SpaceParity",QSerialPort::SpaceParity},
        {"MarkParity",QSerialPort::MarkParity}
    };
    return parityMap;
}

QStringList SerialPortWrap::getDataList()
{
    return QStringList{
        "Data5",
        "Data6",
        "Data7",
        "Data8"
    };
}

QMap<QString, QSerialPort::DataBits> SerialPortWrap::getDataMap()
{
    QMap<QString,QSerialPort::DataBits> dataMap={
        {"Data5",QSerialPort::Data5},
        {"Data6",QSerialPort::Data6},
        {"Data7",QSerialPort::Data7},
        {"Data8",QSerialPort::Data8}
    };
    return dataMap;
}

QStringList SerialPortWrap::getStopList()
{
    return QStringList{
        "OneStop",
        "OneAndHalfStop",
        "TwoStop"
    };
}

QMap<QString, QSerialPort::StopBits> SerialPortWrap::getStopMap()
{
    QMap<QString,QSerialPort::StopBits> stopMap={
        {"OneStop",QSerialPort::OneStop},
        {"OneAndHalfStop",QSerialPort::OneAndHalfStop},
        {"TwoStop",QSerialPort::TwoStop}
    };
    return stopMap;
}

QStringList SerialPortWrap::getFlowList()
{
    return QStringList{
        "NoFlowControl",
        "HardwareControl",
        "SoftwareControl"
    };
}

QMap<QString, QSerialPort::FlowControl> SerialPortWrap::getFlowMap()
{
    QMap<QString,QSerialPort::FlowControl> flowMap={
        {"NoFlowControl",QSerialPort::NoFlowControl},
        {"HardwareControl",QSerialPort::HardwareControl},
        {"SoftwareControl",QSerialPort::SoftwareControl}
    };
    return flowMap;
}

SerialPortWrap::SerialPortWrap(QObject *parent) : QObject(parent)
{

}

void SerialPortWrap::setSerialPort(QSerialPort *port)
{
    _serial=port;
    connect(_serial,&QSerialPort::errorOccurred,this,&SerialPortWrap::onError);
    connect(_serial,&QSerialPort::readyRead,this,&SerialPortWrap::onReadyReady);
}

bool SerialPortWrap::sendNoReply(QByteArray data)
{
    _recList.clear();
    return  _serial->write(data)!=-1;
}

bool SerialPortWrap::sendReply(QByteArray data,QByteArray &rec)
{
    _recList.clear();
    _serial->write(data);

    QElapsedTimer ela;
    ela.start();
    while(_recList.count()==0)
    {
        waitMs(50);
        if(ela.elapsed()>3000)
        {
            throw QString("%1 time out").arg(_serial->portName());
        }
    }
    waitMs(50);
    QByteArray arr;
    for(int i=0;i<_recList.count();i++)
    {
        arr.append(_recList.at(i));
    }
    rec=arr;
    return true;
}

void SerialPortWrap::waitMs(int ms)
{
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);  // 设置定时器，在超时时退出事件循环
    loop.exec();  // 进入事件循环
}

void SerialPortWrap::onError(QSerialPort::SerialPortError err)
{
    qDebug()<<__FUNCTION__<<__LINE__<<_serial->portName()<<_serial->errorString();
}

void SerialPortWrap::onReadyReady()
{
    QByteArray arr=_serial->readAll();
    _recList.append(arr);
    emit receivedData(arr);
}

