#include "widget.h"
#include "ui_widget.h"
#include "QPalette"
#include "QPixmap"
#include "QHBoxLayout"
#include "QDesktopServices"
#include "QUrl"
#include "QPainter"
#include "QBitmap"
#include "QDebug"
#include "QGraphicsDropShadowEffect"
#include "QMouseEvent"
#include "QPropertyAnimation"
#include "QMenu"
#include "QListWidget"
#include "QListWidgetItem"
#include "acctinfodetail.h"
#include "QMessageBox"
#include "QTimer"
#include "networkwindow.h"
#include "QtNetwork"
#include "QTcpSocket"
#include "loginfailwindow.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(483,458);
    this->setProperty("rotateValue",0);

    ui->WidgetPartOne->setStyleSheet("background-color:#00B2EE;"
                                     "border-radius:3px;");
    ui->WidgetPartOne->setContentsMargins(0,0,0,0);


    ui->widgetPartTwo->setContentsMargins(0,10,10,0);
    ui->widgetPartTwo->setStyleSheet("background-color:#FFFAFA;"
                                     "font-family:微软雅黑;");

    initTitle();
    initUserheadPic();
    createStatusMenu();
    initUI();
    initErrorMsgLabel();
    readSetting();
    rotatePic = QPixmap(ui->WidgetPartOne->size());
    ui->WidgetPartOne->render(&rotatePic);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(25);
    effect->setOffset(0);
    effect->setColor(Qt::lightGray);
    ui->WidgetPartOne->setGraphicsEffect(effect);

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::readMessage);
    connect(tcpSocket, &QTcpSocket::connected, this, &Widget::sendData);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

Widget::~Widget()
{
    delete ui;
    this->deleteLater();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousePressPoint = event->pos();
        isMousePressed = true;
    }
    if(isAcctListViewCreated)
    {
        acctListView->hide();
        buttonAcctList->setStyleSheet("QPushButton{background-image:url(:/Image/down.png);}"
                                      "QPushButton:hover{background-image:url(:/Image/down_hover.png);}"
                                      "QPushButton{border-style:none;}");
    }
    emit click();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed && event->buttons() == Qt::LeftButton)
    {
        move(event->pos() - mousePressPoint + this->pos());
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed = false;
}

void Widget::focusOutEvent(QFocusEvent *e)
{
    isMousePressed = false;
    return QWidget::focusOutEvent(e);
}

void Widget::paintEvent(QPaintEvent *event)
{
    if(isRotating)
    {
        int rotateValue = this->property("rotateValue").toInt();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QTransform transform;
        transform.translate(ui->WidgetPartOne->x() + ui->WidgetPartOne->width()/2, 0);
        transform.rotate(rotateValue, Qt::YAxis);
        painter.setTransform(transform);
        painter.drawPixmap(-1*ui->WidgetPartOne->width()/2,ui->WidgetPartOne->y(), rotatePic);
    }
    else
    {
        return QWidget::paintEvent(event);
    }
}

