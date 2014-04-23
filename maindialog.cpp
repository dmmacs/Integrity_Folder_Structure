#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"
#include "windows.h"
#include "version.h"
#include "help.h"



#define CMD_TIMEOUT (60000)

const QString DirTagName = "dir";
const QString DirAttrName = "name";
const QString FldrTagName = "Folder_Structure";
const QString FldrAttrName = "version";
const QString FldrStrucVer = "1.0";

#ifdef QT_DEBUG
const QString DefaultPrjName = "/PSE/AECU/CoreDev";
//const QString DefaultPrjName = "/PSE/AECU/CustomerDev/";
//const QString DefaultCP = "5290:35";
const QString DefaultCP = ":none";
const QString DefaultCust = "platform";
//const QString DefaultCust = "test1";
#else
const QString DefaultPrjName = "/PSE/AECU/CustomerDev";
const QString DefaultCP = ":none";
const QString DefaultCust = "Fiat";
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

static int TotalDirs = 0;
static int Cnt = 0;


MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog)
{
    QCoreApplication::setApplicationVersion(APP_VERSION);

    ui->setupUi(this);

    hlpDlg = NULL;

    StdModel = new QStandardItemModel(this);

    ui->treeView->setModel(StdModel);
    ui->treeView->expandAll();
    ui->treeView->setHeaderHidden(true);

    ui->prgBar->setValue(0);
    ui->prgBar->setVisible(true);
    ui->prgBar->setStyleSheet(PrgStyleBLKText + PrgChunkStytle);

    ui->textEdit->setText("");
//    ui->lineEdit->setStyleSheet("color: grey;");
    ui->lineEdit->setText(DefaultPrjName);
    ui->lineEdit->selectAll();

    // Version Text
    ui->label_5->setVisible(true);
    ui->label_5->setText("Version:" + QCoreApplication::applicationVersion());


    isDirty = false;

    ui->changePackageEdit->addItem(":none");
    ui->changePackageEdit->addItem(":bypass");

#ifdef QT_DEBUG
    ui->mks_portedit->setText("7005");
    ui->mks_serveredit->setText("skobde-mksdev.kobde.trw.com");
//    ui->mks_serveredit->setText(DefMKSHost);
    ui->changePackageEdit->setCurrentText(DefaultCP);
    ui->line_customer->setText(DefaultCust);
    ui->line_customer_project->setText("Dummy1");

    ui->textEdit->setText("tmp.xml");
    LoadXMLData(ui->textEdit->text());
    ui->m_pMKSGenButton->setEnabled(true);

#else
    ui->mks_portedit->setText(DefMKSPort);
    ui->mks_serveredit->setText(DefMKSHost);
    ui->changePackageEdit->setCurrentText(DefaultCP);
    ui->m_pMKSGenButton->setEnabled(true);
//    ui->line_customer->setText(DefaultCust);
#endif



}

MainDialog::~MainDialog()
{
    delete ui;

    if (hlpDlg)
    {
        delete hlpDlg;
    }

}

void MainDialog::ParseXMLFile(QString sPath, QDomDocument *xmlDoc)
{
    QFile file(sPath);

    UpdateLog("Loading XML File: " + sPath, 1);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textEdit->setText("Failed to open file");
        UpdateLog("could not open XML File", 2);
    }
    else
    {
        QString errorMsg;
        int errorLine;
        int errorCol;

        if(!xmlDoc->setContent(&file, &errorMsg, &errorLine, &errorCol))
        {
            UpdateLog("Could Not Build xmlDoc", 2);
            ui->textEdit->setText("Failed to load document");
        }
        else
        {
            UpdateLog("xmlDoc Built", 2);
        }
        file.close();
        UpdateLog("File Closed", 2);
    }

}

