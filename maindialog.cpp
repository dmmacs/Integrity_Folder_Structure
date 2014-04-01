#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"
#include "windows.h"
#include "version.h"


const QString DirTagName = "dir";
const QString DirAttrName = "name";
#ifdef QT_DEBUG
const QString DefaultPrjName = "/PSE/CoreDev/platform/test1";
const QString DefaultCP = "5290:35";
#else
const QString DefaultPrjName = "i.e. /PSE/AECU/CoreDev/platform";
const QString DefaultCP = ":none";
#endif
const QString MKShost = "--hostname=skobde-mks.kobde.trw.com";
const QString MKSport = "--port=7001";

const QString DefMKSPort = "7001";
const QString DefMKSHost = "skobde-mks.kobde.trw.com";


const QString XMLHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

QString Log = "";

int TotalNodes = 0;
int NodesCreated = 0;
QString PrgStyleBLKText = "QProgressBar {border: 2px solid grey; border-radius: 5px; text-align: center; color:#000000;}";
QString PrgStyleWHTText = "QProgressBar {border: 2px solid grey; border-radius: 5px; text-align: center; color:#FFFFFF;}";
QString PrgChunkStytle = "QProgressBar::chunk {background-color: #FFC823; width: 10px; margin: 0.5px;}";


MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    StdModel = new QStandardItemModel(this);

    ui->treeView->setModel(StdModel);
    ui->treeView->expandAll();
    ui->treeView->setHeaderHidden(true);

    ui->prgBar->setValue(0);
    ui->prgBar->setVisible(true);
    ui->prgBar->setStyleSheet(PrgStyleBLKText + PrgChunkStytle);

    ui->textEdit->setText("");
    ui->lineEdit->setStyleSheet("color: grey;");
    ui->lineEdit->setText(DefaultPrjName);
    ui->lineEdit->selectAll();

    // Version Text
    ui->label_5->setVisible(true);
    ui->label_5->setText("Version: " + QString::number(Version::MAJOR) + "." + QString::number(Version::MINOR) + "." + QString::number(Version::BUILD));


    isDirty = false;

//    QPoint loc = this->pos();
    //this->move(0,this->pos().y());

    ui->changePackageEdit->addItem(":none");
    ui->changePackageEdit->addItem(":bypass");

#ifdef QT_DEBUG
    ui->mks_portedit->setText("7001");
    ui->mks_serveredit->setText("skobde-mkstest.kobde.trw.com");
    ui->changePackageEdit->setCurrentText(DefaultCP);

    ui->textEdit->setText("C:/Users/mcdonaldd/Documents/Qt/Projects/MKS_Folder_Structure/tmp.xml");
    LoadXMLData(ui->textEdit->text());
    ui->m_pMKSGenButton->setEnabled(true);

#else
    ui->mks_portedit->setText(DefMKSPort);
    ui->mks_serveredit->setText(DefMKSHost);
    ui->changePackageEdit->setText(DefaultCP);
    ui->m_pMKSGenButton->setEnabled(true);
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

//    qDebug() << sPath;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
//        qDebug() << "Failed to open file";
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
        TotalNodes++;

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
    TotalNodes = 0;

    // Add the node
    QStandardItem *tmpNode;
    tmpNode = new QStandardItem(root.attribute(DirAttrName));
    Node->appendRow(tmpNode);
    TotalNodes++;

    root = root.firstChildElement(DirTagName);


    ProcessXMLData(&root, tmpNode, "  ");
    ui->treeView->expandAll();

    NodesCreated = 0;
//    qDebug() << TotalNodes;
}

void MainDialog::on_pBLoad_clicked()
{
    if (ui->textEdit->text() != "")
    {
//        qDebug() << ui->textEdit->text();

        LoadXMLData(ui->textEdit->text());
    }

}

