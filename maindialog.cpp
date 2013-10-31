#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"

const QString DirTagName = "dir";
const QString DirAttrName = "name";

const QString XMLHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    StdModel = new QStandardItemModel(this);

    ui->treeView->setModel(StdModel);
    ui->treeView->expandAll();
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
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

    if (sFname != "")
    {
        LoadXMLData(sFname);
    }

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

void MainDialog::LoadXMLData(QString sFname)
{
    // Clear the Tree
    StdModel->clear();

    ParseXMLFile(sFname, &xmldoc);

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

void MainDialog::on_pBLoad_clicked()
{
    if (ui->textEdit->toPlainText() != "")
    {
        LoadXMLData(ui->textEdit->toPlainText());
    }

}

void MainDialog::on_pBSave_clicked()
{
    if (ui->textEdit->toPlainText() != "")
    {
        SaveXMLFile(ui->textEdit->toPlainText(), &xmldoc);
    }
    else
    {
        QString sfname = QFileDialog::getSaveFileName(this, tr("Enter FileName"),directory.path());
        ui->textEdit->setText(sfname);
        SaveXMLFile(ui->textEdit->toPlainText(), &xmldoc);
    }
}

void MainDialog::on_pB_ReadDirStruct_clicked()
{
//    QString sFname = QFileDialog::getOpenFileName(this,tr("Open File"),directory.path(),tr("Files(*.*)"));
#if 1
    directory.setPath("C:/Users/dmmacs/Documents/Qt Projects/tmp_root");
#endif
    QString sPath = QFileDialog::getExistingDirectory(this,tr("Select Directory"),directory.path());

    if (sPath != "")
    {
        StdModel->clear();
        xmldoc.clear();

        directory.setPath(sPath);
        QDir tmp;
        tmp.setPath(sPath);
        QString relPath = tmp.relativeFilePath(sPath);

        //xmldoc.appendChild()
        QString nodeName;
        nodeName = XMLHeader + "\n";

        nodeName += "<" + DirTagName + " " + DirAttrName + "=\"" + directory.dirName() + "\" >" + "\n";
//        qDebug() << sPath << relPath;
//        qDebug() << nodeName;

        ReadDir(sPath, &nodeName);
        nodeName += "</" + DirTagName + ">" + "\n";
        qDebug() << nodeName;

        QTemporaryFile file("tempXML",this);
        file.setAutoRemove(false);

        file.open();//QIODevice::WriteOnly | QIODevice::Text);
        qDebug() << file.fileName();


        // Store the xml data in a temporary file
        QTextStream tmpStream(&file);
        tmpStream << nodeName;
        file.close();

        LoadXMLData(file.fileName());

        file.remove();

    }


}

void MainDialog::ReadDir(QString Path, QString *nodeData)
{
    QDir dir(Path);

    QFileInfoList dirs = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);

    foreach(const QFileInfo &fi, dirs)
    {      //Loops through the found files.
        QString sPath = fi.absoluteFilePath();  //Gets the absolute file path
        if (fi.isDir())
        {
            QDir tmp;
            tmp.setPath(Path);
            QString relPath = tmp.relativeFilePath(sPath);
            *nodeData += "<" + DirTagName + " " + DirAttrName + "=\"" + relPath + "\" >" + "\n";
//            qDebug() << sPath << relPath;
//            qDebug() << *nodeData;
            ReadDir(sPath, nodeData);
            *nodeData += "</" + DirTagName + ">" + "\n";
//            qDebug() << *nodeData;

        }
    }

}
