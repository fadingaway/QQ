#include "widget.h"
#include "networkwindow.h"
#include <QApplication>
#include "QStyleFactory"
#include "qqmainwindow.h"
#include "incomingmsgitem.h"
#include "msgtab.h"
#include "newfrienditem.h"
#include "acctinfodetail.h"
#include "classifyitem.h"
#include "contactitem.h"
#include "contacttab.h"
#include "converstationwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    newFriendItem w(nullptr,
//                  "12345",
//                  ":/Image/head_1.jpg",
//                  "370",
//                  40);
//    w.show();


//    AcctInfoDetail y;
//    y.setDetailInfo("abc","123456", ":/Image/head_1.jpg");
//    y.show();

//    classifyItem x(nullptr, 123,"New", QListView::LeftToRight,370);
//    x.show();

//    ContactItem item(nullptr, 370,
//                     ":/Image/head_1.jpg",
//                     "a",
//                     2,
//                     "helloworld",
//                     2);
//    item.show();


//    ContactTab w(nullptr, 370, 800);
//    w.show();

    converstationWindow w;
    w.show();

    return a.exec();
}
