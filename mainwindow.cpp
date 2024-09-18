#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QTranslator>
#include "serialportwrap.h"
#include "logger.h"
#include "widgetutils.h"
#include "qtabout.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&Logger::Instance(),SIGNAL(OnLogging(QString)),this,SLOT(onLog(QString)));
    qInstallMessageHandler(Logger::CustomMessageHandler);

    updateSerialPort();

    _timer=new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&MainWindow::sendMsg);

    _serialPort=new QSerialPort(this);
    _wrap=new SerialPortWrap(this);
    _wrap->setSerialPort(_serialPort);
    connect(_wrap,&SerialPortWrap::receivedData,this,&MainWindow::onReceivedData);

    ui->cmbBaudRate->clear();
    QStringList baudRates={"4800","9600","19200","38400"};
    ui->cmbBaudRate->addItems(baudRates);

    ui->cmbDataBits->clear();
    ui->cmbDataBits->addItems(SerialPortWrap::getDataList());

    ui->cmbParityCom->clear();
    ui->cmbParityCom->addItems(SerialPortWrap::getParityList());

    ui->cmbStopBit->clear();
    ui->cmbStopBit->addItems(SerialPortWrap::getStopList());

    ui->cmbFlowControl->clear();
    ui->cmbFlowControl->addItems(SerialPortWrap::getFlowList());

    loadConfig();

    loadSendConfig();

    ui->gbSend->setEnabled(false);

    undateLanguage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getAvailablePortsString()
{
    QList<QSerialPortInfo> availablePorts=QSerialPortInfo::availablePorts();
    QStringList ret;
    for(auto item:availablePorts)
    {
        ret.append(item.portName());
    }
    return ret;
}

void MainWindow::undateLanguage()
{
    LanguageParam p=ConfigVar::instance().getLanguageParam();
    switchLanguage(p.CurrentLanguage);
    ui->retranslateUi(this);

    if(p.CurrentLanguage=="zh")
    {
        ui->actionChinese->setChecked(true);
        ui->actionEnglish->setChecked(false);
    }
    else
    {
        ui->actionChinese->setChecked(false);
        ui->actionEnglish->setChecked(true);
    }
}

void MainWindow::switchLanguage(QString path)
{
    QString strQM = QString(":/res/serialPort_%1_CN.qm").arg(path);

    if (QFile(strQM).exists())
    {
        QTranslator* translator = new QTranslator;
        qDebug()<<__FUNCTION__<<__LINE__;
        if (translator->load(strQM))
        {
            qApp->removeTranslator(translator);
            //            delete m_translator;
            qApp->installTranslator(translator);
        }
        else
        {
            delete translator;
            translator = NULL;
        }
    }
}

void MainWindow::updateSerialPort()
{
    QStringList availablePorts =getAvailablePortsString();
    ui->cmbPortName->clear();
    ui->cmbPortName->addItems(availablePorts);
}

void MainWindow::loadConfig()
{
    SerialPortParam sp=ConfigVar::instance().getSerialPortParam();
    QStringList pn=getAvailablePortsString();
    if(pn.contains(sp.COMName))
    {
        ui->cmbPortName->setCurrentText(sp.COMName);
    }
    ui->cmbBaudRate->setCurrentText(sp.BaudRate);
    ui->cmbDataBits->setCurrentText(sp.DataBits);
    ui->cmbParityCom->setCurrentText(sp.Parity);
    ui->cmbStopBit->setCurrentText(sp.StopBits);
    ui->cmbFlowControl->setCurrentText(sp.FlowControl);
}

void MainWindow::saveConfig()
{
    SerialPortParam sp;
    sp.COMName=ui->cmbPortName->currentText();
    sp.BaudRate=ui->cmbBaudRate->currentText();
    sp.DataBits=ui->cmbDataBits->currentText();
    sp.Parity=ui->cmbParityCom->currentText();
    sp.StopBits=ui->cmbStopBit->currentText();
    sp.FlowControl=ui->cmbFlowControl->currentText();
    ConfigVar::instance().setSerialPortParam(sp);
}

void MainWindow::loadSendConfig()
{
    SendParam sp=ConfigVar::instance().getSendParam();
    ui->txtSend->setText(sp.SendMsg);
}

void MainWindow::saveSendConfig()
{
    SendParam sp;
    sp.SendMsg=ui->txtSend->text();
    ConfigVar::instance().setSendParam(sp);
}

void MainWindow::onLog(QString log)
{
    ui->txtLog->appendPlainText(log);
    if(ui->txtLog->document()->lineCount()>500)
    {
        ui->txtLog->document()->clear();
    }
}

