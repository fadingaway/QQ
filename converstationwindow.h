#ifndef CONVERSTATIONWINDOW_H
#define CONVERSTATIONWINDOW_H

#include <QWidget>
#include "QPushButton"
#include "QLabel"
#include "QPoint"
#include "QLineEdit"
#include "QComboBox"
#include "QTextEdit"
#include "QVBoxLayout"
#include "mytextedit.h"
#include "QPlainTextEdit"

class MyTextEdit;
class converstationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit converstationWindow(QWidget *parent,
                                 QString acctNo,
                                 QString nickName,
                                 QString friendAcctNo,
                                 QString friendNickName);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    QPixmap getRoundImage(const QPixmap &src, int radius);
    void enlargeWindow(int px);
    void sendTextToContent();
    bool eventFilter(QObject *watched, QEvent *event);
    void openFile();
    void openFolder();
signals:

public slots:
    void showMaxWindow();
    void showDropDownMenu();
    void showHideFunction();
    void showOpenFileMenu();
private:
    QString nickName;
    QString acctNo;
    QString friendAcctNo;
    QString friendNickName;

    QPushButton *buttonDropdown;
    QPushButton *buttonMin;
    QPushButton *buttonMax;
    QPushButton *buttonClose;
    bool isMousePressed = false;
    QPoint MousePressPos;
    QLabel *titleBar;
    QLabel *nameAndQQZone;
    QLabel *nickNameLabel;
    QPushButton *buttonQQZone;

    QLabel *talkWayContainter;
    QPushButton *voiceTalkButton;
    QPushButton *videoTalkButton;
    QPushButton *screenShareButton;
    QPushButton *screenControlButton;
    QPushButton *AppPackageButton;
    QPushButton *otherButton;

    QTextEdit *converstaionContent;
    QLabel *toolboxLabel;
    QPushButton *expressionButton;
    QPushButton *gifButton;
    QPushButton *copyAndCutButton;
    QPushButton *openFileButton;
    QPushButton *openPicButton;
    QPushButton *shakeButton;
    QPushButton *QQpayButton;
    QPushButton *QQMusicButton;
    QPushButton *moreButton;
    QPushButton *historyRecordButton;

    MyTextEdit *inputArea;
    QLabel *bottomLabel;
    QPushButton *closeButton;
    QPushButton *sendButton;
    QPushButton *sendMsgButton;
    QLabel *seperator;
    QPushButton *sendMoreButton;

    QLabel *QQshow;
    QVBoxLayout *conversationLayout;

    QMenu *dropdownMenu;
    bool isResizeInd = false;
    bool isPackageShowed = false;
    QTimer *clock;
};

#endif // CONVERSTATIONWINDOW_H