void MainDialog::on_pBSave_clicked()
{
    bool overwrite = false;
    if (ui->textEdit->text() != "")
    {
        QFile Fout;//("ui->textEdit->text()");
        qDebug() << ui->textEdit->text();
        if (Fout.exists(ui->textEdit->text()))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Overwrite File?", "Overwrite?");
            if (reply == QMessageBox::Yes)
            {
                overwrite = true;
            }
//            msgBox.setWindowTitle("Overwrite File?");
//            msgBox.setText("Overwrite File?");
//            msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
//            msgBox.setDefaultButton(QMessageBox::Yes);
//            msgBox.setIcon(QMessageBox::Warning);

        }
        else
        {
            overwrite = true;
        }
        if (overwrite == true)
        {
            SaveXMLFile(ui->textEdit->text(), &xmldoc);
            isDirty = false;
        }
    }


    if (overwrite == false)
    {
        QString sfname = QFileDialog::getSaveFileName(this, tr("Enter FileName"),directory.path());
        ui->textEdit->setText(sfname);
        SaveXMLFile(sfname, &xmldoc);
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
        ui->textEdit->setText("");

        directory.setPath(sPath);
        QDir tmp;
        tmp.setPath(sPath);
        QString relPath = tmp.relativeFilePath(sPath);

        QString nodeName;
        nodeName = XMLHeader + "\n";

        nodeName += "<" + DirTagName + " " + DirAttrName + "=\"" + directory.dirName() + "\" >" + "\n";

        ReadDir(sPath, &nodeName);
        nodeName += "</" + DirTagName + ">" + "\n";
//        qDebug() << nodeName;

        QTemporaryFile file("tempXML",this);
        file.setAutoRemove(false);

        file.open();//QIODevice::WriteOnly | QIODevice::Text);
//        qDebug() << file.fileName();


        // Store the xml data in a temporary file
        QTextStream tmpStream(&file);
        tmpStream << nodeName;
        file.close();

        LoadXMLData(file.fileName());

        file.remove();

        ui->textEdit->setText("");
        isDirty = true;

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

//    qDebug() << sFname;
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

    ui->prgBar->setVisible(true);
    ui->prgBar->setValue(0);


    //**** Verify Integrity is running, MKS Login ****
    // Add Command Log entry
    UpdateLog("Checking for MKS Integrity Client in Windows Processes", 0);

    UpdateLog("-tasklist", 1);
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

//    qDebug() << Log;
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
            UpdateLog("Task Found",1);
            UpdateLog(item, 2);
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
//        qDebug() << Log;
        UpdateLog(tasks, 2);
        return;
    }

    // MKS Integrity is running,

    // Verify the root project exists
    cmd = "si projectinfo --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --project=" + \
            ui->lineEdit->text() + "/project.pj";


//    qDebug() << cmd;
    UpdateLog("Verify Root Project Exists", 0);
    UpdateLog(cmd, 1);

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
//        qDebug() << Log;
        return;
    }
    QString stdOut, stdErr;
    stdOut = cmdProc.readAllStandardOutput();
    stdErr = cmdProc.readAllStandardError();
    if (stdOut != "")
    {
//        qDebug() << "stdOut = " << stdOut;
        UpdateLog(stdOut, 1);
    }
    else
    {
//        qDebug() << "stdErr = " << stdErr;
        UpdateLog(stdErr, 1);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Invalid Root Project");
        msgBox.setText("The Root Project Does not Exist.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
//        qDebug() << Log;
        return;
    }

    // Root Project exists, Check if new structure top-level already exists.
    QStandardItem *tmpNode;
    QString nodeText;
    tmpNode = StdModel->invisibleRootItem();
    tmpNode = tmpNode->child(0);
    nodeText = tmpNode->text();
    cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --changePackageId=" +\
            ui->changePackageEdit->currentText() + " --nocloseCP --project=" + ui->lineEdit->text() + "/project.pj " + tmpNode->text() + "/project.pj"; //01_From_Customer/project.pj

    NodesCreated = 0;
    CreateMKSProjects("", tmpNode);

    //ui->Commands->setText(Log);

//    qDebug() << Log;

//    qDebug() << Log.size();


}

void MainDialog::CreateMKSProjects(QString root, QStandardItem *rootItem)
{
    QString stdOut, stdErr;
    int exitCode;
    int i, j = 0;
    QStandardItem *tmpItem;
    tmpItem = rootItem;
//    qDebug() << "Node-" << tmpItem->text() << "-" << root;

    QString cmd;
    QProcess cmdProc;

    if (root != "")
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->currentText() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + root + "/project.pj " + \
                tmpItem->text() + "/project.pj"; //01_From_Customer/project.pj
    }
    else
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->currentText() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + "/project.pj " + \
                tmpItem->text() + "/project.pj"; //01_From_Customer/project.pj
    }

    UpdateLog("Add Node-", 0);
    UpdateLog(tmpItem->text(), 1);
