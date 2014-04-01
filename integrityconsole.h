#ifndef INTEGRITYCONSOLE_H
#define INTEGRITYCONSOLE_H

#include <QDialog>

namespace Ui {
class IntegrityConsole;
}

class IntegrityConsole : public QDialog
{
    Q_OBJECT

public:
    explicit IntegrityConsole(QWidget *parent = 0);
    ~IntegrityConsole();

    void AddText(QString Str, int tabCnt);
private:
    Ui::IntegrityConsole *ui;
};

#endif // INTEGRITYCONSOLE_H
