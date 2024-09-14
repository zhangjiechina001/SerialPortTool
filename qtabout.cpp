#include "qtabout.h"
#include "ui_qtabout.h"

SINGLETON_IMPL(QtAbout)
QtAbout::QtAbout(QWidget *parent) : QDialog(parent), ui(new Ui::QtAbout)
{
    ui->setupUi(this);
    this->initForm();
}

QtAbout::~QtAbout()
{
    delete ui;
}

void QtAbout::showEvent(QShowEvent *)
{
//    QtHelper::setFormInCenter(this);
}

void QtAbout::initForm()
{
    //设置一个默认值的信息
    this->setInfo(AboutInfo());
}

void QtAbout::setInfo(const AboutInfo &info, int timeout)
{
    //设置标题
    ui->QtLabName->setText(info.title);

    //设置html内容
    QStringList list;
    list << QString("<html><body>");
    list << QString("<p>版本: %1</p>").arg(info.version);
    list << QString("<p>版权: %1</p>").arg(info.copyright);
    list << QString("<p>邮箱: %1</p>").arg(info.email);
    list << QString("<p>项目地址: <a href=\"%1\">%1</a></p>").arg(info.http);
    list << QString("</body></html>");
    ui->QtLabInfo->setText(list.join("\n"));
    //设置可以打开超链接
    ui->QtLabInfo->setOpenExternalLinks(true);

    //自适应内容以便留出更多的空间拖动
    //ui->QtLabInfo->adjustSize();
    //下面的方法更合适,只需要宽度留出空间就行
    QSize size = ui->QtLabInfo->sizeHint();
    ui->QtLabInfo->resize(size.width(), ui->QtLabInfo->height());

    //设置logo图片

    //重新设置下尺寸才能真正刷新改变后的颜色,尤其是Qt6
    if (this->width() == 720) {
        this->setFixedSize(721, 300);
    } else {
        this->setFixedSize(720, 300);
    }
}
