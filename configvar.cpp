#include "configvar.h"

ConfigVar::ConfigVar()
{
    QString initFile=QString("%1").arg("app_config.ini");
    Setting=new QSettings(initFile,QSettings::IniFormat);
    Setting->setIniCodec("utf-8");
}

ConfigVar &ConfigVar::instance()
{
    static ConfigVar _instance;
    return _instance;
}

SerialPortParam ConfigVar::getSerialPortParam()
{
    QSettings *set=Setting;
    SerialPortParam param;
    if(!set->contains(QString("SerialPortParam/COMName")))
    {
        SerialPortParam setParam;
        setSerialPortParam(setParam);
    }
    set->beginGroup("SerialPortParam");
    param.COMName=set->value("COMName").toString();
    param.BaudRate=set->value("BaudRate").toString();
    param.DataBits=set->value("DataBits").toString();
    param.Parity=set->value("Parity").toString();
    param.StopBits=set->value("StopBits").toString();
    param.FlowControl=set->value("FlowControl").toString();
    set->endGroup();
    return param;
}

void ConfigVar::setSerialPortParam(SerialPortParam param)
{
    QSettings *set=Setting;
    set->beginGroup("SerialPortParam");
    set->setValue("COMName",param.COMName);
    set->setValue("BaudRate",param.BaudRate);
    set->setValue("DataBits",param.DataBits);
    set->setValue("Parity",param.Parity);
    set->setValue("StopBits",param.StopBits);
    set->setValue("FlowControl",param.FlowControl);
    set->endGroup();
    set->sync();
}
