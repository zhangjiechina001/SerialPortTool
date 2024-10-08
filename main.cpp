﻿#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>



void switchLanguange(QString path)
{
    QString strQM = QString(":/res/serialPort_%1_CN.qm").arg(path);

    if (QFile(strQM).exists())
    {
        QTranslator* translator = new QTranslator;
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LanguageParam p=ConfigVar::instance().getLanguageParam();
    switchLanguange(p.CurrentLanguage);
    QApplication::setWindowIcon(QIcon(":/icon.ico"));
    MainWindow w;
    w.show();

    return a.exec();
}