void MainDialog::SaveXMLFile(QString sPath, QDomDocument *xmlDoc)
{
    QFile file(sPath);


    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->textEdit->setText("Failed to open file");
    }
    else
    {
        QTextStream TextStream(&file);
        xmlDoc->save(TextStream,0);
        file.close();
        isDirty = false;
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
        QApplication::processEvents();

        tmpNode = new QStandardItem(nextItem.attribute(DirAttrName));
        rootNode->appendRow(tmpNode);
        UpdateLog("Adding Node: "+ tmpNode->text(), 1);
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
    QString XmlVer;
    // Clear the Tree
//    StdModel->clear();

    UpdateLog("Parsing XML File");
    ParseXMLFile(sFname, &xmldoc);

    // Get Root of Tree and XML
    QStandardItem *Node = StdModel->invisibleRootItem();

    QDomElement root = xmldoc.firstChildElement(FldrTagName);
    XmlVer = root.attribute(FldrAttrName);

    root = root.firstChildElement(DirTagName);
    // Add the Root node
    UpdateLog("Adding the root node: " + root.attribute(DirAttrName), 1);

    TotalNodes = 0;

    // Add the node
    QStandardItem *tmpNode;
    tmpNode = new QStandardItem(root.attribute(DirAttrName));
    if (ui->line_customer_project->text() != "")
    {
        tmpNode->setText(ui->line_customer_project->text());
    }
    Node->appendRow(tmpNode);
    TotalNodes++;

    root = root.firstChildElement(DirTagName);
    if (ui->line_customer_project->text() != "")
    {
//        root.attribute()
    }

    UpdateLog(root.attribute(DirAttrName), 2);

    ProcessXMLData(&root, tmpNode, "  ");
    ui->treeView->expandAll();

    NodesCreated = 0;

}

void MainDialog::on_pBLoad_clicked()
{
    if (ui->textEdit->text() != "")
    {
        StdModel->clear();
        xmldoc.clear();
        ClearLog();
        LoadXMLData(ui->textEdit->text());
    }

}

void MainDialog::on_pBSave_clicked()
{
    bool overwrite = false;
    if (ui->textEdit->text() != "")
    {
        QFile Fout;//("ui->textEdit->text()");
        if (Fout.exists(ui->textEdit->text()))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Overwrite File?", "Overwrite?");
            if (reply == QMessageBox::Yes)
            {
                overwrite = true;
            }
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
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//    this->setCursor(Qt::WaitCursor);
    QApplication::processEvents();

#if 0
    directory.setPath("C:/Users/dmmacs/Documents/Qt Projects/tmp_root");
#endif
    QString sPath = QFileDialog::getExistingDirectory(this,tr("Select Directory"),directory.path());

    if (sPath != "")
    {
        StdModel->clear();
        xmldoc.clear();
        ui->textEdit->setText("");

        Log = "";
        UpdateLog("Reading Directory: " + sPath);

        directory.setPath(sPath);
        // Determine Total SubDirs
        QString cmd = "cmd.exe /c dir /b /s /A D *";
        QProcess cmdProc;
        QString stdOut, stdErr;

        directory.setCurrent(sPath);

        UpdateLog(directory.currentPath());

        cmdProc.start(cmd);
        if(!cmdProc.waitForFinished())
        {
            UpdateLog(cmdProc.errorString(),1);
            UpdateLog("Cound not run dir command to get total Dirs", 1);
        }
        stdOut = cmdProc.readAllStandardOutput();
        stdErr = cmdProc.readAllStandardError();
        if (stdOut != "")
        {
            QStringList outPut;
            QRegExp regExp("[\r\n]");
            outPut = stdOut.split(regExp, QString::SkipEmptyParts);
            TotalDirs = outPut.count();
            Cnt = 0;
        }
        else
        {
            UpdateLog("StdErr: " + stdErr, 1);
        }
        QDir tmp;
        tmp.setPath(sPath);
        QString relPath = tmp.relativeFilePath(sPath);

        UpdateLog("Determine Relative Path: " + relPath);
        QString nodeName;
        nodeName = XMLHeader + "\n";
        nodeName += "<" + FldrTagName + " " + FldrAttrName + "=\"" + FldrStrucVer + "\" >" + "\n";

        nodeName += "<" + DirTagName + " " + DirAttrName + "=\"" + directory.dirName() + "\" >" + "\n";

        UpdateLog("Read Directory Structure");
        ReadDir(sPath, &nodeName);

        // Clean up progress bar to ensure it hits 100% since we are done.
        ui->prgBar->setValue(100);
        nodeName += "</" + DirTagName + ">" + "\n";
        nodeName += "<" + FldrTagName + ">" + "\n";

        UpdateLog("Create Temporary XML File");
        QTemporaryFile file("tempXML",this);
        file.setAutoRemove(false);

        file.open();//QIODevice::WriteOnly | QIODevice::Text);
        UpdateLog("Temp File: " + file.fileName());

        // Store the xml data in a temporary file
        QTextStream tmpStream(&file);
        tmpStream << nodeName;
        file.close();

        LoadXMLData(file.fileName());

//        file.remove();

        ui->textEdit->setText("");
        isDirty = true;

    }

//    this->setCursor(Qt::ArrowCursor);

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
            UpdateLog("Adding DIR: " + relPath, 1);
        }
        Cnt++;
        ui->prgBar->setValue(Cnt * 100 / TotalDirs);
    }

}


