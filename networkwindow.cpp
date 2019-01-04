#include "networkwindow.h"
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

NetworkWindow::NetworkWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(483,458);
    this->setProperty("rotateValue", 0);
    this->setWindowTitle(tr("网络设置"));
    this->setWindowIcon(QIcon(QPixmap(":/Image/QQIcon.png")));

    mainWidget = new QLabel(this);
    mainWidget->setFixedSize(451,390);
    mainWidget->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    titleBar = new QLabel(this);
    titleBar->setFixedSize(431, 50);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    buttonMin = new QPushButton;
    buttonMin->setFlat(true);
    buttonMin->setFixedSize(25,25);
    buttonMin->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} "
                             "QPushButton:hover{background-color:#00BFFF;}"
                             "QPushButton:pressed{background-color:#009ACD;}"
                             "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(buttonMin, &QPushButton::clicked, this, &NetworkWindow::showMinimized);

    buttonClose = new QPushButton;
    buttonClose->setFlat(true);
    buttonClose->setFixedSize(25,25);
    buttonClose->setStyleSheet("QPushButton{border-image:url(:/Image/exit.png)} "
                               "QPushButton:hover{background-color:#00BFFF;}"
                               "QPushButton:pressed{background-color:#009ACD;}"
                               "QToolTip{background-color:white;font-family:微软雅黑;}");
    connect(buttonClose, &QPushButton::clicked, this, &NetworkWindow::returnToMainWindow);

    titleLayout->addSpacing(350);
    titleLayout->addWidget(buttonMin,0, Qt::AlignRight|Qt::AlignTop);
    titleLayout->addWidget(buttonClose,0, Qt::AlignRight|Qt::AlignTop);
    titleBar->setLayout(titleLayout);


    //content
    content = new QLabel(this);
    content->setFixedSize(431,260);
    content->setStyleSheet("QLabel{background-color:rgb(234,242,248);"
                           "border-left:1px solid rgb(152,159,175);"
                           "border-right:1px solid rgb(152,159,175);"
                           "font-family:微软雅黑;"
                           "font-size:9pt;}"
                           "QLineEdit{background-color:white;}"
                           "QComboBox{background-color:white;}");
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->setVerticalSpacing(10);
    gridLayout->setColumnMinimumWidth(0, 40);

    //row 1
    QLabel *labelNetworkSetting = new QLabel(tr("网络设置"),this);
    labelNetworkSetting->setStyleSheet("border-style:none;");
    gridLayout->addWidget(labelNetworkSetting,1,0,1,10, Qt::AlignLeft|Qt::AlignTop);

    //row 2
    QLabel *labelType = new QLabel(tr("类型:"),this);
    labelType->setStyleSheet("border-style:none;");
    QComboBox *comboType = new QComboBox(this);
    comboType->setEditable(true);
    comboType->setFixedSize(120,25);
    comboType->addItem(tr("不使用代理"));
    comboType->addItem(tr("HTTP代理"));
    comboType->addItem(tr("SOCKS5代理"));
    comboType->addItem(tr("使用浏览器设置"));
    comboType->setCurrentIndex(0);
    comboType->setStyleSheet("QComboBox{background:white;}"
                             "QComboBox:editable{border-style:none;"
                             "border:1px solid rgb(193,194,196);"
                             "background-color:white;"
                             "font-size:9pt;"
                             "font-family:微软雅黑;}"
                             "QComboBox::drop-down {border:0px;}"
                             "QComboBox QAbstractItemView{background:white;}"
                             "QComboBox::down-arrow{image:url(:/Image/dropdown.png);}"
                             "QComboBox::down-arrow:on{image:url(:/Image/dropdown_pressed.png);}");
    gridLayout->addWidget(labelType,2,0,1,1, Qt::AlignRight);
    gridLayout->addWidget(comboType,2,1,1,4);
    connect(comboType, &QComboBox::currentTextChanged, this, &NetworkWindow::updateWidgetStatus);

    QLabel *labelAddress = new QLabel(tr("地址:"),this);
    labelAddress->setStyleSheet("border-style:none;");
    editAddress = new QLineEdit(this);
    editAddress->setFixedHeight(25);
    editAddress->setEnabled(false);
    connect(editAddress, &QLineEdit::textChanged, this, &NetworkWindow::updateTestButtonStatus);
    gridLayout->addWidget(labelAddress,2,5,1,1);
    gridLayout->addWidget(editAddress,2,6,1,3);

    QLabel *labelPort = new QLabel(tr("端口:"),this);
    labelPort->setStyleSheet("border-style:none;");
    editPort = new QLineEdit(this);
    editPort->setFixedSize(50,25);
    editPort->setEnabled(false);
    connect(editPort, &QLineEdit::textChanged, this, &NetworkWindow::updateTestButtonStatus);
    gridLayout->addWidget(labelPort,2,9,1,1);
    gridLayout->addWidget(editPort,2,10,1,2);

    //row 3
    QLabel *labelUserName  = new QLabel(tr("用户名:"), this);
    labelUserName->setStyleSheet("border-style:none;");
    editUserName = new QLineEdit(this);
    editUserName->setFixedSize(120, 25);
    editUserName->setEnabled(false);
    connect(editUserName, &QLineEdit::textChanged, this, &NetworkWindow::updateTestButtonStatus);
    gridLayout->addWidget(labelUserName,3,0,1,1, Qt::AlignRight);
    gridLayout->addWidget(editUserName,3,1,1,4);

    QLabel *labelPassword = new QLabel(tr("密码:"),this);
    labelPassword->setStyleSheet("border-style:none;");
    editPassword = new QLineEdit(this);
    editPassword->setFixedHeight(25);
    editPassword->setEnabled(false);
    editPassword->setEchoMode(QLineEdit::Password);
    connect(editPassword, &QLineEdit::textChanged, this, &NetworkWindow::updateTestButtonStatus);
    gridLayout->addWidget(labelPassword,3,5,1,1);
    gridLayout->addWidget(editPassword,3,6,1,3);

    QLabel *labelRegion = new QLabel(tr("域:"), this);
    labelRegion->setStyleSheet("border-style:none;");
    editRegion = new QLineEdit(this);
    editRegion->setFixedSize(50,25);
    editRegion->setEnabled(false);
    connect(editRegion, &QLineEdit::textChanged, this, &NetworkWindow::updateTestButtonStatus);
    gridLayout->addWidget(labelRegion,3,9,1,1, Qt::AlignRight);
    gridLayout->addWidget(editRegion,3,10,1,2);

    //row 4
    buttonTest = new QPushButton(tr("测试"),this);
    buttonTest->setFixedSize(50,25);
    buttonTest->setEnabled(false);
    buttonTest->setStyleSheet("QPushButton{background-color:rgb(246,248,250);"
                              "border:1px solid rgb(152,159,175);"
                              "border-radius:2px;"
                              "font-size:9pt;"
                              "font-family:微软雅黑;} "
                              "QPushButton:hover{background-color:#00BFFF;}"
                              "QPushButton:pressed{background-color:#009ACD;}");
    gridLayout->addWidget(buttonTest,4,10,1,1, Qt::AlignRight);
    connect(buttonTest, &QPushButton::clicked, this, &NetworkWindow::networkTest);

    //row 5
    QLabel *labelLoginServer = new QLabel(tr("登录服务器:"),this);
    labelLoginServer->setStyleSheet("border-style:none;");
    gridLayout->addWidget(labelLoginServer,5,0,1,2);

    //row 6
    QLabel *labelLoginType = new QLabel(tr("类型:"), this);
    labelLoginType->setStyleSheet("border-style:none;");
    QComboBox *comboLoginType = new QComboBox(this);
    comboLoginType->setEditable(true);
    QStringList list = QStringList()<<"不使用高级选项"<<"UDP类型"<<"TCP类型";
    comboLoginType->addItems(list);
    comboLoginType->setFixedSize(120,25);
    comboLoginType->setStyleSheet("QComboBox{background:white;}"
                                  "QComboBox:editable{border-style:none;"
                                  "border:1px solid rgb(193,194,196);"
                                  "background-color:white;"
                                  "font-size:9pt;"
                                  "font-family:微软雅黑;}"
                                  "QComboBox::drop-down {border:0px;}"
                                  "QComboBox QAbstractItemView{background:white;}"
                                  "QComboBox::down-arrow{image:url(:/Image/dropdown.png);}"
                                  "QComboBox::down-arrow:on{image:url(:/Image/dropdown_pressed.png);}");
    gridLayout->addWidget(labelLoginType,6,0,1,1, Qt::AlignRight);
    gridLayout->addWidget(comboLoginType,6,1,1,4);

    QLabel *labelLoginAddress = new QLabel(tr("地址:"),this);
    labelLoginAddress->setStyleSheet("border-style:none;");
    comboLoginAddress = new QComboBox(this);
    comboLoginAddress->setEditable(true);
    comboLoginAddress->setFixedHeight(25);
    comboLoginAddress->setEnabled(false);
    comboLoginAddress->setStyleSheet("QComboBox{background:white;}"
                                     "QComboBox:editable{border-style:none;"
                                     "border:1px solid rgb(193,194,196);"
                                     "background-color:white;"
                                     "font-size:9pt;"
                                     "font-family:微软雅黑;}"
                                     "QComboBox::drop-down {border:0px;}"
                                     "QComboBox QAbstractItemView{background:white;}"
                                     "QComboBox::down-arrow{image:url(:/Image/dropdown.png);}"
                                     "QComboBox::down-arrow:on{image:url(:/Image/dropdown_pressed.png);}");
    gridLayout->addWidget(labelLoginAddress,6,5,1,1);
    gridLayout->addWidget(comboLoginAddress,6,6,1,3);

    QLabel *labelLoginPort = new QLabel(tr("端口:"), this);
    labelLoginPort->setStyleSheet("border-style:none;");
    editLoginPort = new QLineEdit(this);
    editLoginPort->setFixedSize(50,25);
    editLoginPort->setEnabled(false);
    gridLayout->addWidget(labelLoginPort,6,9,1,1);
    gridLayout->addWidget(editLoginPort,6,10,1,2);
    gridLayout->setContentsMargins(20,0,20,50);
    content->setLayout(gridLayout);
    content->setContentsMargins(0,20,0,0);

    bottom = new QLabel(this);
    bottom->setStyleSheet("background-color:rgb(204,227,242);"
                          "border-left:1px solid gray;"
                          "border-bottom:1px solid gray;"
                          "border-right:1px solid gray;"
                          "border-radius:2px;");
    bottom->setFixedSize(431,35);
    bottom->setContentsMargins(0,0,0,0);
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
    connect(buttonOk, &QPushButton::clicked, this, &NetworkWindow::returnToMainWindow);

    QPushButton *buttonCancel = new QPushButton(tr("取消"), this);
    buttonCancel->setFixedSize(80,25);
    buttonCancel->setFlat(true);
    buttonCancel->setStyleSheet("QPushButton{background-color:rgb(246,248,250);"
                                "border:1px solid rgb(152,159,175);"
                                "border-width:1px;"
                                "border-radius:3px;"
                                "font-size:9pt;"
                                "font-family:微软雅黑;} "
                                "QPushButton:hover{background-color:#00BFFF;}"
                                "QPushButton:pressed{background-color:#009ACD;}");
    connect(buttonCancel, &QPushButton::clicked, this, &NetworkWindow::returnToMainWindow);

    bottomLayout->addSpacing(220);
    bottomLayout->addWidget(buttonOk, Qt::AlignRight);
    bottomLayout->addWidget(buttonCancel, Qt::AlignRight);
    bottomLayout->setContentsMargins(5,5,5,5);
    bottomLayout->setSpacing(0);
    bottom->setLayout(bottomLayout);

    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(content);
    mainLayout->addWidget(bottom);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(20,20,20,40);
    mainWidget->setLayout(mainLayout);

    QLinearGradient linearGradient(QPoint(titleBar->x(), titleBar->y()),
                                   QPoint(titleBar->x(), titleBar->y() + titleBar->height()));
    linearGradient.setColorAt(0, QColor(37,158,209));
    linearGradient.setColorAt(1, QColor(234,242,248));
    linearGradient.setSpread(QGradient::RepeatSpread);
    QPixmap pixmap(titleBar->size());
    QPainter painter(&pixmap);
    painter.setBrush(linearGradient);
    painter.drawRect(titleBar->rect());
    titleBar->setPixmap(pixmap);
    titleBar->setStyleSheet("border-radius:2px;"
                            "border-left-style:none;"
                            "border-right:1px solid gray;"
                            "border-top:1px solid gray;");

    painter.end();

    mainWidget->show();

    rotatePixmap = QPixmap(mainWidget->size());
    mainWidget->render(&rotatePixmap);
}

void NetworkWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMousePressed = true;
        MousePressPos = event->pos();
    }
}

void NetworkWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed = false;
}

void NetworkWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed && event->buttons()==Qt::LeftButton)
    {
        move(event->pos() - MousePressPos + this->pos());
    }
}

void NetworkWindow::updateWidgetStatus(const QString &text)
{
    if(text == QString("不使用代理"))
    {
        editAddress->setEnabled(false);
        editPort->setEnabled(false);
        editUserName->setEnabled(false);
        editPassword->setEnabled(false);
        editRegion->setEnabled(false);
    }
    else
    {
        editAddress->setEnabled(true);
        editPort->setEnabled(true);
        editUserName->setEnabled(true);
        editPassword->setEnabled(true);
        editRegion->setEnabled(true);
        editRegion->setText(tr("80"));
    }
}

void NetworkWindow::updateTestButtonStatus()
{
    if(!editAddress->text().isEmpty()
            &&!editPort->text().isEmpty()
            &&!editUserName->text().isEmpty()
            &&!editPassword->text().isEmpty()
            &&!editRegion->text().isEmpty())
    {
        buttonTest->setEnabled(true);
    }
}

void NetworkWindow::networkTest()
{
    QString address = editAddress->text();
    QString port = editPort->text();
    QString username = editUserName->text();
    QString password = editPassword->text();
    QString region = editRegion->text();
    QLabel *information = new QLabel(this);
    information->setText(tr("测试成功"));
    information->setFixedSize(60,20);
    information->setFocusPolicy(Qt::NoFocus);
    information->setStyleSheet("border:1px solid gray;"
                               "font-family:微软雅黑;"
                               "font-size:9pt;"
                               "background-color:white;"
                               "border-radius:3px;");
    information->move(content->pos() + buttonTest->pos()+ QPoint(0,buttonTest->height()));
    information->show();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]{information->hide();});
    timer->start(2500);
}