void Widget::initTitle()
{
    ui->buttonSetting->setFixedSize(25,25);
    ui->buttonSetting->setFlat(true);
    ui->buttonSetting->setAttribute(Qt::WA_TranslucentBackground);
    ui->buttonSetting->setCursor(Qt::PointingHandCursor);
    ui->buttonSetting->setContentsMargins(0,0,0,0);
    ui->buttonSetting->setToolTip(tr("网络设置"));
    ui->buttonSetting->setStyleSheet("QPushButton{border-image:url(:/Image/setting.png)} "
                                     "QPushButton:hover{background-color:#00BFFF;}"
                                     "QPushButton:pressed{background-color:#009ACD;}"
                                     "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(ui->buttonSetting, &QPushButton::clicked, this, &Widget::activateSettingWindow);

    ui->buttonMin->setFixedSize(25,25);
    ui->buttonMin->setFlat(true);
    ui->buttonMin->setAttribute(Qt::WA_TranslucentBackground);
    ui->buttonMin->setCursor(Qt::PointingHandCursor);
    ui->buttonMin->setContentsMargins(0,0,0,0);
    ui->buttonMin->setToolTip(tr("最小化"));
    ui->buttonMin->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} "
                                 "QPushButton:hover:!pressed{background-color:#00BFFF;}"
                                 "QPushButton:pressed{background-color:#009ACD;}"
                                 "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(ui->buttonMin, &QPushButton::released, this, &Widget::minWindow, Qt::QueuedConnection);

    ui->buttonClose->setFixedSize(25,25);
    ui->buttonClose->setFlat(true);
    ui->buttonClose->setAttribute(Qt::WA_TranslucentBackground);
    ui->buttonClose->setCursor(Qt::PointingHandCursor);
    ui->buttonClose->setContentsMargins(0,0,0,0);
    ui->buttonClose->setToolTip(tr("关闭QQ"));
    ui->buttonClose->setStyleSheet("QPushButton{border-image:url(:/Image/exit.png)} "
                                   "QPushButton:hover{background-color:red;}"
                                   "QPushButton:pressed{background-color:#009ACD;}"
                                   "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(ui->buttonClose, &QPushButton::clicked, this, &Widget::close);

    ui->QQIcon->setFixedSize(30,30);
    ui->QQIcon->setPixmap(QPixmap(":/Image/QQTitle.png").scaled(ui->QQIcon->size()));
    QPalette p;
    p.setBrush(QPalette::Text, QBrush(QColor(Qt::white)));
    ui->QQText->setPalette(p);
    ui->QQText->setFont(QFont("Arial",16));
    ui->QQText->setText(tr("QQ"));

    this->setWindowIcon(QIcon(":/Image/QQIcon.png"));
    this->setWindowTitle(tr("QQ2018"));
}

void Widget::initUserheadPic()
{
    //头像
    userHeadIcon = new MyLabel(ui->WidgetPartOne);
    if(lastUserHeadIcon.isNull())
    {
        QPixmap defaultUserHead(":/Image/QQ.jpg");
        lastUserHeadIcon = defaultUserHead;
        QPixmap userIcon(":/Image/d.jpg");
        userIcon = getRoundImage(lastUserHeadIcon,30);
        userHeadIcon->setFixedSize(65,65);
        userHeadIcon->setPixmap(userIcon);
        userHeadIcon->setLeavePic(userIcon);
        userIcon = getRoundHoverImage(userIcon,30);
        userHeadIcon->setEnterPic(userIcon);
    }
    else
    {
        QPixmap userIcon(":/Image/d.jpg");
        userIcon = getRoundImage(lastUserHeadIcon,30);
        userHeadIcon->setFixedSize(65,65);
        userHeadIcon->setPixmap(userIcon);
        userHeadIcon->setLeavePic(userIcon);
        userIcon = getRoundHoverImage(userIcon,30);
        userHeadIcon->setEnterPic(userIcon);

    }
    userHeadIcon->setAttribute(Qt::WA_TranslucentBackground);

    int width = ui->WidgetPartOne->width();
    int height = ui->WidgetPartOne->height();
    userHeadIcon->move(width/2-30, height/2-70);
    userHeadIcon->show();

    moreAcctIcon = new MyLabel(ui->WidgetPartOne);
    QPixmap moreAcct(":/Image/moreAcct.png");
    moreAcct = getRoundImage(moreAcct,20);
    moreAcctIcon->setFixedSize(60,60);
    moreAcctIcon->setAttribute(Qt::WA_TranslucentBackground);
    moreAcctIcon->setPixmap(moreAcct);
    moreAcctIcon->setLeavePic(moreAcct);
    moreAcct = getRoundHoverImage(moreAcct,20);
    moreAcctIcon->setEnterPic(moreAcct);
    moreAcctIcon->move(width/2+20, height/2-70);
    moreAcctIcon->hide();
    isMoreAcctTriggered = false;

    animation = new QPropertyAnimation(moreAcctIcon, "geometry");
    animation->setDuration(100);
    animation->setStartValue(QRect(moreAcctIcon->pos().x(),
                                   moreAcctIcon->pos().y(),
                                   moreAcctIcon->width(),
                                   moreAcctIcon->height()));
    animation->setEndValue(QRect(moreAcctIcon->pos().x()+30,
                                 moreAcctIcon->pos().y(),
                                 moreAcctIcon->width(),
                                 moreAcctIcon->height()));
    connect(userHeadIcon, &MyLabel::clicked, this, &Widget::onMoreAcctClicked);
    connect(this, &Widget::click, [=]{moreAcctIcon->hide();});
}

void Widget::initUI()
{
    //帐号输入行
    headIconAction  = new QAction(this);
    headIconAction->setIcon(QIcon(QPixmap(":/Image/Nofocus.png")));
    ui->acctInput->addAction(headIconAction, QLineEdit::LeadingPosition);
    ui->acctInput->setFont(QFont(tr("微软雅黑"),8));
    ui->acctInput->setPlaceholderText(tr("QQ号码/手机/邮箱"));
    //提示字体大小与输入字体大小不一样
    connect(ui->acctInput, &QLineEdit::textChanged, this, &Widget::changeTextSize);
    ui->acctInput->setStyleSheet("border:none;");
    QHBoxLayout *acctInputLayout = new QHBoxLayout;
    buttonAcctList = new QPushButton;
    buttonAcctList->setFlat(true);
    buttonAcctList->setCursor(Qt::PointingHandCursor);
    buttonAcctList->setFixedSize(25,25);
    buttonAcctList->setFocusPolicy(Qt::NoFocus);
    buttonAcctList->setStyleSheet("QPushButton{background-image:url(:/Image/down.png);}"
                                  "QPushButton:hover{background-image:url(:/Image/down_hover.png);}"
                                  "QPushButton{border-style:none;}");
    acctInputLayout->addWidget(buttonAcctList);
    acctInputLayout->setContentsMargins(0,0,3,0);
    acctInputLayout->setAlignment(Qt::AlignRight);
    connect(buttonAcctList, &QPushButton::clicked, this, &Widget::showDropDownAcctList);
    ui->acctInput->setLayout(acctInputLayout);
    ui->acctInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                 "border-bottom:1px solid #FFE4E1;}");
    ui->acctInput->installEventFilter(this);
    if(!lastUserAcctNo.isEmpty())
    {
        ui->acctInput->setText(lastUserAcctNo);
    }

    //密码输入行
    QHBoxLayout *passwordLayout = new QHBoxLayout;
    buttonKeyBoard = new QPushButton;
    buttonKeyBoard->setFixedSize(25,25);
    buttonKeyBoard->setFlat(true);
    buttonKeyBoard->setFocusPolicy(Qt::NoFocus);
    buttonKeyBoard->setStyleSheet("QPushButton{background-image:url(:/Image/miniKeyboard.png);}"
                                  "QPushButton:hover{background-image:url(:/Image/minikeyboard_hover.png);}"
                                  "QPushButton{border-style:none;}");
    buttonKeyBoard->setCursor(Qt::PointingHandCursor);
    passwordLayout->addWidget(buttonKeyBoard);
    passwordLayout->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    passwordLayout->setContentsMargins(0,0,3,0);
    ui->passwordInput->setLayout(passwordLayout);
    ui->passwordInput->setPlaceholderText(tr("密码"));
    ui->passwordInput->setEchoMode(QLineEdit::Password);
    pwAction = new QAction(this);
    pwAction->setIcon(QIcon(":/Image/locked.png"));
    ui->passwordInput->addAction(pwAction, QLineEdit::LeadingPosition);
    ui->passwordInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                     "border-bottom:1px solid #FFE4E1;}");
    ui->passwordInput->installEventFilter(this);


    if(isRemPWChecked&&!lastUserPassWord.isEmpty())
    {
        ui->passwordInput->setText(lastUserPassWord);
    }
    //connect(buttonKeyBoard, &QPushButton::clicked, this, &Widget::showKeyboard);

    //忘记密码键
    ui->buttonForgetPassword->setFlat(true);
    ui->buttonForgetPassword->setCursor(Qt::PointingHandCursor);
    ui->buttonForgetPassword->setStyleSheet("QPushButton{border-style:none;font-size:9pt;color:gray;}"
                                            "QPushButton:hover{color:#FF83FA}");
    connect(ui->buttonForgetPassword, &QPushButton::clicked, [=]{QDesktopServices::openUrl(QUrl("http:://www.qq.com"));});

    //登录键
    ui->buttonLogin->setFlat(true);
    ui->buttonLogin->setStyleSheet("QPushButton{background:rgb(7,189,253);"
                                   "color:white;"
                                   "font-size:11pt;"
                                   "border-style:outset;"
                                   "border-width:3px;"
                                   "border-color:rgba(209,237,255,30);"
                                   "border-radius:5px;}"
                                    "QPushButton:hover{background:#FF83FA;}"
                                    "QPushButton:pressed{background:#FFBBFF;}");
    ui->buttonLogin->setFocusPolicy(Qt::NoFocus);
    ui->buttonLogin->setFixedHeight(35);
    ui->buttonLogin->setCursor(Qt::PointingHandCursor);
    connect(ui->buttonLogin, &QPushButton::clicked, this, &Widget::login);

    //二维码登录
    ui->buttonFlicker->setFlat(true);
    ui->buttonFlicker->setFixedSize(25,25);
    ui->buttonFlicker->setStyleSheet("QPushButton{background-image:url(:/Image/flicker.png);border-style:none;}"
                                     "QPushButton:hover{background-image:url(:/Image/flicker_hover.png);}");
    connect(ui->buttonFlicker, &QPushButton::clicked, this, &Widget::loginByFlicker);
    //注册账号
    ui->buttonRegist->setFlat(true);
    ui->buttonRegist->setStyleSheet("QPushButton{color:gray;font-size:9pt}"
                                    "QPushButton:hover{color:rgb(7,189,253);}");
    ui->buttonRegist->setText(tr("注册帐号"));
    connect(ui->buttonRegist, &QPushButton::clicked, [=]{QDesktopServices::openUrl(QUrl("http:://www.qq.com"));});

    //自动登陆
    ui->checkBoxAutoLogin->setStyleSheet("QCheckBox{font-size:9pt;color:gray;}"
                                         "QCheckBox::indicator {width:13px; height:13px}"
                                         "QCheckBox::indicator:unchecked {image: url(:/Image/unchecked.png);}"
                                         "QCheckBox::indicator:unchecked:hover {image: url(:/Image/uncheck_hover.png);}"
                                         "QCheckBox::indicator:checked {image: url(:/Image/checked.png);}"
                                         "QCheckBox::indicator:checked:hover {image: url(:/Image/checked_hover.png);}"
                                         "QCheckBox::indicator:indeterminate:hover {image:url(:/Image/unchecked.png)}");
    //记住密码
    ui->checkBoxRememberPassword->setStyleSheet("QCheckBox{font-size:9pt;color:gray;}"
                                                "QCheckBox::indicator {width:13px; height:13px}"
                                                "QCheckBox::indicator:unchecked {image: url(:/Image/unchecked.png);}"
                                                "QCheckBox::indicator:unchecked:hover {image: url(:/Image/uncheck_hover.png);}"
                                                "QCheckBox::indicator:checked {image: url(:/Image/checked.png);}"
                                                "QCheckBox::indicator:checked:hover {image: url(:/Image/checked_hover.png);}"
                                                "QCheckBox::indicator:indeterminate:hover {image:url(:/Image/unchecked.png)}");

}

