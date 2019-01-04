#ifndef NEWFRIENDITEM_H
#define NEWFRIENDITEM_H

#include <QWidget>
#include "QLabel"
#include "QPushButton"

class newFriendItem : public QWidget
{
    Q_OBJECT
public:
    explicit newFriendItem(QWidget *parent,
                           QString userAcctId,
                           QString userIconPath,
                           QString userName,
                           int totalCommenFriends);
    QPixmap getRoundImage(const QPixmap &src);
    QString getElidedText(QFont font, QString str, int MaxWidth);
    QString getUserAcctId();
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void requestToAddFriend(QString acctId);
    void requestToDeleteItem(QString acctId);
public slots:
    void addNewFriend();
    void deleteItem();
    void setDelButtonVisible(bool visible);
private:
    QLabel *userIconLabel;
    QLabel *userNameLabel;
    QLabel *text;
    QPushButton *addFriend;
    QString userAcctId;
    QPushButton *deleteItemButton;
};

#endif // NEWFRIENDITEM_H
