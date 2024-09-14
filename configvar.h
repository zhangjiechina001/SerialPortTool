#ifndef CONFIGVAR_H
#define CONFIGVAR_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QSettings>

struct SerialPortParam
{
    SerialPortParam()
    {
        COMName="COM1";
        BaudRate="4800";
        DataBits="Data5";
        Parity="NoPraity";
        StopBits="OneStop";
        FlowControl="NoFlowControl";
    }

    QString COMName;
    QString BaudRate;
    QString DataBits;
    QString Parity;
    QString StopBits;
    QString FlowControl;
};

struct SendParam
{
    SendParam() {}

    QString SendMsg;
};

class ConfigVar
{
private:
    ConfigVar();
    ConfigVar(const ConfigVar&)=delete;
    ConfigVar& operator=(const ConfigVar&)=delete;

public:
    static ConfigVar& instance();

    SerialPortParam getSerialPortParam();
    void setSerialPortParam(SerialPortParam param);

    SendParam getSendParam();
    void setSendParam(SendParam param);

private:
    QSettings* Setting;
};

#endif // CONFIGVAR_H
