#include "loginfailwindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QGraphicsDropShadowEffect"
#include "QComboBox"
#include "QLineEdit"
#include "QLabel"
#include "QPushButton"
#include "QMouseEvent"
#include "QMessageBox"
#include "QTimer"
#include "QPainter"
#include "QTransform"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "QGraphicsProxyWidget"
#include "QTimeLine"
#include "QPropertyAnimation"
#include "QDebug"
#include "QBitmap"
#include "QLinearGradient"

LoginFailWindow::LoginFailWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(483,458);
    this->setProperty("rotateValue", 0);
    this->setWindowTitle(tr("网络设置"));
    this->setWindowIcon(QIcon(QPixmap(":/Image/QQIcon.png")));
    this->setContentsMargins(10,30,10,30);

    QVBoxLayout *TopLayout = new QVBoxLayout;
    TopLayout->setContentsMargins(10,30,10,30);

    mainWidget = new QWidget(this);
    mainWidget->setFixedSize(451,390);
    mainWidget->setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(15);
    effect->setOffset(0);
    effect->setColor(Qt::darkGray);
    mainWidget->setGraphicsEffect(effect);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    titleBar = new QLabel(this);
    titleBar->setFixedSize(441, 50);

    QHBoxLayout *titleLayout = new QHBoxLayout;

    QLabel *QQIcon = new QLabel(this);
    QQIcon->setFixedSize(30,30);
    QQIcon->setStyleSheet("border-style:none;");
    QQIcon->setPixmap(QPixmap(":/Image/QQTitle.png").scaled(QQIcon->size()));

    QLabel *QQText = new QLabel(this);
    QQText->setFixedSize(40,30);
    QQText->setStyleSheet("border-style:none;color:white;font-family:Arial;font-size:16pt");
    QQText->setText(tr("QQ"));


    buttonMin = new QPushButton;
    buttonMin->setFlat(true);
    buttonMin->setFixedSize(25,25);
    buttonMin->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} "
                             "QPushButton:hover{background-color:#00BFFF;}"
                             "QPushButton:pressed{background-color:#009ACD;}"
                             "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(buttonMin, &QPushButton::clicked, this, &LoginFailWindow::showMinimized);

    buttonClose = new QPushButton;
    buttonClose->setFlat(true);
    buttonClose->setFixedSize(25,25);
    buttonClose->setStyleSheet("QPushButton{border-image:url(:/Image/exit.png)} "
                               "QPushButton:hover{background-color:#00BFFF;}"
                               "QPushButton:pressed{background-color:#009ACD;}"
                               "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(buttonClose, &QPushButton::clicked, this, &LoginFailWindow::deleteLater);

    titleLayout->addWidget(QQIcon, Qt::AlignLeading);
    titleLayout->addWidget(QQText, Qt::AlignLeft);
    titleLayout->addSpacing(290);
    titleLayout->addWidget(buttonMin,0, Qt::AlignRight|Qt::AlignTop);
    titleLayout->addWidget(buttonClose,0, Qt::AlignRight|Qt::AlignTop);
    titleLayout->setContentsMargins(10,10,10,10);
    titleBar->setLayout(titleLayout);


    //content
    content = new QLabel(this);
    content->setFixedSize(441,260);
    content->setContentsMargins(0,0,10,0);
    content->setStyleSheet("QLabel{background-color:rgb(234,242,248);"
                           "font-family:微软雅黑;"
                           "font-size:9pt;}");

    QHBoxLayout *layout = new QHBoxLayout;

    QLabel *icon = new QLabel;
    icon->setFixedSize(30,30);
    icon->setPixmap(QPixmap(":/Image/warning.png").scaled(icon->size()));

    QVBoxLayout *errorLayout = new QVBoxLayout;
    QLabel *errorContent_1 = new QLabel;
    errorContent_1->setFixedWidth(350);
    errorContent_1->setAlignment(Qt::AlignTop);
    errorContent_1->setOpenExternalLinks(true);
    errorContent_1->setContentsMargins(10,0,0,0);
    errorContent_1->setText(QString("你输入的帐号名或者密码不正确，原因可能是：\n")+
                                QString("1、帐号名输入有误;\n")+
                                QString("2、忘记密码;\n")+
                                QString("3、未区分字母大小写;\n")+
                                QString("4、未开启小键盘;\n")+
                                QString("5、帐号名未绑定QQ号。\n"));

    QLabel *errorContent_2 = new QLabel;
    errorContent_2->setFixedWidth(350);
    errorContent_2->setOpenExternalLinks(true);
    errorContent_2->setContentsMargins(10,0,0,0);
    errorContent_2->setText(QString("如果你的密码丢失或遗忘，可访问QQ安全中心")+
                            QString("<style>a{text-decoration: none}</style> <a href =www.qq.com> 找回密码</a>")+
                            QString("。\n\n"));

    QLabel *errorContent_3 = new QLabel;
    errorContent_3->setFixedWidth(350);
    errorContent_3->setOpenExternalLinks(true);
    errorContent_3->setContentsMargins(10,0,0,0);
    errorContent_3->setText(QString("如果你的帐号为非大陆手机号，请")+
                            QString("<style>a{text-decoration: none}</style> <a href =www.qq.com> 点击这里</a>") +
                            QString("进行登录。"));
    errorLayout->addWidget(errorContent_1);
    errorLayout->addWidget(errorContent_2);
    errorLayout->addWidget(errorContent_3);


    layout->addWidget(icon,3,Qt::AlignTop|Qt::AlignRight);
    layout->addLayout(errorLayout);
    layout->setContentsMargins(40,20,40,40);
    layout->setSpacing(0);

    content->setLayout(layout);

    bottom = new QLabel(this);
    bottom->setStyleSheet("background-color:rgb(204,227,242);"
                          "border-radius:2px;");
    bottom->setFixedSize(441,35);
    bottom->setContentsMargins(0,0,0,10);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QPushButton *buttonOk = new QPushButton(tr("确定"), this);
    buttonOk->setFixedSize(80,25);
    buttonOk->setFlat(true);
    buttonOk->setStyleSheet("QPushButton{background-color:rgb(246,248,250);"
                            "border:1px solid rgb(152,159,175);"
                            "border-radius:3px;"
                            "font-size:9pt;"
                            "font-family:微软雅黑;} "
                            "QPushButton:hover{background-color:#00BFFF;}"
                            "QPushButton:pressed{background-color:#009ACD;}");
    connect(buttonOk, &QPushButton::clicked, this, &LoginFailWindow::deleteLater);

    bottomLayout->addSpacing(350);
    bottomLayout->addWidget(buttonOk, Qt::AlignRight|Qt::AlignCenter);
    bottomLayout->setContentsMargins(0,5,0,5);
    bottom->setLayout(bottomLayout);

    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(content);
    mainLayout->addWidget(bottom);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(5,20,5,40);
    mainWidget->setLayout(mainLayout);

    QLinearGradient linearGradient(QPoint(titleBar->x(), titleBar->y()),
                                   QPoint(titleBar->x(), titleBar->y() + titleBar->height()));
    linearGradient.setColorAt(0, QColor(37,158,209));
    linearGradient.setColorAt(1, QColor(234,242,248));
    linearGradient.setSpread(QGradient::RepeatSpread);
    QPixmap pixmap(titleBar->size());
    QPainter painter(&pixmap);
    painter.setBrush(linearGradient);
    painter.drawRect(-1,-1, titleBar->width()+2,titleBar->height()+2);
    titleBar->setPixmap(pixmap);
    titleBar->setStyleSheet("border-radius:5px;border-style:none;");
    painter.end();

    TopLayout->addWidget(mainWidget);
    this->setLayout(TopLayout);
}


void LoginFailWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMousePressed = true;
        MousePressPos = event->pos();
    }
}

void LoginFailWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed = false;
}

void LoginFailWindow::onButtonClicked()
{
    emit returnToMainWindow();
}

void LoginFailWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed && event->buttons()==Qt::LeftButton)
    {
        move(event->pos() - MousePressPos + this->pos());
    }
}