void Widget::initErrorMsgLabel()
{
    isErrorMsgLabelCreated = true;
    errorMsg = new QLabel(this);
    errorMsg->setFixedSize(ui->WidgetPartOne->width(),32);
    errorMsg->setStyleSheet("background:rgb(249,244,213);"
                                "font-family:微软雅黑;"
                                "font-size:8;");

    QHBoxLayout *layout = new QHBoxLayout;
    QPixmap icon(":/Image/ErrorPic.png");
    QLabel *labelIcon = new QLabel(errorMsg);
    labelIcon->setFixedSize(16,16);
    labelIcon->setPixmap(icon);
    labelIcon->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *errorLayout = new QVBoxLayout;
    errorInfo = new QLabel(errorMsg);
    errorInfo->setFixedHeight(16);

    errorCode = new QLabel(errorMsg);
    errorCode->setFixedHeight(16);

    errorLayout->addWidget(errorInfo, Qt::AlignTop);
    errorLayout->addWidget(errorCode, Qt::AlignTop);
    errorLayout->setContentsMargins(0,0,0,0);
    errorLayout->setSpacing(0);
    errorLayout->setStretch(0,0);

    buttonHide = new QPushButton(errorMsg);
    buttonHide->setFlat(true);
    buttonHide->setFixedWidth(30);
    buttonHide->setAttribute(Qt::WA_TranslucentBackground);
    buttonHide->setStyleSheet("QPushButton{background-image:url(:/Image/up.png);"
                                            "border-style:none;}"
                              "QPushButton:hover{background-image:url(:/Image/pressed.png);}");
    connect(buttonHide, &QPushButton::clicked, [=]{errorMsg->hide();});

    layout->addWidget(labelIcon, Qt::AlignLeading|Qt::AlignTop);
    layout->addLayout(errorLayout);
    layout->addWidget(buttonHide, Qt::AlignRight|Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->setStretch(0,0);

    errorMsg->setLayout(layout);
    errorMsg->move(ui->WidgetPartOne->pos()+
                   QPoint(0,ui->WidgetPartOne->height()));

    errorMsg->hide();
}

void Widget::readSetting()
{
    acctInfo singleAcctInfo;
    singleAcctInfo.IconFilePath = QString(":/Image/head_1.jpg");
    singleAcctInfo.userName = QString("一切随风");
    singleAcctInfo.userAcctNumber = QString("80592999");
    userAcctList.append(singleAcctInfo);

    singleAcctInfo.IconFilePath = QString(":/Image/head_2.jpg");
    singleAcctInfo.userName = QString("需要钱");
    singleAcctInfo.userAcctNumber = QString("32343246");
    userAcctList.append(singleAcctInfo);

    singleAcctInfo.IconFilePath = QString(":/Image/head_3.jpg");
    singleAcctInfo.userName = QString("很多很多钱");
    singleAcctInfo.userAcctNumber = QString("849461356");
    userAcctList.append(singleAcctInfo);

    singleAcctInfo.IconFilePath = QString(":/Image/head_4.jpg");
    singleAcctInfo.userName = QString("可以红色票子");
    singleAcctInfo.userAcctNumber = QString("884569124");
    userAcctList.append(singleAcctInfo);

    singleAcctInfo.IconFilePath = QString(":/Image/head_5.jpg");
    singleAcctInfo.userName = QString("毛爷爷");
    singleAcctInfo.userAcctNumber = QString("4566214");
    userAcctList.append(singleAcctInfo);

    singleAcctInfo.IconFilePath = QString(":/Image/head_6.jpg");
    singleAcctInfo.userName = QString("彩票一定要中奖");
    singleAcctInfo.userAcctNumber = QString("884526564");
    userAcctList.append(singleAcctInfo);
}

void Widget::activateSettingWindow()
{
    rotatePic = QPixmap(ui->WidgetPartOne->size());
    rotatePic = QWidget::grab(QRect(ui->WidgetPartOne->pos(), ui->WidgetPartOne->size()));
    //ui->WidgetPartOne->render(&rotatePic,QPoint(), QRegion(), QWidget::DrawWindowBackground|QWidget::DrawChildren|QWidget::IgnoreMask);

    if(isAcctListViewCreated)
    {
        acctListView->hide();
        buttonAcctList->setStyleSheet("QPushButton{background-image:url(:/Image/down.png);}"
                                      "QPushButton:hover{background-image:url(:/Image/down_hover.png);}"
                                      "QPushButton{border-style:none;}");
    }

    if(isRotating)
        return;
    isRotating = true;
    QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "rotateValue");
    // 设置旋转持续时间;
    rotateAnimation->setDuration(500);
    // 设置旋转角度变化趋势;
    rotateAnimation->setEasingCurve(QEasingCurve::InCubic);
    // 设置旋转角度范围;
    rotateAnimation->setStartValue(5);
    rotateAnimation->setEndValue(90);
    connect(rotateAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    connect(rotateAnimation, SIGNAL(finished()), this, SLOT(onRotateFinished()));
    // 隐藏当前窗口，通过不同角度的绘制来达到旋转的效果;
    rotateAnimation->start();
    isToShowNetworkWindow = true;
    ui->WidgetPartOne->hide();
}

void Widget::activeMainWindow(QPoint point)
{
    this->move(point);
    if(isRotating)
        return;
    isRotating = true;
    QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "rotateValue");
    // 设置旋转持续时间;
    rotateAnimation->setDuration(500);
    // 设置旋转角度变化趋势;
    rotateAnimation->setEasingCurve(QEasingCurve::InCubic);
    // 设置旋转角度范围;
    rotateAnimation->setStartValue(90);
    rotateAnimation->setEndValue(5);
    connect(rotateAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    connect(rotateAnimation, SIGNAL(finished()), this, SLOT(onRotateFinished()));
    // 隐藏当前窗口，通过不同角度的绘制来达到旋转的效果;
    rotateAnimation->start();
    isToShowNetworkWindow = false;
}

void Widget::startLoginWindow()
{
    ui->widgetPartTwo->hide();
    logining = new QLabel(tr("登录中..."), ui->WidgetPartOne);
    logining->setFixedWidth(100);
    logining->setStyleSheet("font-family:微软雅黑;"
                            "font-size:13pt;"
                            "color:white;"
                            "border-style:none;"
                            "background:transparent;");
    logining->move(ui->WidgetPartOne->pos() +
                   userHeadIcon->pos() +
                   QPoint(-15, userHeadIcon->width()));
    logining->show();

    buttonCancelLogin = new QPushButton(tr("取消"),ui->WidgetPartOne);
    buttonCancelLogin->setFlat(true);
    buttonCancelLogin->setFixedSize(100,33);
    buttonCancelLogin->setStyleSheet("border:1px solid white;"
                                     "background:transparent;"
                                     "font-family:微软雅黑;"
                                     "font-size:11pt;"
                                     "color:white;");
    buttonCancelLogin->move(ui->widgetPartTwo->pos()+
                            ui->buttonLogin->pos()+
                            QPoint(ui->buttonLogin->width()/3,5));
    buttonCancelLogin->show();
    connect(buttonCancelLogin, &QPushButton::clicked, [=]{ui->widgetPartTwo->show();
                                                            buttonCancelLogin->deleteLater();
                                                            logining->deleteLater();
                                                            tcpSocket->close();});
}

void Widget::startErrorWindow()
{
    qDebug()<<"startErrorWindow";
    if(!isLoginFailWindowActive)
    {
        loginFailwind = new LoginFailWindow(this);
        loginFailwind->show();
        isLoginFailWindowActive = true;
        connect(loginFailwind, &LoginFailWindow::destroyed, [=]{isLoginFailWindowActive = false;
                                                                tcpSocket->close();
                                                                 ui->widgetPartTwo->show();
                                                                 buttonCancelLogin->deleteLater();
                                                                 logining->deleteLater();});
    }
}

void Widget::showNetworkErrorInfo(QString error)
{
    qDebug()<<"showNetworkErrorInfo";
    errorInfo->setText(error);
    errorCode->setText(error);
    errorMsg->show();
}

void Widget::sendData()
{
    qDebug()<<"sendData";
    QString acctNo = ui->acctInput->text();
    QString password = ui->passwordInput->text();
    QByteArray outblock;
    QDataStream out(&outblock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out << acctNo<<password;

    int ret = tcpSocket->write(outblock);
    if(ret == -1)
    {
        showNetworkErrorInfo(tcpSocket->errorString());
        return;
    }
    outblock.resize(0);
}

void Widget::startMainWindow()
{
    qDebug()<<"mainWindow";
    QPropertyAnimation *animationToRight = new QPropertyAnimation(ui->WidgetPartOne, "geometry");
    animationToRight->setDuration(500);
    animationToRight->setStartValue(QRect(ui->WidgetPartOne->x(),
                                   ui->WidgetPartOne->y(),
                                   ui->WidgetPartOne->width(),
                                   ui->WidgetPartOne->height()));
    animationToRight->setEndValue(QRect(ui->WidgetPartOne->x()+ui->WidgetPartOne->width()+25,
                                 ui->WidgetPartOne->y(),
                                 ui->WidgetPartOne->width(),
                                 ui->WidgetPartOne->height()));

    QPropertyAnimation *animationToCenter = new QPropertyAnimation(ui->WidgetPartOne, "geometry");
    animationToCenter->setDuration(500);
    animationToCenter->setStartValue(QRect(ui->WidgetPartOne->x()+ui->WidgetPartOne->width()+25,
                                           ui->WidgetPartOne->y(),
                                           ui->WidgetPartOne->width(),
                                           ui->WidgetPartOne->height()));
    animationToCenter->setEndValue(QRect(ui->WidgetPartOne->x()+ui->WidgetPartOne->width()+25,
                                         ui->WidgetPartOne->y()+ui->WidgetPartOne->height()/2,
                                         5,
                                         5));

    animationToRight->start();
    connect(animationToRight, &QPropertyAnimation::finished, [=]{animationToCenter->start();});
    connect(animationToCenter, &QPropertyAnimation::finished, [=]{ui->WidgetPartOne->deleteLater();});
}

void Widget::onRotateFinished()
{
    isRotating = false;
    if(isToShowNetworkWindow)
    {
        NetworkWindow *network = new NetworkWindow;
        network->show();
        network->move(this->mapToGlobal(QPoint(0,0)));
        network->startToShow();
        connect(network, &NetworkWindow::rotateFinished, this, &Widget::activeMainWindow);
    }
    else
    {
        ui->WidgetPartOne->show();
    }
}

void Widget::readMessage()
{
    qDebug()<<"read data from Server";
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_6);
    if(tcpSocket->bytesAvailable())
        in>>message;
    qDebug()<<"message = "<<message;
    if(message == QString("success"))
    {
        tcpSocket->close();
        logining->setText("登录成功");
        buttonCancelLogin->deleteLater();
        userHeadIcon->hide();
        loginStatusIcon->hide();
        moreAcctIcon->hide();

        QPropertyAnimation *animationText = new QPropertyAnimation(logining, "geometry");
        animationText->setDuration(500);
        animationText->setStartValue(QRect(logining->x(), logining->y(), logining->width(), logining->height()));
        animationText->setEndValue(QRect(logining->x(), logining->y()-50, logining->width(),logining->height()));
        animationText->start();
        connect(animationText, &QPropertyAnimation::finished, this, &Widget::startMainWindow);
    }
    else if(message == QString("password_error"))
    {
        tcpSocket->close();
        startErrorWindow();
    }
}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<"displayError";
    ui->widgetPartTwo->show();
    buttonCancelLogin->hide();
    logining->hide();
    showNetworkErrorInfo(tcpSocket->errorString());
}

