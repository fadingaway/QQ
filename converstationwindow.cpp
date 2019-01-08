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
#include "QMenu"
#include "QTime"

converstationWindow::converstationWindow(QWidget *parent,
                                         QString acctNo,
                                         QString nickName,
                                         QString friendAcctNo,
                                         QString friendNickName) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(600,500);
    this->acctNo = acctNo;
    this->nickName = nickName;
    this->friendAcctNo = friendAcctNo;
    this->friendNickName = friendNickName;
    this->setMaximumSize(QApplication::desktop()->availableGeometry().size());
    //this->setStyleSheet("border:1px solid black;");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    titleBar = new QLabel(this);
    titleBar->setFixedSize(this->width(), 38);
    titleBar->setMaximumWidth(QApplication::desktop()->availableGeometry().width());

    QHBoxLayout *titleLayout = new QHBoxLayout;
    nameAndQQZone = new QLabel(this);
    QHBoxLayout *nameQQZoneLayout = new QHBoxLayout;

    nickNameLabel = new QLabel(nameAndQQZone);
    nickNameLabel->setText(this->friendNickName);
    nickNameLabel->setFont(QFont("微软雅黑",12));
    nickNameLabel->setAlignment(Qt::AlignCenter);
    QFontMetrics fontWidth(nickNameLabel->font());
    int width = fontWidth.width(nickNameLabel->text());  //计算字符串宽度
    nickNameLabel->setFixedSize(width+4,25);

    buttonQQZone = new QPushButton(nameAndQQZone);
    buttonQQZone->setFlat(true);
    buttonQQZone->setFixedSize(25,25);
    buttonQQZone->setCursor(Qt::PointingHandCursor);
    buttonQQZone->setStyleSheet("background-image:url(:/Image/focus.png)");
    nameQQZoneLayout->addStretch(3);
    //nameQQZoneLayout->addSpacing((this->width() - 4*30 - nickName->width() - buttonQQZone->width())/2);
    nameQQZoneLayout->addWidget(nickNameLabel, Qt::AlignCenter);
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
    buttonDropdown->setStyleSheet("QPushButton{border-image:url(:/Image/dropDown_small.png)}"
                                  "QPushButton:hover{background-color:#00BFFF;}"
                                  "QPushButton:pressed{background-color:#009ACD;}"
                                  "QToolTip{background-color:white;font-family:微软雅黑;}");
    buttonDropdown->setToolTip("窗口设置");
    connect(buttonDropdown, &QPushButton::clicked, this, showDropDownMenu);

    buttonMin = new QPushButton(this);
    buttonMin->setFlat(true);
    buttonMin->setFixedSize(25,25);
    buttonMin->setCursor(Qt::PointingHandCursor);
    buttonMin->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} "
                             "QPushButton:hover{background-color:#00BFFF;}"
                             "QPushButton:pressed{background-color:#009ACD;}"
                             "QToolTip{background-color:white;font-family:微软雅黑;}" );
    buttonMin->setToolTip("最小化");
    connect(buttonMin, &QPushButton::clicked, this, &converstationWindow::showMinimized);

    buttonMax = new QPushButton(this);
    buttonMax->setFlat(true);
    buttonMax->setFixedSize(25,25);
    buttonMax->setCursor(Qt::PointingHandCursor);
    buttonMax->setStyleSheet("QPushButton{border-image:url(:/Image/max.png)}"
                             "QPushButton:hover{background-color:#00BFFF;}"
                             "QPushButton:pressed{background-color:#009ACD;}"
                             "QToolTip{background-color:white;font-family:微软雅黑;}");
    buttonMax->setToolTip("最大化");
    connect(buttonMax, &QPushButton::clicked, this, &converstationWindow::showMaxWindow);

    buttonClose = new QPushButton;
    buttonClose->setFlat(true);
    buttonClose->setFixedSize(25,25);
    buttonClose->setCursor(Qt::PointingHandCursor);
    buttonClose->setAttribute(Qt::WA_TranslucentBackground);
    buttonClose->setStyleSheet("QPushButton{border-image:url(:/Image/exit.png)} "
                               "QPushButton:hover{background-color:red;}"
                               "QPushButton:pressed{background-color:#009ACD;}"
                               "QToolTip{background-color:white;font-family:微软雅黑;}");
    buttonClose->setToolTip("关闭");
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
    voiceTalkButton->setFixedSize(25,25);
    voiceTalkButton->setCursor(Qt::PointingHandCursor);
    voiceTalkButton->setStyleSheet("QPushButton{border-image:url(:/Image/call.png); border-radius:10px;"
                                   "background-color:rgb(228,237,245)}"
                                   "QPushButton::hover{background-color:rgb(205,222,236)}");
    voiceTalkButton->setToolTip("发起语音通话");

    videoTalkButton = new QPushButton(this);
    videoTalkButton->setFlat(true);
    videoTalkButton->setFixedSize(25,25);
    videoTalkButton->setCursor(Qt::PointingHandCursor);
    videoTalkButton->setStyleSheet("QPushButton{border-image:url(:/Image/videoTalk.png); border-radius:10px;"
                                   "background-color:rgb(228,237,245)}"
                                   "QPushButton::hover{background-color:rgb(205,222,236)}");
    videoTalkButton->setToolTip("发起视频通话");

    otherButton = new QPushButton(this);
    otherButton->setFlat(true);
    otherButton->setFixedSize(25,25);
    otherButton->setCursor(Qt::PointingHandCursor);
    otherButton->setStyleSheet("QPushButton{border-image:url(:/Image/more_big.png); border-radius:10px;"
                                   "background-color:rgb(228,237,245)}"
                                   "QPushButton::hover{background-color:rgb(205,222,236)}");


    mutiwayTalkLayout->addStretch(5);
    mutiwayTalkLayout->addWidget(voiceTalkButton);
    mutiwayTalkLayout->addWidget(videoTalkButton);
    mutiwayTalkLayout->addWidget(otherButton);
    mutiwayTalkLayout->setSpacing(10);
    mutiwayTalkLayout->setContentsMargins(0,12,13,3);

    conversationLayout = new QVBoxLayout;
    converstaionContent = new QTextEdit(this);
    converstaionContent->resize(this->width()-140, this->height());
    converstaionContent->setReadOnly(true);
    converstaionContent->setFontFamily("微软雅黑");
    converstaionContent->setFontPointSize(10);
    converstaionContent->setLineWidth(converstaionContent->width()- 10);
    converstaionContent->setStyleSheet("border-bottom:1px solid lightgray;"
                                       "border-left-style:none;"
                                       "border-right-style:none;"
                                       "border-top-style:none;");

    QHBoxLayout *toolBoxLayout = new QHBoxLayout;

    toolboxLabel = new QLabel(this);
    toolboxLabel->resize(this->width()-140, 22);
    toolboxLabel->setAttribute(Qt::WA_TranslucentBackground);

    expressionButton = new QPushButton(this);
    expressionButton->setFlat(true);
    expressionButton->setFixedSize(20,20);
    expressionButton->setCursor(Qt::PointingHandCursor);
    expressionButton->setStyleSheet("border-image:url(:/Image/expression.png)");

    gifButton = new QPushButton(this);
    gifButton->setFlat(true);
    gifButton->setFixedSize(20,20);
    gifButton->setCursor(Qt::PointingHandCursor);
    gifButton->setStyleSheet("border-image:url(:/Image/gif.png)");

    copyAndCutButton = new QPushButton(this);
    copyAndCutButton->setFlat(true);
    copyAndCutButton->setFixedSize(20,20);
    copyAndCutButton->setCursor(Qt::PointingHandCursor);
    copyAndCutButton->setStyleSheet("border-image:url(:/Image/screenshot.png)");

    openFileButton = new QPushButton(this);
    openFileButton->setFlat(true);
    openFileButton->setFixedSize(20,20);
    openFileButton->setCursor(Qt::PointingHandCursor);
    openFileButton->setStyleSheet("border-image:url(:/Image/folder.png)");

    openPicButton = new QPushButton(this);
    openPicButton->setFlat(true);
    openPicButton->setFixedSize(20,20);
    openPicButton->setCursor(Qt::PointingHandCursor);
    openPicButton->setStyleSheet("border-image:url(:/Image/picture.png)");

    shakeButton = new QPushButton(this);
    shakeButton->setFlat(true);
    shakeButton->setFixedSize(20,20);
    shakeButton->setCursor(Qt::PointingHandCursor);
    shakeButton->setStyleSheet("border-image:url(:/Image/shake.png)");

    QQpayButton = new QPushButton(this);
    QQpayButton->setFlat(true);
    QQpayButton->setFixedSize(20,20);
    QQpayButton->setCursor(Qt::PointingHandCursor);
    QQpayButton->setStyleSheet("border-image:url(:/Image/redpackage.png)");

    QQMusicButton = new QPushButton(this);
    QQMusicButton->setFlat(true);
    QQMusicButton->setFixedSize(20,20);
    QQMusicButton->setCursor(Qt::PointingHandCursor);
    QQMusicButton->setStyleSheet("border-image:url(:/Image/music.png)");

    moreButton = new QPushButton(this);
    moreButton->setFlat(true);
    moreButton->setFixedSize(20,20);
    moreButton->setCursor(Qt::PointingHandCursor);
    moreButton->setStyleSheet("border-image:url(:/Image/more_small.png)");

    historyRecordButton = new QPushButton(this);
    historyRecordButton->setFlat(true);
    historyRecordButton->setFixedSize(20,20);
    historyRecordButton->setCursor(Qt::PointingHandCursor);
    historyRecordButton->setStyleSheet("border-image:url(:/Image/history.png)");

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
    toolboxLabel->setLayout(toolBoxLayout);

    inputArea = new MyTextEdit(this);
    inputArea->resize(this->width()-140, this->height()-titleBar->height()
                                            -converstaionContent->height()
                                            );
    inputArea->setStyleSheet("border-style:none;");
    inputArea->setMinimumHeight(20);
    connect(inputArea, &MyTextEdit::enterKeyPressed, this, &converstationWindow::sendTextToContent);


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
    connect(closeButton , &QPushButton::clicked, this, &converstationWindow::close);

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
    connect(sendMsgButton, &QPushButton::clicked, this, &converstationWindow::sendTextToContent);

    sendMoreButton = new QPushButton(this);
    sendMoreButton->setFlat(true);
    sendMoreButton->setFixedSize(27,30);
    sendMoreButton->setCursor(Qt::PointingHandCursor);
    sendMoreButton->setStyleSheet("background-image:url(:/Image/down_hover.png);");
    connect(sendMoreButton, &QPushButton::clicked, [=]{converstaionContent->resize(converstaionContent->width(),
                                                                                   converstaionContent->height()+5);
                                                       toolboxLabel->move(toolboxLabel->pos()+QPoint(0,5));
                                                        inputArea->resize(inputArea->width(),
                                                                         inputArea->height()-5);
                                                       inputArea->move(inputArea->pos() + QPoint(0,5));});

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

    conversationLayout->addLayout(mutiwayTalkLayout, 1);
    conversationLayout->addWidget(converstaionContent, 9);
    conversationLayout->addWidget(toolboxLabel, 1);
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

void converstationWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->pos().x() > 0&& event->pos().y() > 0
            && event->pos().x() < titleBar->width()
            && event->pos().y() < titleBar->height())
    {
        showMaxWindow();
    }
}

void converstationWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed && event->buttons()==Qt::LeftButton)
    {
        move(event->pos() - MousePressPos + this->pos());
    }
}

void converstationWindow::showMaxWindow()
{
    if(this->isMaximized())
    {
        this->showNormal();
        buttonMax->setToolTip("最大化");
        buttonMax->setStyleSheet("background-image:url(:/Image/max.png)");
    }
    else
    {
        this->showMaximized();
        buttonMax->setStyleSheet("background-image:url(:/Image/restore.png)");
        buttonMax->setToolTip("还原");
    }
}

void converstationWindow::showDropDownMenu()
{
    dropdownMenu = new QMenu(this);
    dropdownMenu->addAction("保持窗口最前");
    dropdownMenu->addAction("合并会话窗口");
    dropdownMenu->addSeparator();
    dropdownMenu->addAction("启用场景秀模式");
    dropdownMenu->addAction("音视频通话设置");
    dropdownMenu->addSeparator();
    dropdownMenu->addAction("更多设置");
    dropdownMenu->setStyleSheet("QMenu {"
                                "background-color: white;"
                                "border-radius:3px;"
                                "margin:0px;"
                                "border:1px solid lightgray;}"
                            "QMenu::item { "
                             "padding-left:20px;"
                             "padding-right:20px;"
                             "padding-top:5px;"
                             "padding-bottom:5px;\
                             border-style:none; /* reserve space for selection border */}\
                            QMenu::item:selected {\
                                background: lightgray;}\
                            QMenu::separator {\
                                height: 1px;\
                                background: lightgray;\
                                margin-left: 0px;\
                                margin-right: 0px;\}");
    dropdownMenu->move(mapToGlobal(buttonDropdown->pos())+QPoint(5,40));
    dropdownMenu->setStyleSheet("font-family:微软雅黑;font-size:10;");
    dropdownMenu->show();

    QAction *action = new QAction(dropdownMenu);
    action->setText("test");
    action->setChecked(true);
    dropdownMenu->addAction(action);
    action->setCheckable(true);
    action->setChecked(true);

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


void converstationWindow::sendTextToContent()
{
    QString inputText = inputArea->toPlainText();
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString outputText = this->nickName + " " + currentTime +
            "\n    " +inputText;
    converstaionContent->append(outputText);
    inputArea->clear();
}
