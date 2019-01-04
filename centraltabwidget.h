#ifndef CENTRALTABWIDGET_H
#define CENTRALTABWIDGET_H

#include <QWidget>
#include "QTabWidget"

class MsgTab;
class ContactTab;
class NewsTab;


class centralTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit centralTabWidget(QWidget *parent = 0);
signals:
    void tabChanged(int tabIndex);
public slots:
public:
    QTabWidget *tabWidget;
    MsgTab *message;
    ContactTab *contact;
    NewsTab *news;
};

#endif // CENTRALTABWIDGET_H
