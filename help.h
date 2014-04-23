#ifndef HELP_H
#define HELP_H

#include <QDialog>

namespace Ui {
class Help;
}

class Help : public QDialog
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

    QString XML_help;
    QString Read_DIR_Help;
    QString Integrity_Help;

private slots:
    void on_xml_checkBox_clicked();

    void on_xml_RadioBtn_clicked();

private:
    Ui::Help *ui;
};

#endif // HELP_H
