#include "maindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
#if 0
    void MainDialog::centerWidget(QWidget *w, bool useSizeHint)
    {
        if(w->isFullScreen())
            return;

        QSize size;
        if(useSizeHint)
            size = w->sizeHint();
        else
            size = w->size();

        QDesktopWidget *d = QApplication::desktop();
        int ws = d->width();   // returns screen width
        int h = d->height();  // returns screen height
        int mw = size.width();
        int mh = size.height();
        int cw = (ws/2) - (mw/2);
        int ch = (h/2) - (mh/2);
        w->move(cw,ch);
    }
#endif

    QDesktopWidget *d = QApplication::desktop();

    QRect mainScreenSize = d->availableGeometry(d->primaryScreen());
    int left = mainScreenSize.width() / 8;
    int top = mainScreenSize.height() / 2;
    top -= w.height() / 2;
    w.move(left, top);
    w.show();

    return a.exec();
}
