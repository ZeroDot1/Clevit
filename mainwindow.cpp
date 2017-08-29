#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(this)
{
    ui->setupUi(this);

    text = ui->plainTextEdit->toPlainText();

    isSaved = false, changedTitle = false;

    selFilter = tr("Text Files (*.txt)");

    title = QDir::currentPath();

    this->setWindowTitle(title);

    //when the timer times out the signal is emitted and the ontimeout() private slot is executed

    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

    //call the timer every 10 ms

    timer.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeout()
{   
    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged,this,&MainWindow::textChanged);
}

void MainWindow::textChanged()
{
    disconnect(ui->plainTextEdit,&QPlainTextEdit::textChanged,this,&MainWindow::textChanged);

    isSaved = false;

    text = ui->plainTextEdit->toPlainText();

    if(changedTitle == false && fileNotChanged() == false)
    {
        this->setWindowTitle(title+"*");

        changedTitle = true;
    }
    else
        if(fileNotChanged() == true)
        {
            this->setWindowTitle(title);

            changedTitle = false;
        }
}

bool MainWindow::fileNotChanged()
{
    int res = QString::compare(originalText,text,Qt::CaseSensitive);

    if(res == 0)
        return true;
    else
        return false;

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    text = ui->plainTextEdit->toPlainText();

    // If the user doesn't has type text or open any text file, so the program doesn't show the message box

    if(text.isEmpty() == false)
    {        
        if(isSaved == false && changedTitle == true)
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
                {
                    event->ignore();
                    title = QDir::currentPath();
                }
                else
                    if(resBtn == QMessageBox::No)
                        event->accept();
        }
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
    originalText.clear();
    ui->plainTextEdit->clear();

    title = QDir::currentPath();

    this->setWindowTitle(title);

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

        originalText = text;

        ui->plainTextEdit->insertPlainText(text);

        text.clear();

        changedTitle = false;
        title = path;

    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        title = QDir::currentPath();
    }

    textFile.close();

    this->setWindowTitle(title);

    isSaved = false;
}

void MainWindow::on_actionSave_triggered()
{

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

        originalText = text;

        text.clear();
    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        title = QDir::currentPath();
    }

    textFile.close();

    title = path;

    this->setWindowTitle(title);

    isSaved = true;
    changedTitle = false;
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

        originalText = text;

        text.clear();
    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        title = QDir::currentPath();
    }

    textFile.close();

    title = path;

    this->setWindowTitle(title);

    isSaved = true;
    changedTitle = false;
}

void MainWindow::on_actionAbout_TPad_triggered()
{
    About *about = new About(this);

    about->show();
}