void MainWindow::onReceivedData(QByteArray arr)
{
    if(ui->chbRecord->isChecked())
    {
        qDebug()<<"接收:"<<getFormatString(arr);
    }
    if(ui->chbRecSub->isChecked())
    {
        QString formattedData = getFormatString(arr);
        // 使用HTML格式设置颜色
        QString html = QString("<span style='color:green;'>接收: </span><span>%1</span>").arg(formattedData);
        ui->reciveData->append(html);  // 使用 append 追加内容
    }
}

void MainWindow::on_btnUpdataSerialPort_clicked()
{
    // 获取所有可用的串口信息
    updateSerialPort();
}

void MainWindow::on_openSerial_clicked()
{
    QMap<QString,QSerialPort::Parity> parityMap=SerialPortWrap::getParityMap();
    QMap<QString,QSerialPort::DataBits> dataMap=SerialPortWrap::getDataMap();
    QMap<QString,QSerialPort::StopBits> stopMap=SerialPortWrap::getStopMap();
    _serialPort->setPortName(ui->cmbPortName->currentText());
    _serialPort->setParity(parityMap[ui->cmbParityCom->currentText()]);
    _serialPort->setBaudRate(ui->cmbBaudRate->currentText().toInt());
    _serialPort->setDataBits(dataMap[ui->cmbDataBits->currentText()]);
    _serialPort->setStopBits(stopMap[ui->cmbStopBit->currentText()]);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);  //无流控制
    _serialPort->setReadBufferSize(1000);
    if(_serialPort->open(QIODevice::ReadWrite))
    {
        ui->openSerial->setEnabled(false);
        ui->gbSend->setEnabled(true);
        saveConfig();
    }
}

void MainWindow::on_closeSerial_clicked()
{
    if(_serialPort!=nullptr)
    {
        _serialPort->close();
        ui->openSerial->setEnabled(true);
        ui->gbSend->setEnabled(false);
    }
}

QByteArray MainWindow::sendMsg()
{
    QByteArray arr=ui->chbHex->isChecked()?QByteArray::fromHex(ui->txtSend->text().toUtf8())
                                         :ui->txtSend->text().toUtf8();
    _wrap->sendNoReply(arr);
    if(ui->chbRecord->isChecked())
    {
        qDebug()<<"发送:"<<ui->txtSend->text();
    }

    QString formattedData = getFormatString(arr);
    // 使用HTML格式设置颜色
    QString html = QString("<span style='color:red;'>发送: </span><span>%1</span>").arg(formattedData);
    ui->reciveData->append(html);  // 使用 append 追加内容

    return arr;
}

void MainWindow::on_sendDataBtn_clicked()
{
    QByteArray arr = sendMsg();

    saveSendConfig();
}

void MainWindow::on_sendDataBtn_2_clicked()
{
    WidgetUtils::Execute(ui->sendDataBtn_2,[this]
    {
        QByteArray arr=QByteArray::fromHex(ui->txtSend->text().toUtf8());
        QByteArray rec;
        _wrap->sendReply(arr,rec);

        QString formattedData = getFormatString(arr);
        // 使用HTML格式设置颜色
        QString html = QString("<span style='color:red;'>发送: </span><span>%1</span>"
                               "<span style='color:green;'>  接收: </span><span>%2</span>").arg(formattedData).arg(getFormatString(rec));
        ui->reciveData->append(html);  // 使用 append 追加内容

        saveSendConfig();
    });
}

QString MainWindow::getFormatString(QByteArray arr)
{
    QString formatted = "";
    if(ui->chbHex->isChecked())
    {
        for (int i = 0; i < arr.size(); ++i) {
          formatted += QString(" %1").arg(static_cast<unsigned char>(arr[i]), 2, 16, QLatin1Char('0')).toUpper();
        }
    }
    else
    {
        formatted=QString::fromUtf8(arr);
    }
    return formatted;
}

void MainWindow::on_actionAbout_triggered()
{
    QtAbout *w=new QtAbout(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btnClear_clicked()
{
    ui->reciveData->clear();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    ui->sendDataBtn->setEnabled(!checked);
    ui->sendDataBtn_2->setEnabled(!checked);
    if(checked)
    {
        _timer->setInterval(ui->spinBox->value());
        _timer->start();
    }
    else
    {
        _timer->stop();
    }
}

void MainWindow::on_actionChinese_triggered()
{
    LanguageParam p;
    p.CurrentLanguage="zh";
    ConfigVar::instance().setLanguageParam(p);
    WidgetUtils::WidgetUtilsNew::ShowWarning(tr("Restart the software to take effect"));
//    undateLanguage();
}

void MainWindow::on_actionEnglish_triggered()
{
    LanguageParam p;
    p.CurrentLanguage="en";
    ConfigVar::instance().setLanguageParam(p);
    WidgetUtils::WidgetUtilsNew::ShowWarning(tr("Restart the software to take effect"));
//    undateLanguage();
}