void MainDialog::on_lineEdit_selectionChanged()
{
    if (ui->lineEdit->text() == "")
    {

//        ui->lineEdit->setStyleSheet("color: black;");
    }
}


void MainDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == DefaultPrjName)
    {
//        ui->lineEdit->setStyleSheet("color: grey;");
    }
    else
    {
//        ui->lineEdit->setStyleSheet("color: black;");
    }

}

void MainDialog::on_toolButton_clicked()
{
    QString sFname = QFileDialog::getOpenFileName(this,tr("Open File"),directory.path(),tr("Files(*.*)"));

    ui->textEdit->setText(sFname);
    on_pBLoad_clicked();

//    if (sFname != "")
//    {
//        ClearLog();
//        LoadXMLData(sFname);
//    }

}


// Create the sturture in MKS
// 1. Verify MKS Login
// 2. Verify CP exists
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


    if (ui->line_customer->text() == "")
    {
        QMessageBox msg;
        msg.setInformativeText("Must enter a customer (i.e. Fiat)");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
        return;
    }

    if (ui->line_customer_project->text() == "")
    {
        QMessageBox msg;
        msg.setInformativeText("Must enter a project name (i.e. Gen7.2)");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
        return;
    }
    if (ui->changePackageEdit->currentText() == "")
    {
        QMessageBox msg;
        msg.setInformativeText("Must enter a change Package");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
        return;
    }
    if (ui->lineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setInformativeText("Must enter a base project (i.e. /PSE/CustomerDev/");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
        return;
    }



    //**** Verify Integrity is running, MKS Login ****
    // Add Command Log entry
    UpdateLog("Checking for MKS Integrity Client in Windows Processes", 0);

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

        UpdateLog("-tasklist", 1);
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
        UpdateLog(tasks, 2);
        return;
    }

    // MKS Integrity is running,

    // Verify CP exists
    QString cp;
    cp = ui->changePackageEdit->currentText();
    if ((cp != ":none") && (cp != ":bypass"))
    {
        // Todo: Check to see if CP exists
    }

    UpdateLog("Verify Base Project Exists", 0);
    if (!VerifyProjectExists(ui->lineEdit->text(), ""))
    {
        UpdateLog("Base Project does not exist" + ui->lineEdit->text(), 0);
        return;
    }
    UpdateLog("Verify Customer Exists", 0);
    if (!VerifyProjectExists(ui->lineEdit->text(), ui->line_customer->text()))
    {
        UpdateLog("Customer Project does not exist" + ui->lineEdit->text(), 0);
        return;
    }

    UpdateLog("Verify Project Exists", 0);
    if (!VerifyProjectExists(ui->lineEdit->text() + ((ui->line_customer->text().right(1) == "/") ? "" : "/") + ui->line_customer->text(), ui->line_customer_project->text()))
    {
        UpdateLog("Project does not exist" + ui->lineEdit->text(), 0);
        return;
    }

    QStandardItem *tmpNode;
    QString nodeText;
    tmpNode = StdModel->invisibleRootItem();
    tmpNode = tmpNode->child(0);
    nodeText = tmpNode->text();

    NodesCreated = 0;
    CreateMKSProjects("", tmpNode);
}

