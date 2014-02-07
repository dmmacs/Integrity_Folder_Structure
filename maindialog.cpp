#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"
#include "windows.h"


const QString DirTagName = "dir";
const QString DirAttrName = "name";
#ifdef QT_DEBUG
const QString DefaultPrjName = "/PSE/CoreDev/platform";
const QString DefaultCP = "none";
#else
const QString DefaultPrjName = "i.e. /PSE/AECU/CoreDev/platform";
const QString DefaultCP = "none";
#endif
const QString MKShost = "--hostname=skobde-mks.kobde.trw.com";
const QString MKSport = "--port=7001";

const QString DefMKSPort = "7001";
const QString DefMKSHost = "skobde-mks.kobde.trw.com";


const QString XMLHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

QString Log = "";


MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    StdModel = new QStandardItemModel(this);

    ui->treeView->setModel(StdModel);
    ui->treeView->expandAll();
    ui->treeView->setHeaderHidden(true);

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    ui->textEdit->setText("");
    ui->lineEdit->setStyleSheet("color: grey;");
    ui->lineEdit->setText(DefaultPrjName);
    ui->lineEdit->selectAll();

#ifdef QT_DEBUG
    ui->mks_portedit->setText("7001");
    ui->mks_serveredit->setText("skobde-mkstest.kobde.trw.com");
    ui->changePackageEdit->setText(DefaultCP);

    LoadXMLData("C:/Users/mcdonaldd/Documents/Qt/Projects/tmp.xml");


#else
    ui->mks_portedit->setText(DefMKSPort);
    ui->mks_serveredit->setText(DefMKSHost);
#endif

}

MainDialog::~MainDialog()
{
    delete ui;
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
//        qDebug() << tabStr << nextItem.attribute(DirAttrName);
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
//    qDebug() << root.attribute(DirAttrName);

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
    if (ui->lineEdit->text() != "")
    {
        LoadXMLData(ui->lineEdit->text());
    }

}

void MainDialog::on_pBSave_clicked()
{
    if (ui->lineEdit->text() != "")
    {
        SaveXMLFile(ui->lineEdit->text(), &xmldoc);
    }
    else
    {
        QString sfname = QFileDialog::getSaveFileName(this, tr("Enter FileName"),directory.path());
        ui->textEdit->setText(sfname);
        SaveXMLFile(ui->lineEdit->text(), &xmldoc);
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

        QString nodeName;
        nodeName = XMLHeader + "\n";

        nodeName += "<" + DirTagName + " " + DirAttrName + "=\"" + directory.dirName() + "\" >" + "\n";

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
            ReadDir(sPath, nodeData);
            *nodeData += "</" + DirTagName + ">" + "\n";

        }
    }

}

void MainDialog::on_lineEdit_selectionChanged()
{
    if (ui->lineEdit->text() == "")
    {

        ui->lineEdit->setStyleSheet("color: black;");
    }
}

void MainDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == DefaultPrjName)
    {
        ui->lineEdit->setStyleSheet("color: grey;");
    }
    else
    {
        ui->lineEdit->setStyleSheet("color: black;");
    }

}

void MainDialog::on_toolButton_clicked()
{
    QString sFname = QFileDialog::getOpenFileName(this,tr("Open File"),directory.path(),tr("Files(*.*)"));

    qDebug() << sFname;
    ui->textEdit->setText(sFname);

    if (sFname != "")
    {
        LoadXMLData(sFname);
    }

}


