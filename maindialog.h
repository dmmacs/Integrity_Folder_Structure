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
    void on_pB_BrowseButton_clicked();

    void on_pBLoad_clicked();

    void on_pBSave_clicked();

    void on_pB_ReadDirStruct_clicked();

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

};

#endif // MAINDIALOG_H
