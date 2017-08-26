#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// QAction Slots and Signals

void MainWindow::on_actionCopy_triggered()
{
    // Copy

    ui->plainTextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    // Paste

    ui->plainTextEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    // Cut

    ui->plainTextEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    // Undo

    ui->plainTextEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    // Redo

    ui->plainTextEdit->redo();
}

void MainWindow::on_actionExit_triggered()
{
    // Quit

    QApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
    // Save the path of the text file

    path = QFileDialog::getOpenFileName(this,"Select a text file",QDir::currentPath());

    //std::cout << path.toStdString() << std::endl;

    QFile textFile(path);

    // Open the text file to read/write

    if(textFile.open(QIODevice::ReadWrite))
    {
        text = textFile.readAll();

        ui->plainTextEdit->insertPlainText(text);
    }
    else
        std::cout << "Error: " << textFile.errorString().toStdString() << std::endl;

    textFile.close();
}

void MainWindow::on_actionSave_triggered()
{
    QFile textFile(path);

    // Open the text File to Write

    if(textFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        text.clear();

        text = ui->plainTextEdit->toPlainText();

        QTextStream textAppend(&textFile);

        textAppend << text;

    }

    textFile.close();
}
