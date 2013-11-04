#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QDir>
#include <QtCore>
#include <QtGui>
#include <QStandardItem>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QFileDialog>
#include <QLineEdit>
#include <QInputDialog>

namespace Ui {
class MainDialog;
}



class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_pBLoad_clicked();

    void on_pBSave_clicked();

    void on_pB_ReadDirStruct_clicked();

    void on_lineEdit_selectionChanged();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_toolButton_clicked();


    void on_m_pMKSGenButton_clicked();

private:
    Ui::MainDialog *ui;
    QStandardItemModel *StdModel;
    QDomDocument xmldoc;
    QDir directory;

    void ParseXMLFile(QString sPath, QDomDocument *xmlDoc);
    void SaveXMLFile(QString sPath, QDomDocument *xmlDoc);
    void ProcessXMLData(QDomElement *rootxml, QStandardItem *rootNode, QString tabStr);
    void ReadDir(QString Path, QString *nodeData);
    void LoadXMLData(QString sFname);


    bool matchProcessName(DWORD processID, std::string processName);

};




#endif // MAINDIALOG_H
