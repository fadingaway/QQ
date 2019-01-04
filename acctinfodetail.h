#ifndef ACCTINFODETAIL_H
#define ACCTINFODETAIL_H

#include "QLabel"
#include "QPushButton"
#include "QListWidgetItem"
#include <QWidget>

class AcctInfoDetail : public QWidget
{
    Q_OBJECT
public:
    explicit AcctInfoDetail(QWidget *parent = Q_NULLPTR);
    ~AcctInfoDetail();
    void setDetailInfo(const QString &nickName, const QString &acctNumber, const QString &iconFilePath);
    QPixmap getRoundImage(const QPixmap &src);
    void onDelButtonClicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void onClicked();
    void setDelButtonStatus(bool hide);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void removeAcct(QString nickName, QString AcctNo, QString filePath);
    void showAcctInfo(QString nickName, QString AcctNo, QString filePath);
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
    bool isMousePressed = false;
};

#endif // ACCTINFODETAIL_H
