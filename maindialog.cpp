#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"

const QString DirTagName = "dir";
const QString DirAttrName = "name";

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    StdModel = new QStandardItemModel(this);

    ui->treeView->setModel(StdModel);
    ui->treeView->expandAll();
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_pB_BrowseButton_clicked()
{
    QString sFname = QFileDialog::getOpenFileName(this,tr("Open File"),directory.path(),tr("Files(*.*)"));

    qDebug() << sFname;
    ui->textEdit->setText(sFname);

}

void MainDialog::ParseXMLFile(QString sPath, QDomDocument *xmlDoc)
{
    QFile file(sPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        ui->textEdit->setText("Failed to open file");
    }
    else
    {
        if(!xmlDoc->setContent(&file))
        {
            qDebug() << "Failed to load document";
            ui->textEdit->setText("Failed to load document");
        }
        file.close();
    }

}

void MainDialog::SaveXMLFile(QString sPath, QDomDocument *xmlDoc)
{
    QFile file(sPath);

    qDebug() << sPath;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        ui->textEdit->setText("Failed to open file");
    }
    else
    {
        QTextStream TextStream(&file);
        xmlDoc->save(TextStream,0);
        file.close();
    }
}

void MainDialog::ProcessXMLData(QDomElement *rootxml, QStandardItem *rootNode, QString tabStr)
{
    QStandardItem *tmpNode;
    QDomElement nextItem;
    QDomElement childItem;

    nextItem = *rootxml;
    while (nextItem.isNull() == FALSE)
    {
        qDebug() << tabStr << nextItem.attribute(DirAttrName);
        tmpNode = new QStandardItem(nextItem.attribute(DirAttrName));
        rootNode->appendRow(tmpNode);

        if (nextItem.hasChildNodes())
        {
            childItem = nextItem.firstChildElement();
            QString tmp;
            tmp = tabStr + "  ";
            ProcessXMLData(&childItem, tmpNode, tmp);
        }
        nextItem = nextItem.nextSiblingElement(DirTagName);
    }

}

void MainDialog::on_pBLoad_clicked()
{
    if (ui->textEdit->toPlainText() != "")
    {
        // Clear the Tree
        StdModel->clear();

        ParseXMLFile("C:/Users/dmmacs/Documents/Qt Projects/QFileSystemModel/tmp.xml", &xmldoc);

        // Get Root of Tree and XML
        QStandardItem *Node = StdModel->invisibleRootItem();
        QDomElement root = xmldoc.firstChildElement(DirTagName);

        // Add the Root node
        qDebug() << root.attribute(DirAttrName);

        // Add the node
        QStandardItem *tmpNode;
        tmpNode = new QStandardItem(root.attribute(DirAttrName));
        Node->appendRow(tmpNode);

        root = root.firstChildElement(DirTagName);

        ProcessXMLData(&root, tmpNode, "  ");
        ui->treeView->expandAll();

    }

}

void MainDialog::on_pBSave_clicked()
{
    if (ui->textEdit->toPlainText() != "")
    {
        SaveXMLFile(ui->textEdit->toPlainText(), &xmldoc);
    }
}

void MainDialog::on_pB_ReadDirStruct_clicked()
{
    mksfoldertemplate dlg;

    dlg.exec();
    //mksfoldertemplate();
}
