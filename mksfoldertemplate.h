#ifndef MKSFOLDERTEMPLATE_H
#define MKSFOLDERTEMPLATE_H

#include <QDialog>
#include <QDir>
#include <QtCore>
#include <QtGui>
#include <QDirModel>
#include <QStandardItem>
#include <QTreeView>
#include <QFileSystemModel>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>


namespace Ui {
class mksfoldertemplate;
}

class mksfoldertemplate : public QDialog
{
    Q_OBJECT

public:
    explicit mksfoldertemplate(QWidget *parent = 0);
    ~mksfoldertemplate();

private:
    Ui::mksfoldertemplate *ui;

    QFileSystemModel *dirmodel;

};

#endif // MKSFOLDERTEMPLATE_H
