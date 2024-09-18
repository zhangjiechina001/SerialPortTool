# 1.这是一个基于Qt的串口调试工具
# 2.核心类是SerialPortWrap，将QSerialPort包装为SendReply和SendNoReply两个方法
```cpp
#ifndef SERIALPORTWRAP_H
#define SERIALPORTWRAP_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QSerialPort>

class SerialPortWrap : public QObject
{
    Q_OBJECT
public:
    static QMap<QString,QSerialPort::Parity> getParityMap();

    static QMap<QString,QSerialPort::DataBits> getDataMap();

    static QMap<QString,QSerialPort::StopBits> getStopMap();

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
```
# 3.设备运行界面
![图片](https://github.com/user-attachments/assets/53ff118d-43e8-4611-9039-84a262be016e)