// Create the sturture in MKS
// 1. Verify MKS Login
// 2. Verify structure is not empty
// 3. Verify base project exists.
// 4. Create the structure
void MainDialog::on_m_pMKSGenButton_clicked()
{
    QString cmd;
    QProcess cmdProc;
    int i;

    Log = "";

    //**** Verify Integrity is running, MKS Login ****
    // Add Command Log entry
    Log += "Checking for MKS Integrity Client in Windows Processes";
    Log += "\n";
    Log += "-tasklist";
    Log += "\n";
    cmdProc.start("tasklist");
    if(!cmdProc.waitForFinished())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Could not get Tasklist");
        msgBox.setText("Could not get Tasklist, not sure why since this is standard command line operation. \n\rNeed admin rights?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

    qDebug() << Log;
        return;
    }

    QString tasks;
    QStringList taskList;
    tasks = cmdProc.readAllStandardOutput();
    tasks = tasks.trimmed();
    QRegExp regExp("[\r\n]");
    taskList = tasks.split(regExp, QString::SkipEmptyParts);

    for (i = 0; i < taskList.count(); i++)
    {
        QString item = taskList.at(i);
        if (item.contains("IntegrityClient.exe"))
        {
            // Found the Client running, break the loop
            Log += item;
            Log += "\n";
//            qDebug() << item;
            break;
        }
    }
    if (i == taskList.count())
    {
        // Couldn't find the process in the list
        QMessageBox msgBox;
        msgBox.setWindowTitle("MKS Integrity not running");
        msgBox.setText("Could not find MKS Integrity running, please start and login into MKS Integrity and rerun the program");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        qDebug() << Log;
        return;
    }

    // MKS Integrity is running, start building commandlines and run

    // Verify the root project exists
    cmd = "si projectinfo --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --project=" + \
            ui->lineEdit->text() + "/project.pj";


//    qDebug() << cmd;
    Log += "Verify Root Project Exists";
    Log += "\n";
    Log += "-";
    Log += cmd;
    Log += "\n";

    cmdProc.start(cmd);
    if (!cmdProc.waitForFinished())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("MKS Integrity not running");
        msgBox.setText("Could not run command lines for MKS integrity.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        qDebug() << Log;
        return;
    }
    QString stdOut, stdErr;
    stdOut = cmdProc.readAllStandardOutput();
    stdErr = cmdProc.readAllStandardError();
    if (stdOut != "")
    {
//        qDebug() << "stdOut = " << stdOut;
    }
    else
    {
//        qDebug() << "stdErr = " << stdErr;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Invalid Root Project");
        msgBox.setText("The Root Project Does not Exist.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        qDebug() << Log;
        return;
    }

    // Root Project exists, Check if new structure top-level already exists.
    QStandardItem *tmpNode;
    QString nodeText;
    tmpNode = StdModel->invisibleRootItem();
    tmpNode = tmpNode->child(0);
    nodeText = tmpNode->text();
    cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --changePackageId=" +\
            ui->changePackageEdit->text() + " --nocloseCP --project=" + ui->lineEdit->text() + "/project.pj " + tmpNode->text() + "/project.pj"; //01_From_Customer/project.pj

    CreateMKSProjects("", tmpNode);

    qDebug() << Log;
    return;

//    qDebug() << "First Node-" << tmpNode->text();
    Log += "Add First Node";
    Log += "\n";
    Log += "-";
    Log += cmd;
    Log += "\n";

    cmdProc.start(cmd);
    if (!cmdProc.waitForFinished())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("MKS Integrity not running");
        msgBox.setText("Could not run command lines for MKS integrity.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        qDebug() << Log;
        return;
    }

    stdOut = cmdProc.readAllStandardOutput();
    stdErr = cmdProc.readAllStandardError();
    if (stdOut != "")
    {
//        qDebug() << "stdOut = " << stdOut;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Root Project already exists");
        msgBox.setText("Could not run command lines for MKS integrity.");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Question);
        if (msgBox.exec() == QMessageBox::No)
        {
            qDebug() << Log;
            return;
        }

    }
    // Project does not exist, so start creating it.
//    qDebug() << "stdErr = " << stdErr;
//    tmpNode = StdModel->invisibleRootItem();
//    nodeText = tmpNode->text();
    CreateMKSProjects(nodeText, tmpNode);


//    qDebug() << Log;


}

void MainDialog::CreateMKSProjects(QString root, QStandardItem *rootItem)
{
    int i, j = 0;
    QStandardItem *tmpItem;
    tmpItem = rootItem;
    qDebug() << "Node-" << tmpItem->text() << "-" << root;

    QString cmd;
    QProcess cmdProc;

    if (root != "")
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->text() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + root + "/project.pj " + \
                tmpItem->text() + "/project.pj"; //01_From_Customer/project.pj
    }
    else
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->text() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + "/project.pj " + \
                tmpItem->text() + "/project.pj"; //01_From_Customer/project.pj
    }

    Log += "Add Node-";
    Log += tmpItem->text();
    Log += "\n";
    Log += "-";
    Log += cmd;
    Log += "\n";

    //si createsubproject --no --hostname=%mkshost% --port=%mksport% --changePackageId=%changePackageID% --nocloseCP --project=%project% 01_From_Customer/project.pj
    for (i = 0; i < rootItem->rowCount(); i++)
    {
//        tmpItem = rootItem->child(i);
        if (tmpItem->hasChildren())
        {
            QStandardItem *childItem;
            childItem = tmpItem->child(j);
            CreateMKSProjects(root + "/" + tmpItem->text(), childItem);
            j++;
        }
        else
        {
//            Log += "Add Node";
//            Log += "\n";
//            Log += "-";
//            Log += tmpItem->text();
//            Log += "\n";
//            qDebug() << tmpItem->text();
        }
    }
}
