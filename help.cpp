#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);

    XML_help = "This is the XML Help";



    Read_DIR_Help = "This is the Read Directory Structure Help";
    Integrity_Help = "This is the Integrity function help";

}

Help::~Help()
{
    delete ui;
}

void Help::on_xml_checkBox_clicked()
{

}

void Help::on_xml_RadioBtn_clicked()
{
//    if (ui->xml_RadioBtn->isChecked())
//    {
//        ui->textBrowser->setText(XML_help);
//    }
}
