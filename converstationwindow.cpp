#include "converstationwindow.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QTextEdit"
#include "QTextDocument"
#include "QMouseEvent"
#include "QFontMetrics"
#include "QDesktopWidget"
#include "QApplication"
#include "QDebug"
#include "QGraphicsDropShadowEffect"
#include "QPainter"
#include "QPainterPath"
#include "qmath.h"
#include "QScrollBar"
converstationWindow::converstationWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(600,500);
    this->setMaximumSize(QApplication::desktop()->availableGeometry().size());
    //this->setStyleSheet("border:1px solid black;");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    titleBar = new QLabel(this);
    titleBar->setFixedSize(this->width(), 38);
    titleBar->setMaximumWidth(QApplication::desktop()->availableGeometry().width());

    QHBoxLayout *titleLayout = new QHBoxLayout;
    nameAndQQZone = new QLabel(this);
    QHBoxLayout *nameQQZoneLayout = new QHBoxLayout;

    nickName = new QLabel(nameAndQQZone);
    nickName->setText("Helloworld");
    nickName->setFont(QFont("微软雅黑",12));
    nickName->setAlignment(Qt::AlignCenter);
    QFontMetrics fontWidth(nickName->font());
    int width = fontWidth.width(nickName->text());  //计算字符串宽度
    nickName->setFixedSize(width+4,25);

    buttonQQZone = new QPushButton(nameAndQQZone);
    buttonQQZone->setFlat(true);
    buttonQQZone->setFixedSize(25,25);
    buttonQQZone->setCursor(Qt::PointingHandCursor);
    buttonQQZone->setStyleSheet("background-image:url(:/Image/focus.png)");
    nameQQZoneLayout->addStretch(3);
    //nameQQZoneLayout->addSpacing((this->width() - 4*30 - nickName->width() - buttonQQZone->width())/2);
    nameQQZoneLayout->addWidget(nickName, Qt::AlignCenter);
    nameQQZoneLayout->addWidget(buttonQQZone, Qt::AlignCenter);
    nameQQZoneLayout->addStretch(2);
    //nameQQZoneLayout->addSpacing((this->width() - 4*30 - nickName->width() - buttonQQZone->width())/2);
    nameQQZoneLayout->setSpacing(0);
    nameQQZoneLayout->setMargin(0);
    nameAndQQZone->setLayout(nameQQZoneLayout);
    nameAndQQZone->resize(this->width() - 4*30, 25+2);
    nameAndQQZone->setStyleSheet("border-style:none;");

    buttonDropdown = new QPushButton(this);
    buttonDropdown->setFlat(true);
    buttonDropdown->setFixedSize(25,25);
    buttonDropdown->setCursor(Qt::PointingHandCursor);
    buttonDropdown->setStyleSheet("QPushButton{background-image:url(:/Image/down_hover.png)}"
                                  "QToolTip{background-color:white;font-family:微软雅黑;}");

    buttonMin = new QPushButton;
    buttonMin->setFlat(true);
    buttonMin->setFixedSize(25,25);
    buttonMin->setCursor(Qt::PointingHandCursor);
    buttonMin->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} ");
    connect(buttonMin, &QPushButton::clicked, this, &converstationWindow::showMinimized);

    buttonMax = new QPushButton(this);
    buttonMax->setFlat(true);
    buttonMax->setFixedSize(25,25);
    buttonMax->setCursor(Qt::PointingHandCursor);
    buttonMax->setStyleSheet("background-image:url(:/Image/uncheck_hover.png)");
    connect(buttonMax, &QPushButton::clicked, this, &converstationWindow::showMaxWindow);

    buttonClose = new QPushButton;
    buttonClose->setFlat(true);
    buttonClose->setFixedSize(25,25);
    buttonClose->setCursor(Qt::PointingHandCursor);
    buttonClose->setAttribute(Qt::WA_TranslucentBackground);
    buttonClose->setStyleSheet("QPushButton{border-image:url(:/Image/exit.png)} "
                               "QPushButton:hover{background-color:#00BFFF;}"
                               "QPushButton:pressed{background-color:#009ACD;}");
    connect(buttonClose, &QPushButton::clicked, this, &converstationWindow::close);

    titleLayout->addWidget(nameAndQQZone);
    //titleLayout->addStretch(1);
    titleLayout->addWidget(buttonDropdown, 0 , Qt::AlignRight|Qt::AlignTop);
    titleLayout->addWidget(buttonMin, 0 , Qt::AlignRight|Qt::AlignTop);
    titleLayout->addWidget(buttonMax,0, Qt::AlignRight|Qt::AlignTop);
    titleLayout->addWidget(buttonClose,0, Qt::AlignRight|Qt::AlignTop);
    titleLayout->setSpacing(3);
    titleLayout->setContentsMargins(0,5,5,5);
    titleBar->setLayout(titleLayout);
    titleBar->setStyleSheet("QLabel{background-color:rgb(39,195,253);"
                            "border-style:none;}"
                            );
    //titleBar->setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *contentLayout = new QHBoxLayout;

    QHBoxLayout *mutiwayTalkLayout = new QHBoxLayout;
    voiceTalkButton = new QPushButton(this);
    voiceTalkButton->setFlat(true);
    voiceTalkButton->setFixedSize(30,30);
    voiceTalkButton->setCursor(Qt::PointingHandCursor);
    QPixmap pixmap_1(":/Image/online.png");
    pixmap_1 = getRoundImage(pixmap_1, voiceTalkButton->width()/2);
    voiceTalkButton->setIcon(QIcon(pixmap_1));


    videoTalkButton = new QPushButton(this);
    videoTalkButton->setFlat(true);
    videoTalkButton->setFixedSize(30,30);
    videoTalkButton->setCursor(Qt::PointingHandCursor);
    QPixmap pixmap_2(":/Image/offline.png");
    pixmap_2 = getRoundImage(pixmap_2, voiceTalkButton->width()/2);
    videoTalkButton->setIcon(QIcon(pixmap_2));

    otherButton = new QPushButton(this);
    otherButton->setFlat(true);
    otherButton->setFixedSize(30,30);
    otherButton->setCursor(Qt::PointingHandCursor);
    QPixmap pixmap_3(":/Image/hide.png");
    pixmap_3 = getRoundImage(pixmap_3, voiceTalkButton->width()/2);
    otherButton->setIcon(QIcon(pixmap_3));

    mutiwayTalkLayout->addStretch(5);
    mutiwayTalkLayout->addWidget(voiceTalkButton);
    mutiwayTalkLayout->addWidget(videoTalkButton);
    mutiwayTalkLayout->addWidget(otherButton);
    mutiwayTalkLayout->setSpacing(5);
    mutiwayTalkLayout->setContentsMargins(0,12,13,3);

    conversationLayout = new QVBoxLayout;
    converstaionContent = new QTextEdit(this);
    converstaionContent->resize(this->width()-140, this->height());
    //converstaionContent->setReadOnly(true);
    converstaionContent->setStyleSheet("border-bottom:1px solid lightgray;"
                                       "border-left-style:none;"
                                       "border-right-style:none;"
                                       "border-top-style:none;");

    QHBoxLayout *toolBoxLayout = new QHBoxLayout;
    expressionButton = new QPushButton(this);
    expressionButton->setFlat(true);
    expressionButton->setFixedSize(25,25);
    expressionButton->setCursor(Qt::PointingHandCursor);
    expressionButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    gifButton = new QPushButton(this);
    gifButton->setFlat(true);
    gifButton->setFixedSize(25,25);
    gifButton->setCursor(Qt::PointingHandCursor);
    gifButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    copyAndCutButton = new QPushButton(this);
    copyAndCutButton->setFlat(true);
    copyAndCutButton->setFixedSize(25,25);
    copyAndCutButton->setCursor(Qt::PointingHandCursor);
    copyAndCutButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    openFileButton = new QPushButton(this);
    openFileButton->setFlat(true);
    openFileButton->setFixedSize(25,25);
    openFileButton->setCursor(Qt::PointingHandCursor);
    openFileButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    openPicButton = new QPushButton(this);
    openPicButton->setFlat(true);
    openPicButton->setFixedSize(25,25);
    openPicButton->setCursor(Qt::PointingHandCursor);
    openPicButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    shakeButton = new QPushButton(this);
    shakeButton->setFlat(true);
    shakeButton->setFixedSize(25,25);
    shakeButton->setCursor(Qt::PointingHandCursor);
    shakeButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    QQpayButton = new QPushButton(this);
    QQpayButton->setFlat(true);
    QQpayButton->setFixedSize(25,25);
    QQpayButton->setCursor(Qt::PointingHandCursor);
    QQpayButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    QQMusicButton = new QPushButton(this);
    QQMusicButton->setFlat(true);
    QQMusicButton->setFixedSize(25,25);
    QQMusicButton->setCursor(Qt::PointingHandCursor);
    QQMusicButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    moreButton = new QPushButton(this);
    moreButton->setFlat(true);
    moreButton->setFixedSize(25,25);
    moreButton->setCursor(Qt::PointingHandCursor);
    moreButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    historyRecordButton = new QPushButton(this);
    historyRecordButton->setFlat(true);
    historyRecordButton->setFixedSize(25,25);
    historyRecordButton->setCursor(Qt::PointingHandCursor);
    historyRecordButton->setStyleSheet("background-image:url(:/Image/focus.png)");

    toolBoxLayout->addWidget(expressionButton);
    toolBoxLayout->addWidget(gifButton);
    toolBoxLayout->addWidget(copyAndCutButton);
    toolBoxLayout->addWidget(openFileButton);
    toolBoxLayout->addWidget(openPicButton);
    toolBoxLayout->addWidget(shakeButton);
    toolBoxLayout->addWidget(QQpayButton);
    toolBoxLayout->addWidget(QQMusicButton);
    toolBoxLayout->addWidget(moreButton);
    toolBoxLayout->addStretch(3);
    toolBoxLayout->setSpacing(5);
    toolBoxLayout->addWidget(historyRecordButton, Qt::AlignRight);
    toolBoxLayout->setContentsMargins(5,1,13,1);

