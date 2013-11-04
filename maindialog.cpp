#include "maindialog.h"
#include "ui_maindialog.h"

#include "mksfoldertemplate.h"
#include "windows.h"


const QString DirTagName = "dir";
const QString DirAttrName = "name";
#ifdef QT_DEBUG
const QString DefaultPrjName = "/PSE/AECU/CoreDev/platform";
#else
const QString DefaultPrjName = "i.e. /PSE/AECU/CoreDev/platform";
#endif
const QString MKShost = "--hostname=skobde-mks.kobde.trw.com";
const QString MKSport = "--port=7001";

const QString XMLHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

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

#ifdef QT_DEBUG
    cmd = "si projectinfo " + MKShost + " " + MKSport + " --project=" + "/PSE" + "/project.pj";
#else
    cmd = "si projectinfo " + MKShost + " " + MKSport + " --project=" + ui->lineEdit->text() + "/project.pj";
#endif

    qDebug() << cmd;


    QProcess bat;
    bat.start(cmd);
    bat.waitForFinished();
        QInputDialog input;
        qDebug() << bat.readAllStandardOutput();
        qDebug() << bat.readAllStandardError();
    return;
    bat.waitForFinished();

//    bat.waitForReadyRead(5000);

    QProcess process;

    process.start(cmd);
    process.waitForFinished(10000);
    QString p_stdout = process.readAllStandardOutput();
    QString p_stderr = process.readAllStandardError();

    if (p_stderr == "") //("Revision"))
    {
        qDebug() << "No errors, " << p_stdout;
    }
    else
    {
        qDebug() << "Errors, " << p_stderr;
    }



}
#include <psapi.h>
#include <tchar.h>
bool MainDialog::matchProcessName( DWORD processID, std::string processName)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Compare process name with your string
    QString tmp1, tmp2;
    tmp1 = wcstombs(szProcessName);
    tmp2 = processName.c_str();
    bool matchFound = !_tcscmp(szProcessName, processName.c_str() );

    // Release the handle to the process.
    CloseHandle( hProcess );

    return matchFound;
}
