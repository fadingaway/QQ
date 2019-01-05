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
class converstationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit converstationWindow(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    QPixmap getRoundImage(const QPixmap &src, int radius);
signals:

public slots:
    void showMaxWindow();
    void showDropDownMenu();
private:
    QPushButton *buttonDropdown;
    QPushButton *buttonMin;
    QPushButton *buttonMax;
    QPushButton *buttonClose;
    bool isMousePressed = false;
    QPoint MousePressPos;
    QLabel *titleBar;
    QLabel *nameAndQQZone;
    QLabel *nickName;
    QPushButton *buttonQQZone;

    QPushButton *voiceTalkButton;
    QPushButton *videoTalkButton;
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

    QTextEdit *inputArea;
    QLabel *bottomLabel;
    QPushButton *closeButton;
    QPushButton *sendButton;
    QPushButton *sendMsgButton;
    QLabel *seperator;
    QPushButton *sendMoreButton;

    QLabel *QQshow;
    QVBoxLayout *conversationLayout;

    QMenu *dropdownMenu;
};

#endif // CONVERSTATIONWINDOW_H
