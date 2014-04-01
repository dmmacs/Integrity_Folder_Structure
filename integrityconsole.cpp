#include "integrityconsole.h"
#include "ui_integrityconsole.h"

IntegrityConsole::IntegrityConsole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntegrityConsole)
{
    ui->setupUi(this);
}

IntegrityConsole::~IntegrityConsole()
{
    delete ui;
}

static const char * tabs[] =
{
    "",
    "    ",
    "        ",
    "            ",
    "                ",
    "                    ",
    "                        ",
    "                            ",
    "                                ",
    "                                    "
};

void IntegrityConsole::AddText(QString Str, int tabCnt)
{
    QString curStr = ui->Commands->toPlainText();

    QRegExp regExp("[\r\n]");
    QStringList lines;
    lines = Str.split(regExp,QString::KeepEmptyParts);

    for (int i = 0; i < lines.count(); i++)
    {
        QString item = lines.at(i);
        curStr += tabs[tabCnt];
        curStr += item;
        curStr += "\n";
    }

    ui->Commands->setText(curStr);

//    ui->Commands->setText(ui->Commands->toPlainText() + Str + "\n");
}