void NetworkWindow::returnToMainWindow()
{
    /*QGraphicsView *view = new QGraphicsView;
    QGraphicsScene *scene = new QGraphicsScene;
    view->setScene(scene);
    //加入登录和设置窗体
    QGraphicsProxyWidget *graphicsWid1 = scene->addWidget(wid1);
    QGraphicsProxyWidget *graphicsWid2 = scene->addWidget(wid2);
    //设置窗体旋转-180度，并隐藏
    graphicsWid2->setTransform(QTransform().translate(graphicsWid2->boundingRect().width()/2,
                                                      graphicsWid2->boundingRect().height()/2)
                               .rotate(-180,Qt::YAxis)
                               .translate(-graphicsWid2->boundingRect().width()/2,
                                          -graphicsWid2->boundingRect().height()/2));
    graphicsWid2->setVisible(false);
    //新建线型值闹钟
    QTimeLine *timeLine = new QTimeLine(1500,scene);
    timeLine->setStartFrame(0);
    timeLine->setEndFrame(180);
    //每次值发生变化，旋转一定角度
    connect(timeLine,&QTimeLine::frameChanged,[timeLine,graphicsWid1,graphicsWid2](const int frame){
        //对于登录窗体正常旋转
        graphicsWid1->setTransform(QTransform().translate(graphicsWid1->boundingRect().width()/2,
                                                          graphicsWid1->boundingRect().height()/2)
                                   .rotate(frame,Qt::YAxis)
                                   .translate(-graphicsWid1->boundingRect().width()/2,
                                              -graphicsWid1->boundingRect().height()/2));

        //对于设置窗体由于开始旋转到了-180度，所以现在应该是-179,-176,-170....,0,这里应该是frame-180;也就是加上起点为-180度啦
        graphicsWid2->setTransform(QTransform().translate(graphicsWid2->boundingRect().width()/2,
                                                          graphicsWid2->boundingRect().height()/2)
                                   .rotate(frame - 180,Qt::YAxis)
                                   .translate(-graphicsWid2->boundingRect().width()/2,
                                              -graphicsWid2->boundingRect().height()/2));

        switch (timeLine->direction())
        {
            case QTimeLine::Forward: //前面向后面转，frame从0到180，当frame大于或等于90度时，显示设置窗体，登录窗体隐藏
                if(frame > 90 || frame == 90){
                    graphicsWid1->setVisible(false);
                    graphicsWid2->setVisible(true);
                }
                break;
            case QTimeLine::Backward: //后面向前面转，frame从180到0，当frame小于或等于90度时，显示登录窗体，设置窗体隐藏
                if(frame < 90 || frame == 90){
                    graphicsWid1->setVisible(true);
                    graphicsWid2->setVisible(false);
                }
                break;
        }
    });
    //旋转结束后，设置线形闹钟值的方向
    connect(timeLine,&QTimeLine::finished,[timeLine]{timeLine->toggleDirection();});
    //启用闹钟，每过一段时间，启动一次线型闹钟
    QTimer *timer = new QTimer;
    timer->setInterval(3000);
    connect(timer,&QTimer::timeout,[timeLine]{timeLine->start();});

    view->show();
    timer->start();

    return a.exec();*/

    isRotating = true;
    isToShow = false;
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
    mainWidget->hide();
}

void NetworkWindow::startToShow()
{
    isRotating = true;
    isToShow = true;
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
    mainWidget->hide();
}


void NetworkWindow::onRotateFinished()
{
    isRotating = false;
    if(isToShow)
    {
        mainWidget->show();
        repaint();
    }
    else
    {
        emit rotateFinished(this->mapToGlobal(QPoint(0,0)));
        this->deleteLater();
    }
}

void NetworkWindow::paintEvent(QPaintEvent *event)
{
    if(isRotating)
    {
        // 小于90度时;
        int rotateValue = this->property("rotateValue").toInt();
        if (rotateValue <= 90)
        {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(mainWidget->x() + mainWidget->width()/2, 0);
            transform.rotate(rotateValue, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1* mainWidget->width()/2,mainWidget->y(), rotatePixmap);
        }
        // 大于90度时
        else
        {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(mainWidget->x() + mainWidget->width()/2,0);
            transform.rotate(rotateValue + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1* mainWidget->width()/2,mainWidget->y(), rotatePixmap);
        }
    }
    else
    {
        return QWidget::paintEvent(event);
    }
}