void Widget::login()
{
    if(ui->acctInput->text().isEmpty())
    {
        ui->acctInput->setFocus();
        QLabel *message = new QLabel(this);
        message->setText(tr("帐号不能为空"));
        QTimer *timer = new QTimer(this);
        message->move(ui->WidgetPartOne->pos() + ui->widgetPartTwo->pos() + ui->acctInput->pos()+QPoint(60,2));
        message->setStyleSheet("background-color:#FFF5EE;"
                               "font-family:微软雅黑;"
                               "font-size:9pt;"
                               "border-radius:5px;"
                               "border-style:none;"
                               "padding:5px;");
        message->show();
        connect(timer, &QTimer::timeout, [=]{message->hide();});
        timer->start(2500);
    }
    else if(ui->passwordInput->text().isEmpty())
    {
        ui->passwordInput->setFocus();
        QLabel *message = new QLabel(this);
        message->setText(tr("密码不能为空"));
        QTimer *timer = new QTimer(this);
        message->move(ui->WidgetPartOne->pos() + ui->widgetPartTwo->pos() + ui->passwordInput->pos()+QPoint(60,2));
        message->setStyleSheet("background-color:#FFF5EE;"
                               "font-family:微软雅黑;"
                               "font-size:9pt;"
                               "border-radius:5px;"
                               "border-style:none;"
                               "padding:5px;");
        message->show();
        connect(timer, &QTimer::timeout, [=]{message->hide();});
        timer->start(2500);
    }
    else
    {
        blockSize = 0;
        tcpSocket->abort();
        tcpSocket->connectToHost("localhost", 6666);
        if(isErrorMsgLabelCreated)
            errorMsg->hide();
        startLoginWindow();
        sendData();
    }
}