void MainDialog::CreateMKSProjects(QString root, QStandardItem *rootItem)
{
    QString stdOut, stdErr;
    int exitCode;
    int i, j = 0;
    QStandardItem *tmpItem;
    tmpItem = rootItem;

    QString cmd;
    QProcess cmdProc;

    QApplication::processEvents();

    if (root != "")
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->currentText() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + ((ui->line_customer->text().right(1) == "/") ? "" : "/") \
                + ui->line_customer->text() + root + "/project.pj " + \
                tmpItem->text() + "/project.pj";
    }
    else
    {
        cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->currentText() + \
                " --nocloseCP --project=" + ui->lineEdit->text() + ((ui->line_customer->text().right(1) == "/") ? "" : "/") \
                + ui->line_customer->text() + "/project.pj " + \
                tmpItem->text() + "/project.pj";
    }

    UpdateLog("Add Node-", 0);
    UpdateLog(tmpItem->text(), 1);
    UpdateLog(cmd, 2);
    if (RunMKSCmd(&cmd, &cmdProc))
    {
        exitCode = cmdProc.exitCode();
        stdOut = cmdProc.readAllStandardOutput();
        stdErr = cmdProc.readAllStandardError();

        NodesCreated++;
        if (exitCode == 0)
        {
            // Success
            UpdateLog("Success-\n", 1);
            UpdateLog(stdErr, 2);
        }
        else
        {
            // Fail
            UpdateLog("Fail\n", 1);
            UpdateLog(stdErr, 2);
        }
        ui->prgBar->setValue((NodesCreated * 100) / TotalNodes);
    }
    for (i = 0; i < rootItem->rowCount(); i++)
    {
        if (tmpItem->hasChildren())
        {
            QStandardItem *childItem;
            childItem = tmpItem->child(j);
            CreateMKSProjects(root + "/" + tmpItem->text(), childItem);
            j++;
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

void MainDialog::UpdateLog(QString Str)
{
    UpdateLog(Str, 0);
}

void MainDialog::ClearLog()
{
    Log = "";
    ui->Commands->setText("");
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
//    ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());
    QScrollBar *sb = ui->Commands->verticalScrollBar();
    sb->setValue(sb->maximum());
//    ui->Commands->verticalScrollBar()->maximum();//ui->Commands->verticalScrollBar()->maximum());
}


void MainDialog::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }

}

void MainDialog::on_Help_Btn_clicked()
{
    if (hlpDlg == NULL)
    {
        hlpDlg = new Help;
    }
    hlpDlg->setModal(true);
    hlpDlg->show();
}

QString MainDialog::BuildBaseCmd(void)
{
    return ("si projectinfo --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --project=");
}

boolean MainDialog::VerifyProjectExists(QString base, QString prj)
{
    QString cmd;
    QProcess cmdProc;
    boolean retVal = true;

    // Verify the root project exists
    cmd = "si projectinfo --hostname=" + ui->mks_serveredit->text() + " --port=" + ui->mks_portedit->text() + " --project=" + \
            ui->lineEdit->text() + "/project.pj";
    cmd = BuildBaseCmd();
    cmd += base;
    if (cmd.at(cmd.length()-1) != '/')
    {
        cmd += "/";
    }
    if (prj != "")
    {
        cmd += prj;
    }
    if (cmd.at(cmd.length()-1) != '/')
    {
        cmd += "/";
    }
    cmd += "project.pj";

    UpdateLog(cmd, 1);

    cmdProc.start(cmd);
    if (!cmdProc.waitForFinished(CMD_TIMEOUT))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("MKS Integrity not running");
        msgBox.setText("Could not run command lines for MKS integrity.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return (false);
    }
    QString stdOut, stdErr;
    stdOut = cmdProc.readAllStandardOutput();
    stdErr = cmdProc.readAllStandardError();
    if (stdOut != "")
    {
        UpdateLog(stdOut, 1);
        retVal = true;
    }
    else
    {
        QMessageBox::StandardButton reply;
        QString msg = base + ((base.right(1) == "/") ? "" : "/") + prj;
                reply = QMessageBox::question(this, "Project does not exist, Create?", "Project, " + msg + " does not exist, Create?");
        if ((reply == QMessageBox::Yes) && (prj != ""))
        {
            cmd = "si createsubproject --no --hostname=" + ui->mks_serveredit->text() + " --port=" + \
                    ui->mks_portedit->text() + " --changePackageId=" + ui->changePackageEdit->currentText() + \
                    " --nocloseCP --project=" + base + "/project.pj " + \
                    prj + "/project.pj"; //01_From_Customer/project.pj
            UpdateLog("Creating Project, " + prj, 0);
            UpdateLog(cmd, 0);
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
                return (false);
            }
            QString stdOut, stdErr;
            stdOut = cmdProc.readAllStandardOutput();
            stdErr = cmdProc.readAllStandardError();
            if (stdOut != "")
            {
                UpdateLog(stdOut, 0);
            }
            else
            {
                UpdateLog(stdErr, 0);
            }
            retVal = true;

        }
        else
        {
            UpdateLog(stdErr, 1);
            QMessageBox msgBox;
            msgBox.setWindowTitle("Invalid Root Project");
            msgBox.setText("The Root Project Does not Exist.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return (false);
        }
    }

    return (retVal);
}

void MainDialog::on_line_customer_project_editingFinished()
{
    // Customer Project changed, update the list with the new name
    on_pBLoad_clicked();
}
