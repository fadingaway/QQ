#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QWidget>
#include "QLabel"
#include "QImage"
#include "QPixmap"
#include "QHBoxLayout"
#include "QMenu"

class ContactItem : public QWidget
{
    Q_OBJECT
public:
    explicit ContactItem(QWidget *parent,
                         int width,
                         QString userIconPath,
                         QString userName,
                         int userStatus,
                         QString userSignature,
                         int layout_mode);

    QPixmap getRoundImage(const QPixmap &src, int radius);
    QString getElidedText(QFont font, QString str, int MaxWidth);
    int getLayoutMode();
    QString getUserName();
    QString getUserAcctNo();
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:

public slots:
    void refreshUserStatus(int userStatus);
    void refreshUserName(QString userName);
    void refreshUserSignature(QString userSignature);
    void refreshUserOnline(bool isOnline);
    void refreshUserIcon(QString userIconPath);
    void setMiniStatusIcon(int status);
    QImage toGray(QImage image);
    void setFontSize(int fontSize);
    void calculateSignatureLengh(int mode);
    void setLayoutMode(int layoutMode);
    void createContactMenu();
    void updateMenuStatus();
    void createConversationWindow(QString userAcctNo);
//    void createEmailWindow(QString userAcctNo);
//    void createUserDetailInfoView(QString userAcctNo);
//    void createShareNameCardWindow(QString userAcctNo);
//    void viewMsgRecord();
//    void authoritySetting();
//    void updateRemarkName(QString remarkName);
//    void MoveContactTo(QString groupId);
//    void deleteFriend();
//    void reportUser();
//    void friendManagement();
//    void vipSwiftfunction();
//    void openQQZone();
private:
    QLabel *userIcon;
    QLabel *userNameLabel;
    QLabel *userSignatureLabel;
    QLabel *miniStatusIcon;
    QString userName;
    QString userAcctNo;
    int userStatus;
    int height;
    QString userSignature;
    QString userIconPath;
    bool isOnline = false;
    int layout_mode = 1;
    QHBoxLayout *mainLayout_mode_2;
    QHBoxLayout *mainLayout_mode_1;
    QMenu *contactItemMenu;
    bool isMenuCreated = false;
    enum
    {
        LOGIN_BY_MOBILE = 1,
        LOGIN_BY_PC = 2,
        LOGIN_BY_IPHONE = 3,
        OFFLINE = 4
    };
};

#endif // CONTACTITEM_H