void Widget::onMoreAcctClicked()
{
    if(isMoreAcctTriggered)
    {
        QPropertyAnimation *rollBackAnimation = new QPropertyAnimation(moreAcctIcon, "geometry");
        rollBackAnimation->setDuration(100);
        rollBackAnimation->setStartValue(QRect(moreAcctIcon->pos().x(),
                                       moreAcctIcon->pos().y(),
                                       moreAcctIcon->width(),
                                       moreAcctIcon->height()));
        rollBackAnimation->setEndValue(QRect(moreAcctIcon->pos().x()-30,
                                     moreAcctIcon->pos().y(),
                                     moreAcctIcon->width(),
                                     moreAcctIcon->height()));
        rollBackAnimation->start();
        connect(rollBackAnimation, &QPropertyAnimation::finished,[=]{moreAcctIcon->hide();});
        isMoreAcctTriggered = false;
    }
    else
    {
        moreAcctIcon->show();
        animation->start();
        isMoreAcctTriggered = true;
    }
}

void Widget::createStatusMenu()
{
    loginStatusIcon = new MyLabel(ui->WidgetPartOne);
    loginStatusIcon->setFixedSize(20,20);
    QPixmap menuIcon(20,20);
    menuIcon.fill(QColor(9,241,117));
    menuIcon = getRoundImage(menuIcon,7);
    loginStatusIcon->setPixmap(menuIcon);

    loginStatusMenu = new QMenu(tr("Menu"),ui->WidgetPartOne);
    actionOnline = loginStatusMenu->addAction(QIcon(":/Image/online.png"), tr("我在线上"));
    actionActive = loginStatusMenu->addAction(QIcon(":/Image/welcome.png"), tr("Q我吧"));
    loginStatusMenu->addSeparator();
    actionAway = loginStatusMenu->addAction(QIcon(":/Image/away.png"), tr("离开"));
    actionBusy = loginStatusMenu->addAction(QIcon(":/Image/busy.png"), tr("忙碌"));
    actionNoDisturb = loginStatusMenu->addAction(QIcon(":/Image/Nodisturb.png"), tr("请勿打扰"));
    loginStatusMenu->addSeparator();
    actionHide = loginStatusMenu->addAction(QIcon(":/Image/hide.png"), tr("隐身"));

    loginStatusMenu->setFixedSize(loginStatusMenu->size().width()*1.5, loginStatusMenu->size().height()*7);
    loginStatusMenu->setWindowFlags(loginStatusMenu->windowFlags()|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    loginStatusMenu->setAttribute(Qt::WA_TranslucentBackground);
    loginStatusMenu->setStyleSheet("QMenu{border: 1px solid lightgray;"
                                           "border-radius:5px;"
                                           "padding:0px;"
                                           "font-family:微软雅黑;"
                                           "background:rgba(255,255,255,80%);"
                                           "font-size:9pt;}\
                                    QMenu::item {padding:10 5 5 40;border-radius:5px;}\
                                    QMenu::item:selected{background:rgb(39 , 134 , 228);}\
                                    QMenu::icon{padding:10 5 5 15 ;}\
                                    QMenu::separator{height:1px;\
                                                    margin-left:1px;\
                                                    margin-top:2px;\
                                                    margin-right:0;\
                                                    margin-bottom:2px;\
                                                    background-color:rgb(228,228,228);}");

    int x = userHeadIcon->x();
    int y = userHeadIcon->y();
    int width = userHeadIcon->width();
    int height = userHeadIcon->height();
    loginStatusIcon->move(x+width*3/4, y+height*3/4);
    loginStatusIcon->setAttribute(Qt::WA_TranslucentBackground);
    loginStatusIcon->raise();
    //connect(loginStatusIcon, &MyLabel::clicked,[=]{loginStatusMenu->exec(QCursor::pos());});
    connect(loginStatusIcon, &MyLabel::clicked,[=]{loginStatusMenu->exec(loginStatusIcon->mapToGlobal(QPoint(0,0)+ QPoint(0,25)));});
    connect(loginStatusMenu, &QMenu::triggered, this, &Widget::setLoginStatus);
}

void Widget::setLoginStatus(QAction *action)
{
    loginStatusIcon->setPixmap(action->icon().pixmap(20,20));
}

QPixmap Widget::PixmapToRound(QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->acctInput)
    {
        if(event->type() == QEvent::FocusIn)
        {
            ui->acctInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                         "border-bottom:1px solid #FF83FA;}");
            headIconAction->setIcon(QIcon(":/Image/focus.png"));
        }
        else if(event->type() == QEvent::FocusOut)
        {
            ui->acctInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                         "border-bottom:1px solid #FFE4E1;}");
            headIconAction->setIcon(QIcon(":/Image/Nofocus.png"));
        }
    }
    else if(watched == ui->passwordInput)
    {
        if(event->type() == QEvent::FocusIn)
        {
            ui->passwordInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                         "border-bottom:1px solid #FF83FA;}");
            pwAction->setIcon(QIcon(":/Image/unlocked.png"));
        }
        else if(event->type() == QEvent::FocusOut)
        {
            ui->passwordInput->setStyleSheet("QLineEdit{border: 1px solid white;"
                                         "border-bottom:1px solid #FFE4E1;}");
            pwAction->setIcon(QIcon(":/Image/locked.png"));
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::minWindow()
{
    ui->buttonMin->setAttribute(Qt::WA_UnderMouse, false);
    ui->buttonMin->repaint();
    this->showMinimized();
}

void Widget::changeTextSize()
{
    buttonAcctList->setStyleSheet("QPushButton{background-image:url(:/Image/down.png);}"
                                  "QPushButton:hover{background-image:url(:/Image/down_hover.png);}"
                                  "QPushButton{border-style:none;}");
    if(ui->acctInput->text().isEmpty())
        ui->acctInput->setFont(QFont(tr("微软雅黑"),8));
    else
        ui->acctInput->setFont(QFont(tr("微软雅黑"),13));
}

QPixmap Widget::getRoundImage(const QPixmap &src, int radius)
{
    //创建遮罩
    /*QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    painter.end();

    //创建圆形图像
    QImage resultImage(size, QImage::Format_ARGB32_Premultiplied);
    QPainter painter2(&resultImage);
    painter2.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    painter2.setCompositionMode(QPainter::CompositionMode_Source);
    painter2.fillRect(resultImage.rect(), Qt::transparent);
    painter2.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter2.drawPixmap(0, 0, mask);
    painter2.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter2.drawPixmap(0, 0, src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter2.end();
    return QPixmap::fromImage(resultImage);*/

    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(174,217,248));
    painter.setPen(pen);
    painter.drawEllipse(0,0,2*radius+4, 2*radius+4);

    return pixmap;
}