//    toolboxLabel = new QLabel(this);
//    toolboxLabel->setFixedSize(this->width()-140,27);
//    toolboxLabel->setLayout(toolBoxLayout);

    inputArea = new QTextEdit(this);
    inputArea->resize(this->width()-140, this->height()-titleBar->height()
                                            -converstaionContent->height()
                                            );
    inputArea->setStyleSheet("border-style:none;");

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    closeButton = new QPushButton(this);
    closeButton->setText("关闭(C)");
    closeButton->setFlat(true);
    closeButton->setFixedSize(72,30);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet("QPushButton{font-size:10pt; color:black;border-radius:3px;"
                               "font-family:微软雅黑;border:1px solid lightgray;}"
                               "QPushButton:hover{background-color:#00BFFF;}"
                               "QPushButton:pressed{background-color:#009ACD;}");

    sendButton = new QPushButton(this);
    sendButton->setFixedSize(90,30);
    sendButton->setStyleSheet("QPushButton{background-color:rgb(80,158,225);font-size:10pt;"
                              "font-family:微软雅黑;"
                              "color:white;border-radius:3px;}"
                              "QPushButton:hover{background-color:#009ACD;}");

    QHBoxLayout *sendButtonLayout = new QHBoxLayout;

    sendMsgButton = new QPushButton(this);
    sendMsgButton->setText("发送(S)");
    sendMsgButton->setFlat(true);
    sendMsgButton->setFixedSize(63,30);
    sendMsgButton->setCursor(Qt::PointingHandCursor);

    sendMoreButton = new QPushButton(this);
    sendMoreButton->setFlat(true);
    sendMoreButton->setFixedSize(27,30);
    sendMoreButton->setCursor(Qt::PointingHandCursor);
    sendMoreButton->setStyleSheet("background-image:url(:/Image/down_hover.png);");
    sendButtonLayout->addWidget(sendMsgButton);
    sendButtonLayout->addWidget(sendMoreButton);
    sendButtonLayout->setMargin(0);
    sendButtonLayout->setSpacing(0);
    sendButton->setLayout(sendButtonLayout);

    bottomLayout->addStretch(4);
    bottomLayout->addWidget(closeButton, Qt::AlignRight);
    bottomLayout->addWidget(sendButton, Qt::AlignRight);
    bottomLayout->setSpacing(13);
    bottomLayout->setContentsMargins(0,5,13,5);
