#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtDebug>
#include <QFile>
#include <QSettings>
#include <QStringListModel>
#include <QProcess>
#include <QThread>
#include <QStandardPaths>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSettigs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSettigs(){
    qDebug()<<QDir::homePath();
    QSettings settings("Sett","Sett");
    filesToCompress= settings.value("filesToCompress").toStringList();
    workingDir=settings.value("workingDir",QDir::homePath()).toString();
    tmpPath=settings.value("workingDir",QDir::homePath()).toString()+"/tmp";
    outputPathHeader=settings.value("outputPathHeader","c://").toString();
    qDebug()<<workingDir;
    qDebug()<<tmpPath;
    qDebug()<<outputPathHeader;
    qDebug()<<filesToCompress;

    model.setStringList( filesToCompress );
    ui->fileList->setModel( &model );
    ui->fileList->show();
}

void MainWindow::on_SelWorkDir_clicked()
{
    workingDir = QFileDialog::getExistingDirectory(this,"Open file",workingDir);
    qDebug()<<workingDir;
    QSettings settings("Sett","Sett");
    settings.setValue("workingDir",workingDir);
    qDebug()<<"Working dir "<<workingDir;
    tmpPath=workingDir+"/tmp";
}

void MainWindow::on_AddFile_clicked()
{
    QString tmp = (QFileDialog::getOpenFileName(this,"Open file",workingDir)).split(workingDir+"/").last();
    bool test=false;
    foreach (QString fileName, filesToCompress) {
        if (fileName==tmp) {
            test= true;
            qDebug()<<"file already exist";
        }
    }
    if(!test){
        filesToCompress<<tmp;
    }
    QSettings settings("Sett","Sett");
    settings.setValue("filesToCompress",filesToCompress);
    model.setStringList( filesToCompress );
    ui->fileList->setModel( &model );
    ui->fileList->show();
    qDebug()<<"Files: "<<filesToCompress;
}

void MainWindow::on_DelFile_clicked()
{
    int i = ui->fileList->currentIndex().row();
    filesToCompress.removeAt(i);
    QSettings settings("Sett","Sett");
    settings.setValue("filesToCompress",filesToCompress);
    model.setStringList( filesToCompress );
    ui->fileList->setModel( &model );
    ui->fileList->show();
    qDebug()<<"Files: "<<filesToCompress;
}

void MainWindow::on_HeaderOutput_clicked()
{
    outputPathHeader = QFileDialog::getExistingDirectory(this,"Open file",outputPathHeader);
    qDebug()<<outputPathHeader;
    QSettings settings("Sett","Sett");
    settings.setValue("outputPathHeader",outputPathHeader);
    qDebug()<<"Path to headers "<<outputPathHeader;
}

void MainWindow::on_delAll_clicked()
{
    filesToCompress.clear();
    QSettings settings("Sett","Sett");
    settings.setValue("filesToCompress",filesToCompress);
    model.setStringList( filesToCompress );
    ui->fileList->setModel( &model );
    ui->fileList->show();
    qDebug()<<"Files: "<<filesToCompress;
}

void MainWindow::zip(int filename)
{
    QStringList list;
    QString in = workingDir+"/"+filesToCompress[filename];
    QString out = tmpPath+"/"+filesToCompress[filename].split("/").last()+".gz";
    list<< "a" << "-tgzip"<<out<<in;
    qDebug()<<"list "<<list;
    QProcess process;
    process.start("\"C:/Program Files/7-Zip/7z.exe\"",list);

    while (process.waitForFinished()) {
    }
    process.close();
    QFile file( filename +".gz");

    qDebug()<<"zip "<<file;
}

void MainWindow::on_convert_clicked()
{
    for (int i = 0; i < filesToCompress.size(); ++i) {
        //qDebug()<<tmpPath+"/"+file;
        zip(i);
    }
    setHeaderName();
    recap();
}

