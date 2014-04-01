/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include "mylineeditclass.h"

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QTreeView *treeView;
    QPushButton *pBLoad;
    QPushButton *pBSave;
    QPushButton *pB_ReadDirStruct;
    QProgressBar *prgBar;
    MyLineEditClass *lineEdit;
    QLineEdit *textEdit;
    QLabel *label;
    QPushButton *m_pMKSGenButton;
    QToolButton *toolButton;
    QLabel *label_2;
    MyLineEditClass *mks_serveredit;
    QLabel *label_3;
    MyLineEditClass *mks_portedit;
    QLabel *label_4;
    MyLineEditClass *changePackageEdit;
    QTextEdit *Commands;
    QLabel *label_5;
    QCheckBox *ConsoleCheck;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(681, 620);
        QIcon icon;
        icon.addFile(QStringLiteral("../../../../Desktop/temp/Trw_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainDialog->setWindowIcon(icon);
        treeView = new QTreeView(MainDialog);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(10, 40, 431, 511));
        pBLoad = new QPushButton(MainDialog);
        pBLoad->setObjectName(QStringLiteral("pBLoad"));
        pBLoad->setGeometry(QRect(6, 560, 91, 23));
        pBSave = new QPushButton(MainDialog);
        pBSave->setObjectName(QStringLiteral("pBSave"));
        pBSave->setGeometry(QRect(106, 560, 91, 23));
        pB_ReadDirStruct = new QPushButton(MainDialog);
        pB_ReadDirStruct->setObjectName(QStringLiteral("pB_ReadDirStruct"));
        pB_ReadDirStruct->setGeometry(QRect(340, 560, 101, 23));
        prgBar = new QProgressBar(MainDialog);
        prgBar->setObjectName(QStringLiteral("prgBar"));
        prgBar->setGeometry(QRect(10, 590, 431, 23));
        prgBar->setValue(24);
        lineEdit = new MyLineEditClass(MainDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(450, 230, 221, 22));
        textEdit = new QLineEdit(MainDialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(10, 10, 401, 22));
        label = new QLabel(MainDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(450, 210, 221, 16));
        m_pMKSGenButton = new QPushButton(MainDialog);
        m_pMKSGenButton->setObjectName(QStringLiteral("m_pMKSGenButton"));
        m_pMKSGenButton->setGeometry(QRect(460, 270, 93, 28));
        QFont font;
        font.setPointSize(6);
        m_pMKSGenButton->setFont(font);
        toolButton = new QToolButton(MainDialog);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(410, 10, 27, 22));
        label_2 = new QLabel(MainDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(450, 40, 221, 16));
        mks_serveredit = new MyLineEditClass(MainDialog);
        mks_serveredit->setObjectName(QStringLiteral("mks_serveredit"));
        mks_serveredit->setGeometry(QRect(450, 60, 221, 22));
        label_3 = new QLabel(MainDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(450, 90, 221, 16));
        mks_portedit = new MyLineEditClass(MainDialog);
        mks_portedit->setObjectName(QStringLiteral("mks_portedit"));
        mks_portedit->setGeometry(QRect(450, 110, 221, 22));
        label_4 = new QLabel(MainDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(450, 140, 221, 16));
        changePackageEdit = new MyLineEditClass(MainDialog);
        changePackageEdit->setObjectName(QStringLiteral("changePackageEdit"));
        changePackageEdit->setGeometry(QRect(450, 160, 221, 22));
        Commands = new QTextEdit(MainDialog);
        Commands->setObjectName(QStringLiteral("Commands"));
        Commands->setGeometry(QRect(680, 10, 565, 601));
        Commands->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(MainDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(450, 590, 231, 16));
        ConsoleCheck = new QCheckBox(MainDialog);
        ConsoleCheck->setObjectName(QStringLiteral("ConsoleCheck"));
        ConsoleCheck->setGeometry(QRect(470, 10, 121, 20));
        QWidget::setTabOrder(treeView, pBLoad);
        QWidget::setTabOrder(pBLoad, pBSave);

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", 0));
        pBLoad->setText(QApplication::translate("MainDialog", "Load XML File", 0));
        pBSave->setText(QApplication::translate("MainDialog", "Save XML File", 0));
        pB_ReadDirStruct->setText(QApplication::translate("MainDialog", "Read Dir Struct", 0));
        lineEdit->setText(QString());
        label->setText(QApplication::translate("MainDialog", "Base Project", 0));
        m_pMKSGenButton->setText(QApplication::translate("MainDialog", "Create Prj in MKS", 0));
        toolButton->setText(QApplication::translate("MainDialog", "...", 0));
        label_2->setText(QApplication::translate("MainDialog", "MKS Server", 0));
        mks_serveredit->setText(QString());
        label_3->setText(QApplication::translate("MainDialog", "MKS Port", 0));
        mks_portedit->setText(QString());
        label_4->setText(QApplication::translate("MainDialog", "Change Package", 0));
        changePackageEdit->setText(QString());
        label_5->setText(QApplication::translate("MainDialog", "Version 0.3", 0));
        ConsoleCheck->setText(QApplication::translate("MainDialog", "Show Console", 0));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