//    bottomLabel = new QLabel(this);
//    bottomLabel->setLayout(bottomLayout);
//    bottomLabel->resize(this->width()-140, 40);

    conversationLayout->addLayout(mutiwayTalkLayout, 1);
    conversationLayout->addWidget(converstaionContent, 9);
    conversationLayout->addLayout(toolBoxLayout, 1);
    conversationLayout->addWidget(inputArea, 4);
    conversationLayout->addLayout(bottomLayout, 1);
    conversationLayout->setContentsMargins(1,0,0,0);
    conversationLayout->setSpacing(0);

    QQshow = new QLabel(this);
    QQshow->resize(140,this->height() - titleBar->height());
    QPixmap pixmap(":/Image/head_4.jpg");
    pixmap = pixmap.scaled(QQshow->size(), Qt::KeepAspectRatio);
    QQshow->setPixmap(pixmap);
    QQshow->setContentsMargins(1,0,0,0);
    QQshow->setStyleSheet("background-color:white;border-left:1px solid lightgray;");

    contentLayout->addLayout(conversationLayout);
    contentLayout->addWidget(QQshow);
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->setStretch(0,0);

    mainLayout->addWidget(titleBar, Qt::AlignTop);
    mainLayout->addLayout(contentLayout);
    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setSpacing(0);
    mainLayout->setStretch(0,0);
    this->setLayout(mainLayout);

}


void converstationWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMousePressed = true;
        MousePressPos = event->pos();
    }
}

void converstationWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed = false;
}

void converstationWindow::showMaxWindow()
{
    if(this->isMaximized())
    {
        this->showNormal();
        buttonMax->setStyleSheet("background-image:url(:/Image/uncheck_hover.png)");
    }
    else
    {
        this->showMaximized();
//        QDesktopWidget* desktopWidget = QApplication::desktop();
//        QRect clientRect = desktopWidget->availableGeometry();
//        this->resize(clientRect.size());
//        converstaionContent->resize(this->width()-140, this->height()*4/7);
//        inputArea->resize(this->width()-140, this->height()
//                                                -titleBar->height()
//                                                -converstaionContent->height()
//                                                -60);
//        QQshow->resize(140,this->height() - titleBar->height());
        buttonMax->setStyleSheet("background-image:url(:/Image/uncheck.png)");
        //conversationLayout->setSpacing(0);
        //this->move(0,0);
    }
}

void converstationWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed && event->buttons()==Qt::LeftButton)
    {
        move(event->pos() - MousePressPos + this->pos());
    }
}

void converstationWindow::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i = 0; i < 5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(100 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QWidget::paintEvent(event);
}

QPixmap converstationWindow::getRoundImage(const QPixmap &src, int radius)
{
    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    return pixmap;
}