QPixmap Widget::getRoundHoverImage(QPixmap &src, int radius)
{
    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(255,128,255));
    painter.setPen(pen);
    painter.drawEllipse(0,0,2*radius+4, 2*radius+4);

    return pixmap;
}

void Widget::showDropDownAcctList()
{
    buttonAcctList->setStyleSheet("QPushButton{background-image:url(:/Image/pressed.png);}"
                                  "QPushButton{border-style:none;}");

    if(isAcctListViewCreated)
        acctListView->hide();
    acctListView = new QListWidget(this);
    acctListView->setFixedWidth(ui->acctInput->width()-5);

    for(int i = 0; i < userAcctList.size(); i++)
    {
        AcctInfoDetail *item = new AcctInfoDetail;
        item->setDetailInfo(userAcctList.at(i).userName,
                            userAcctList.at(i).userAcctNumber,
                            userAcctList.at(i).IconFilePath);
        QListWidgetItem *listItem = new QListWidgetItem(acctListView);
        listItem->setSizeHint(QSize(ui->acctInput->width()-25, 60));
        acctListView->setItemWidget(listItem, item);
        connect(item, &AcctInfoDetail::showAcctInfo, this, &Widget::setChoosedAcctInfo);
        connect(item, &AcctInfoDetail::removeAcct, this, &Widget::removeAcctFromDropList);
    }

    QPoint pos_WidgetPartOne = ui->WidgetPartOne->pos();
    QPoint pos_WidgetPartTwo = ui->widgetPartTwo->pos();
    QPoint pos = pos_WidgetPartOne + pos_WidgetPartTwo;
    acctListView->move(pos.x() + ui->acctInput->pos().x(),
                       pos.y() + ui->acctInput->pos().y() + ui->acctInput->height()+2);
    acctListView->setStyleSheet("QListWidget{border-radius:5px;background-color: rgba(255,255,255,240);}"
                                "QListWidget::item{margin:6px 0 3px 0;}\
                                QListWidget::item:selected{border-radius:3px;\
                                                        background-color:rgba(106 , 171 , 219 , 80);}\
                                QListWidget::item:hover{border-radius:3px;\
                                                        background-color:rgb(220 , 220 , 220);}");
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setOffset(0);
    acctListView->setGraphicsEffect(effect);
    acctListView->show();
    acctListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    acctListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    isAcctListViewCreated = true;
    acctListView->setMouseTracking(true);
    connect(acctListView, &QListWidget::itemEntered, this, &Widget::activeDelButton);
}

