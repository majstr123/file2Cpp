#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SelWorkDir_clicked();

    void on_AddFile_clicked();

    void on_DelFile_clicked();

    void on_HeaderOutput_clicked();

    void on_delAll_clicked();

    void on_convert_clicked();

private:
    Ui::MainWindow *ui;
    QString workingDir;
    QStringList filesToCompress;
    QStringList outputList;
    QString tmpPath;
    QString outputPathHeader;
    QStringListModel model;
    void initSettigs();
    void zip(int filename);
    void setHeaderName();
    void convert(QString, QString fileName, QString headerDef, QString headerVar);


    void recap();
};

#endif // MAINWINDOW_H
