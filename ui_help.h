/********************************************************************************
** Form generated from reading UI file 'help.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Help
{
public:
    QTextBrowser *textBrowser;
    QCheckBox *xml_checkBox;
    QCheckBox *xml_checkBox_2;
    QCheckBox *checkBox;

    void setupUi(QDialog *Help)
    {
        if (Help->objectName().isEmpty())
            Help->setObjectName(QStringLiteral("Help"));
        Help->resize(580, 780);
        textBrowser = new QTextBrowser(Help);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 10, 541, 721));
        xml_checkBox = new QCheckBox(Help);
        xml_checkBox->setObjectName(QStringLiteral("xml_checkBox"));
        xml_checkBox->setGeometry(QRect(100, 740, 81, 20));
        xml_checkBox_2 = new QCheckBox(Help);
        xml_checkBox_2->setObjectName(QStringLiteral("xml_checkBox_2"));
        xml_checkBox_2->setGeometry(QRect(10, 740, 81, 20));
        checkBox = new QCheckBox(Help);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(190, 740, 131, 20));

        retranslateUi(Help);

        QMetaObject::connectSlotsByName(Help);
    } // setupUi

    void retranslateUi(QDialog *Help)
    {
        Help->setWindowTitle(QApplication::translate("Help", "Dialog", 0));
        xml_checkBox->setText(QApplication::translate("Help", "Save xml", 0));
        xml_checkBox_2->setText(QApplication::translate("Help", "Load xml", 0));
        checkBox->setText(QApplication::translate("Help", "Read Dir Structure", 0));
    } // retranslateUi

};

namespace Ui {
    class Help: public Ui_Help {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
