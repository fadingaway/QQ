#ifndef ACCTINFOITEM_H
#define ACCTINFOITEM_H

#include "QLabel"
#include "QPushButton"
#include "QListWidgetItem"
#include <QWidget>

class AcctInfoItem : public QWidget
{
    Q_OBJECT
public:
    explicit AcctInfoItem(QWidget *parent = Q_NULLPTR);
    ~AcctInfoItem();
    void setDetailInfo(int index, const QString &iconFilePath, const QString &nickName, const QString &acctNumber);
    QPixmap getRoundImage(const QPixmap &src);
    void onDelButtonClicked();
signals:
    void removeAcct(int index, QString nickName, QString AcctNo, QString filePath);

private:
    int index;
    QLabel *icon;
    QPushButton *del;
    QLabel *nickName;
    QLabel *acctNo;
    QString iconFilePath;
    QString nickNameString;
    QString acctNoString;
    QString defaultIconFilePath = ":/Image/QQ.jpg";
};

#endif // ACCTINFOITEM_H