void Widget::setChoosedAcctInfo(QString nickName, QString AcctNo, QString filePath)
{
    ui->acctInput->setText(AcctNo);
    QPixmap pixmap(filePath);
    pixmap = getRoundImage(pixmap,30);
    userHeadIcon->setPixmap(pixmap);
    userHeadIcon->setLeavePic(pixmap);
    pixmap = getRoundHoverImage(pixmap,30);
    userHeadIcon->setEnterPic(pixmap);
    acctListView->hide();
    ui->passwordInput->clear();
}

void Widget::removeAcctFromDropList(QString nickName, QString AcctNo, QString filePath)
{
    for(int i = 0; i < userAcctList.size(); i++ )
    {
        if(userAcctList.at(i).userAcctNumber == AcctNo)
        {
            userAcctList.removeAt(i);
            break;
        }
    }
    showDropDownAcctList();
}

void Widget::activeDelButton(QListWidgetItem *item)
{
    AcctInfoDetail *detail = qobject_cast<AcctInfoDetail *>(acctListView->itemWidget(item));
    for(int i = 0; i < userAcctList.size(); i++)
    {
        if(item == acctListView->item(i))
        {
            detail->setDelButtonStatus(false);
        }
        else
        {
            AcctInfoDetail *RestItem = qobject_cast<AcctInfoDetail *>(acctListView->itemWidget(acctListView->item(i)));
            RestItem->setDelButtonStatus(true);
        }
    }
}

