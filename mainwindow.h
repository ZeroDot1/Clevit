#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

    QString path; // Path of text file
    QString text; // Text of text file
};

#endif // MAINWINDOW_H
