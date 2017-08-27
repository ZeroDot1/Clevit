#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isSaved = false;

    selFilter = tr("Text Files (*.txt)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isSaved == false)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this,"TPad - Text Editor",tr("Do you want to save the text file?"),QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

        if(resBtn == QMessageBox::Yes)
        {
            while(isSaved == false)
                on_actionSave_triggered();

            event->accept();
        }
        else
            if(resBtn == QMessageBox::Cancel)
                event->ignore();
            else
                if(resBtn == QMessageBox::No)
                    event->accept();
    }
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

void MainWindow::on_actionNew_File_triggered()
{
    path.clear();
    text.clear();
    ui->plainTextEdit->clear();
}

void MainWindow::on_actionOpen_triggered()
{
    // Save the path of the text file

    path = QFileDialog::getOpenFileName(this,"Select a text file",QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt)"),&selFilter);

    //std::cout << path.toStdString() << std::endl;

    QFile textFile(path);

    // Open the text file to read/write

    if(textFile.open(QIODevice::ReadWrite))
    {
        text = textFile.readAll();

        ui->plainTextEdit->insertPlainText(text);
    }
    else
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

    textFile.close();

    isSaved = false;
}

void MainWindow::on_actionSave_triggered()
{
    std::cout << path.toStdString() << std::endl;

    if(path.isEmpty())
        path = QFileDialog::getSaveFileName(this,"Save a text file",QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt)"),&selFilter);

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

    isSaved = true;
}

void MainWindow::on_actionSave_as_triggered()
{
    path = QFileDialog::getSaveFileName(this,"Save as a text file",QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt)"),&selFilter);

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

    isSaved = true;
}