void Widget::loginByFlicker()
{
    ui->widgetPartTwo->hide();
    userHeadIcon->hide();
    loginStatusIcon->hide();
    twoDimensionCodePic = new MyLabel(this);
    twoDimensionCodePic->setStyleSheet("border-radius:25px;");
    twoDimensionCodePic->setFixedSize(130,130);

    QPixmap pixmap(":/Image/TwoDimensionCode.png");
    pixmap = pixmap.scaled(twoDimensionCodePic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    twoDimensionCodePic->setPixmap(pixmap);
    twoDimensionCodePic->move(ui->WidgetPartOne->pos() + QPoint(ui->WidgetPartOne->width()/3, ui->WidgetPartOne->height()/5));
    twoDimensionCodePic->show();

    twoDimensioncodeText = new MyLabel(this);
    twoDimensioncodeText->setOpenExternalLinks(true);
    twoDimensioncodeText->setText(QString("用")+
                                  QString("<style>a{text-decoration: none}</style><a href =www.qq.com> QQ手机版</a>")+
                                  QString("扫描二维码安全登录。") +
                                  QString("<style>a{text-decoration: none}</style><a href =www.qq.com> 手机丢失?</a>"));
    twoDimensioncodeText->setStyleSheet("font-family:微软雅黑;"
                                        "font-size:10pt;");
    twoDimensioncodeText->move(twoDimensionCodePic->x()-55,
                               twoDimensionCodePic->y()+twoDimensionCodePic->height()+25);
    twoDimensioncodeText->show();

    backButton = new QPushButton(this);
    backButton->setFlat(true);
    backButton->setText(tr("返回"));
    backButton->move(ui->WidgetPartOne->pos() +
                     ui->widgetPartTwo->pos()+
                     ui->buttonLogin->pos());
    backButton->setStyleSheet("QPushButton{background:rgb(7,189,253);"
                              "font-family:微软雅黑;"
                              "color:white;"
                              "font-size:11pt;"
                              "border-style:outset;"
                              "border-width:3px;"
                              "border-color:rgba(209,237,255,30);"
                              "border-radius:5px;}"
                              "QPushButton:hover{background:#FF83FA;}"
                              "QPushButton:pressed{background:#FFBBFF;}");
    backButton->setFocusPolicy(Qt::NoFocus);
    backButton->setFixedSize(ui->buttonLogin->size());
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, &QPushButton::clicked, [=]{ui->widgetPartTwo->show();
                                                    userHeadIcon->show();
                                                    loginStatusIcon->show();
                                                    twoDimensionCodePic->hide();
                                                    twoDimensioncodeText->hide();
                                                    backButton->hide();});
    backButton->show();

}