void  MainWindow::setHeaderName()
{
    QDir directory(tmpPath);
    QStringList files = directory.entryList(QStringList() << "*.gz",QDir::Files);
    foreach(QString filename, files) {

        QString inputFile= tmpPath+"/"+filename; //file with full path

        filename= filename.split("/").last();
        QStringList rename =filename.split(".");
        QString header=rename[0];
        QString headerVar=rename[0];
        QString headerDef=rename[0];
        for (int i = 1; i < rename.size()-1; ++i) {
            header+="_"+rename[i];
            headerVar+="_"+rename[i];
        }
        if (outputPathHeader=="") {
            outputPathHeader=workingDir+"/include/";
        }
        filename=outputPathHeader+"/"+rename[0]+".h"; //header path
        outputList<<rename[0]+".h";
        headerDef=header.toUpper()+"_H";
        convert(inputFile,filename,headerDef, headerVar);
    }
}

void MainWindow::convert(QString inputFile , QString outputFile, QString headerDef, QString headerVar)
{

    qDebug()<<inputFile<<"  "<<outputFile<<"  "<<headerDef<<"  "<< headerVar;
    QFile file( inputFile);
    while (!file.exists()) {
    }

    file.close();
    char separator[255];
    int bytes_per_string;
    unsigned int wrap_line;
    strcpy(separator, ", ");  	//default separator
    bytes_per_string = 1;	//read bytes (not words or ints) from file
    wrap_line = 70;
    int i;
    unsigned int chars_in_line=0;	//< number of chars in current line
    int stringlength;	//< length of single converted string
    long lSize;			//< filesize
    unsigned char *buffer;		//< pointer to buffer for input file
    FILE *fileIn, *fileOut; //< input and output file


    // start processing
    fileIn = fopen(inputFile.toStdString().c_str(), "rb"); // open input file (binary)
    if (fileIn==nullptr)
    {
        qDebug()<<"Error opening input file";
        exit (1);
    }

    // open output file
    fileOut = fopen(outputFile.toStdString().c_str(), "wt");
    if (fileOut==nullptr)
    {
        qDebug()<<"Error opening output file for write";
        exit (1);
    }
    // obtain file size.
    fseek (fileIn , 0 , SEEK_END);
    lSize = ftell (fileIn);
    rewind (fileIn);
    printf("Filesize: %d bytes.\\n", lSize);
    if(lSize%bytes_per_string)
    {
        qDebug()<<"Error: length of file isn't multiplication of bytes_per_string value.";
        qDebug()<<"Please modify input file or select other formatting";
        exit (3);
    }

    // allocate memory to contain the whole file.
    buffer = (unsigned char*) malloc (lSize);
    if (buffer == nullptr)
    {
        qDebug()<<"malloc for input file buffer failed (not enough memory?)";
        exit (2);
    }

    // copy the file into the buffer.
    fread (buffer,1,lSize,fileIn);
    fprintf(fileOut,"#ifndef %s\n#define %s\nconst char %s[] PROGMEM = {\n",headerDef.toStdString().c_str(),headerDef.toStdString().c_str(),headerVar.toStdString().c_str());

    stringlength = 4 + strlen(separator);	//length of single converted string
    for(i=0; i<lSize-1; i++)
    {
        fprintf(fileOut,"0x%02X%s",buffer[i],separator);
        chars_in_line += stringlength;
        if(chars_in_line >= wrap_line)
        {
            fprintf(fileOut, "\n");
            chars_in_line = 0;
        }
    }
    fprintf(fileOut,"0x%02X};\nsize_t  %s = %d;\n#endif",buffer[i],(headerVar+"_size").toStdString().c_str(),i+1);	//no separator after last string

    if(fileIn) fclose (fileIn);
    if(fileOut) fclose (fileOut);
    if(buffer) free (buffer);
}

void MainWindow::recap(){

    QFile file(outputPathHeader+"/web.h");
    qDebug()<<file;
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "#ifndef WEB_H\n";
        out << "#define WEB_H\n\n";
        foreach (QString fileName, outputList) {
            out << "#include \""+fileName+"\"\n";
        }
        out << "#endif";

        file.close();
        outputList.clear();
        if (ui->deleteTmp->isChecked()) {
            QDir dir(tmpPath);
            dir.removeRecursively();
        }
}
