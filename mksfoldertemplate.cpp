#include "mksfoldertemplate.h"
#include "ui_mksfoldertemplate.h"

mksfoldertemplate::mksfoldertemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mksfoldertemplate)
{
    ui->setupUi(this);

    dirmodel = new QFileSystemModel(this);

    QString sPath = "C:/Users/dmmacs/Documents/Qt Projects/tmp_root";

    dirmodel->setRootPath(sPath);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setReadOnly(true);


    ui->treeView->setModel(dirmodel);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
    //ui->treeView->columnWidth(0)
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    ui->m_dirPath->setText(sPath);
}

mksfoldertemplate::~mksfoldertemplate()
{
    delete ui;
}
