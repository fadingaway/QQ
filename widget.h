#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QPushButton"
#include "QMenu"
#include "mylabel.h"
#include "QPropertyAnimation"
#include "QListWidget"
#include "QMessageBox"
#include "QPixmap"
#include "QTcpSocket"
#include "loginfailwindow.h"

class QTcpSocket;
class LoginFailWindow;
namespace Ui {
class Widget;
}

// 登录状态;
typedef enum
{
    ONLINE = 1,     //在线;
    ACTIVE = 2,         //活跃;
    AWAY = 3,           //离开;
    BUSY = 4,           //忙碌;
    NOT_DISTURB = 5,    //请勿打扰;
    HIDE = 6,           //隐身;
    OFFLINE = 7         //离线;
}LoginStatus;

struct acctInfo
{
    QString IconFilePath;
    QString userName;
    QString userAcctNumber;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *e);
    void paintEvent(QPaintEvent *event);
    void initTitle();
    void initUserheadPic();
    void initUI();
    void initErrorMsgLabel();
    void createStatusMenu();
    void readSetting();
    void activateSettingWindow();
    void setLastUserHeadIcon();
    void setLastUserAcctNo();
    void showKeyboard();
    void login();
    void setLoginStatus(QAction *);
    QPixmap PixmapToRound(QPixmap &src, int radius);
    QPixmap getRoundImage(const QPixmap &src, int radius);
    QPixmap getRoundHoverImage(QPixmap &src, int radius);
    bool eventFilter(QObject *watched, QEvent *event);
    void minWindow();
    void changeTextSize();
    void onMoreAcctClicked();
    void showDropDownAcctList();
    void setChoosedAcctInfo(QString nickName, QString AcctNo, QString filePath);
    void removeAcctFromDropList(QString nickName, QString AcctNo, QString filePath);
    void activeDelButton(QListWidgetItem *item);
    void loginByFlicker();
    void activeMainWindow(QPoint point);
    void startLoginWindow();
    void startErrorWindow();
    void showNetworkErrorInfo(QString error);
    void sendData();
    void startMainWindow();
public slots:
    void onRotateFinished();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
signals:
    void click();
private:
    Ui::Widget *ui;
    QPushButton *buttonKeyBoard;
    QPixmap lastUserHeadIcon;
    QString lastUserAcctNo;
    QString lastUserPassWord;
    LoginStatus lastLoginStatus;
    bool isRemPWChecked = false;
    bool isAutoLoginChecked = false;
    QMenu *loginStatusMenu;
    QAction *headIconAction;
    QAction *pwAction;
    QPushButton *buttonAcctList;
    QPoint mousePressPoint;
    bool isMousePressed = false;
    MyLabel *userHeadIcon;
    MyLabel *moreAcctIcon;
    MyLabel *loginStatusIcon;
    QPropertyAnimation *animation;
    bool isMoreAcctTriggered = false;
    bool isAcctListViewCreated = false;

    QAction *actionOnline;
    QAction *actionActive;
    QAction *actionAway;
    QAction *actionBusy;
    QAction *actionNoDisturb;
    QAction *actionHide;
    QListWidget *acctListView;

    QList<acctInfo> userAcctList;
    MyLabel *twoDimensionCodePic;
    MyLabel *twoDimensioncodeText;
    QPushButton *backButton;
    QPixmap rotatePic;
    QLabel *mainWidget;
    bool isRotating = false;
    bool isToShowNetworkWindow = false;

    QTcpSocket *tcpSocket;
    quint16 blockSize;
    QString message;
    QPushButton *buttonCancelLogin;
    QLabel *logining;
    QLabel *errorMsg;
    QPushButton *buttonHide;
    bool isErrorMsgLabelCreated = false;
    QLabel *errorInfo;
    QLabel *errorCode;

    LoginFailWindow *loginFailwind;
    bool isLoginFailWindowActive = false;
};

#endif // WIDGET_H