//    UpdateLog("\n");
//    UpdateLog("-");
    UpdateLog(cmd, 2);
//    UpdateLog("\n");
    if (RunMKSCmd(&cmd, &cmdProc))
    {
        exitCode = cmdProc.exitCode();
        stdOut = cmdProc.readAllStandardOutput();
        stdErr = cmdProc.readAllStandardError();

        NodesCreated++;
        if (exitCode == 0)
        {
            // Success
//            qDebug() << "Success" << stdErr;
            UpdateLog("Success-\n", 1);
            UpdateLog(stdErr, 2);
//            UpdateLog("\n");
        }
        else
        {
            // Fail
//            qDebug() << "Fail-" << stdErr;
            UpdateLog("Fail\n", 1);
            UpdateLog(stdErr, 2);
//            UpdateLog("\n");
        }
        if (((NodesCreated * 100) / TotalNodes) > 50)
        {
//            ui->prgBar->setStyleSheet(PrgStyleWHTText + PrgChunkStytle);
        }
        ui->prgBar->setValue((NodesCreated * 100) / TotalNodes);
    }
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
//            UpdateLog("Add Node");
//            UpdateLog("\n");
//            UpdateLog("-");
//            UpdateLog(tmpItem->text());
//            UpdateLog("\n");
//            qDebug() << tmpItem->text();
        }
    }
}

boolean MainDialog::RunMKSCmd(QString *cmd, QProcess *proc)
{
    boolean retVal;

    proc->start(*cmd);
    if (!proc->waitForFinished())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Could not run command to add node");
        msgBox.setText("Could not run command lines for MKS integrity.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        retVal = false;
    }
    else
    {
        retVal = true;
    }

    return (retVal);

}

void MainDialog::closeEvent(QCloseEvent *event)
{
    bool saveFile = false;

    if (isDirty)
    {
        //Messagebox to save
        QMessageBox::StandardButton reply;

        if (ui->textEdit->text() != "")
        {
            reply = QMessageBox::question(this, "Overwrite File?", "Overwrite?");
            if (reply == QMessageBox::Yes)
            {
                saveFile = true;
            }
        }
        else
        {
            reply = QMessageBox::question(this, "Save Directory Strcutre to XML?","Save Directory Strcutre to XML?");
            if (reply == QMessageBox::Yes)
            {
                saveFile = true;
            }
        }

        if (saveFile == true)
        {
            on_pBSave_clicked();
            if (isDirty == true)
            {
                // User Saved file, we are done.
                event->ignore();
            }
            else
            {
                // User clicked on Cancel, stay here.
                event->accept();
            }
        }
        else
        {
            event->accept();
        }
    }
    else
    {
        event->accept();
    }

}

void MainDialog::on_ConsoleCheck_clicked()
{
    if (ui->ConsoleCheck->isChecked())
    {
        this->resize(1251, this->size().height());
    }
    else
    {
        this->resize(681, this->size().height());

    }
}


void MainDialog::UpdateLog(QString Str, int tabCnt = 0)
{
    AddText(Str, tabCnt);
    Log += Str;
    Log += "\n";
}

static const char * tabs[] =
{
    "",
    "    ",
    "        ",
    "            ",
    "                ",
    "                    ",
    "                        ",
    "                            ",
    "                                ",
    "                                    "
};

void MainDialog::AddText(QString Str, int tabCnt)
{
    QString curStr = ui->Commands->toPlainText();

    QRegExp regExp("[\r\n]");
    QStringList lines;
    lines = Str.split(regExp,QString::KeepEmptyParts);

    for (int i = 0; i < lines.count(); i++)
    {
        QString item = lines.at(i);
        curStr += tabs[tabCnt];
        curStr += item;
        curStr += "\n";
    }

    ui->Commands->setText(curStr);

//    ui->Commands->setText(ui->Commands->toPlainText() + Str + "\n");
}


void MainDialog::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }

}
