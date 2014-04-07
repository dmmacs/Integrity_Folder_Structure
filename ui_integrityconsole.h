/********************************************************************************
** Form generated from reading UI file 'integrityconsole.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTEGRITYCONSOLE_H
#define UI_INTEGRITYCONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_IntegrityConsole
{
public:
    QTextEdit *Commands;

    void setupUi(QDialog *IntegrityConsole)
    {
        if (IntegrityConsole->objectName().isEmpty())
            IntegrityConsole->setObjectName(QStringLiteral("IntegrityConsole"));
        IntegrityConsole->resize(565, 753);
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        IntegrityConsole->setFont(font);
        Commands = new QTextEdit(IntegrityConsole);
        Commands->setObjectName(QStringLiteral("Commands"));
        Commands->setGeometry(QRect(10, 10, 541, 731));

        retranslateUi(IntegrityConsole);

        QMetaObject::connectSlotsByName(IntegrityConsole);
    } // setupUi

    void retranslateUi(QDialog *IntegrityConsole)
    {
        IntegrityConsole->setWindowTitle(QApplication::translate("IntegrityConsole", "Console", 0));
    } // retranslateUi

};

namespace Ui {
    class IntegrityConsole: public Ui_IntegrityConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTEGRITYCONSOLE_H
