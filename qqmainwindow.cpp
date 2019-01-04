#include "qqmainwindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QLabel"
#include "mylabel.h"
#include "QPushButton"
#include "QLineEdit"
#include "QAction"
#include "QPainter"
QQMainWindow::QQMainWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setMinimumSize(400,700);
    readSetting();
    initTitle();
}

void QQMainWindow::readSetting()
{
    userHeadPic = QPixmap("");
    defaultUserHeadPic = QPixmap("");
    nickName = QString("一切随风");
    signatureText = QString("fadingaway");
}

void QQMainWindow::initTitle()
{
    headerLabel = new QLabel(this);
    headerLabel->setFixedSize(400,200);
    headerLabel->setStyleSheet("background-image:url()");
    QVBoxLayout *headerLayout = new QVBoxLayout;

    //title ---------start-------------
    titleLabel = new QLabel(headerLabel);
    QHBoxLayout *titleLayout = new QHBoxLayout;

    QLabel *QQIcon = new QLabel(headerLabel);
    QQIcon->setFixedSize(16,16);
    QQIcon->setPixmap(QPixmap(":/Image/d.jpg"));

    QPushButton *medalIcon = new QPushButton(headerLabel);
    medalIcon->setFixedSize(16,16);
    medalIcon->setAttribute(Qt::WA_TranslucentBackground);
    medalIcon->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *skinIcon = new QPushButton(headerLabel);
    skinIcon->setFixedSize(16,16);
    skinIcon->setAttribute(Qt::WA_TranslucentBackground);
    skinIcon->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *QQZone = new QPushButton(headerLabel);
    QQZone->setFixedSize(16,16);
    QQZone->setAttribute(Qt::WA_TranslucentBackground);
    QQZone->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *QQNews = new QPushButton(headerLabel);
    QQNews->setFixedSize(16,16);
    QQNews->setAttribute(Qt::WA_TranslucentBackground);
    QQNews->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *plusButton = new QPushButton(headerLabel);
    plusButton->setFixedSize(16,16);
    plusButton->setAttribute(Qt::WA_TranslucentBackground);
    plusButton->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *minButton = new QPushButton(headerLabel);
    minButton->setFixedSize(16,16);
    minButton->setAttribute(Qt::WA_TranslucentBackground);
    minButton->setStyleSheet("background-image:url();border-sytle:none;");

    QPushButton *closeButton = new QPushButton(headerLabel);
    closeButton->setFixedSize(16,16);
    closeButton->setAttribute(Qt::WA_TranslucentBackground);
    closeButton->setStyleSheet("background-image:url();border-sytle:none;");

    titleLayout->addWidget(QQIcon, Qt::AlignLeading);
    titleLayout->addSpacing(100);
    titleLayout->addWidget(medalIcon, Qt::AlignRight);
    titleLayout->addWidget(skinIcon, Qt::AlignRight);
    titleLayout->addWidget(QQZone, Qt::AlignRight);
    titleLayout->addWidget(QQNews, Qt::AlignRight);
    titleLayout->addWidget(plusButton, Qt::AlignRight);
    titleLayout->addWidget(minButton, Qt::AlignRight);
    titleLayout->addWidget(closeButton, Qt::AlignRight);
    titleLayout->setContentsMargins(5,5,5,5);
    titleLayout->setSpacing(0);
    titleLayout->setStretch(0,0);
    titleLabel->setLayout(titleLayout);
    //title ---------end-------------

    //user info --------------start-----------------
    QHBoxLayout *userInfoLayout = new QHBoxLayout;
    MyLabel *headerPicLabel =  new MyLabel;
    headerPicLabel->setFixedSize(65,65);
    headerPicLabel->setAttribute(Qt::WA_TranslucentBackground);
    QPixmap headPic("");
    headPic = getRoundImage(headPic, 30);
    headerPicLabel->setPixmap(headPic);

    QVBoxLayout *nickNameLayout = new QVBoxLayout;
    QHBoxLayout *nickNameInfoLayout = new QHBoxLayout;
    MyLabel *nickNameLabel = new MyLabel;
    nickNameLabel->setText(nickName);
    nickNameLabel->setMinimumWidth(nickNameLabel->fontMetrics().lineWidth());
    nickNameLabel->setStyleSheet("font-family:微软雅黑;font-size:12pt;");

    QLabel *level = new QLabel;
    level->setFixedSize(8,8);
    level->setPixmap(QPixmap(":/Image/d.jpg"));

    QLabel *vipLevel = new QLabel;
    vipLevel->setFixedSize(8,8);
    vipLevel->setPixmap(QPixmap(":/Image/d.jpg"));
    nickNameInfoLayout->addWidget(nickNameLabel, Qt::AlignLeft);
    nickNameInfoLayout->addWidget(level);
    nickNameInfoLayout->addWidget(vipLevel);
    nickNameInfoLayout->setContentsMargins(3,2,3,2);
    nickNameInfoLayout->setSpacing(0);

    MyLabel *signature = new MyLabel;
    signature->setText(signatureText);
    signature->setAttribute(Qt::WA_TranslucentBackground);
    signature->setFixedSize(150,20);
    signature->setStyleSheet("font-family:微软雅黑;font-size:8pt;");
    nickNameLayout->addLayout(nickNameInfoLayout);
    nickNameLayout->addWidget(signature);
    nickNameLayout->setContentsMargins(5,2,5,2);
    nickNameLayout->setSpacing(0);

    QPushButton *weatherButton = new QPushButton;
    weatherButton->setAttribute(Qt::WA_TranslucentBackground);
    weatherButton->setStyleSheet("border-style:none;background-image:url();");

    userInfoLayout->addWidget(headerPicLabel, Qt::AlignLeading);
    userInfoLayout->addLayout(nickNameLayout);
    userInfoLayout->addWidget(weatherButton);
    //user info --------------end-----------------

    //search component -----------start--------------
    QLineEdit *search = new QLineEdit;
    QAction *searchIconAction = new QAction(this);
    searchIconAction->setIcon(QIcon(QPixmap("")));
    search->addAction(searchIconAction, QLineEdit::LeadingPosition);
    search->setStyleSheet("border-style:none;font-family:微软雅黑;font-size:8pt;");
    search->setPlaceholderText("搜索");
    //search component -----------end--------------

    headerLayout->addWidget(headerLabel);
    headerLayout->addLayout(userInfoLayout);
    headerLayout->addWidget(search);
    headerLabel->setLayout(headerLayout);
}

QPixmap QQMainWindow::getRoundImage(const QPixmap &src, int radius)
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
    pen.setColor(QColor(174,217,248));
    painter.setPen(pen);
    painter.drawEllipse(0,0,2*radius+4, 2*radius+4);

    return pixmap;
}
