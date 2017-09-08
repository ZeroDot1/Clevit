#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionNew_File_triggered()
{
    path.clear();
    text.clear();
    originalText.clear();
    ui->textEdit->clear();

    ui->textEdit->setTextColor(Qt::black);
    ui->colorBtn->setStyleSheet("background-color: black");

    font.setStyleHint(QFont::Monospace);
    font.setStyleName("Monospace");
    font.setPixelSize(12);
    ui->fontSizeBox->setCurrentText("12");

    ui->fontComboBox->setFont(font);
    ui->fontComboBox->setCurrentText("Monospace");

    ui->textEdit->setFont(font);
    ui->textEdit->setTextColor(Qt::black);
    ui->colorBtn->setStyleSheet("background-color: black");

    title = QDir::currentPath();

    this->setWindowTitle(title);

}


void MainWindow::on_actionOpen_triggered()
{
    // Save the path of the text file

    path = QFileDialog::getOpenFileName(this,"Select a text file",QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html)"),&selFilter);

    QFile textFile(path);

    // Open the text file to read/write

    if(textFile.open(QIODevice::ReadWrite))
    {
        text = textFile.readAll();

        if(htmlFileVerifier() == true)
            ui->textEdit->setHtml(text);
        else
            ui->textEdit->setHtml(text);

        originalText = ui->textEdit->toPlainText();

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
        path = QFileDialog::getSaveFileName(this,"Save a text file",QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html)"),&selFilter);

    if(path.isEmpty() == false)
    {
        QFile textFile(path);

        // Open the text File to Write

        if(textFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            text.clear();

            if(htmlFileVerifier() == true)
                text = ui->textEdit->toHtml();
            else
                text = ui->textEdit->toPlainText(); // You need to copy the text in plaintext to compare with the originaltext

            originalText = text;

            text.clear();

            if(htmlFileVerifier() == true)
                text = ui->textEdit->toHtml();
            else
                text = ui->textEdit->toHtml();

            QTextStream textAppend(&textFile);

            textAppend << text;

            title = path;

            text.clear();

            isSaved = true;
            changedTitle = false;
        }

        else
        {
            QMessageBox::warning(this,"Save File Error",textFile.errorString());

            textChanged();

            if(path.isEmpty())
                title = QDir::currentPath();
            else
                title = path;
        }

        textFile.close();

        this->setWindowTitle(title);
    }
    else
        QMessageBox::warning(this,"Save File Error","Save canceled");
}

void MainWindow::on_actionSave_as_triggered()
{
    path = QFileDialog::getSaveFileName(this,"Save as a text file",QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html)"),&selFilter);

    if(path.isEmpty())
        return;

    QFile textFile(path);

    // Open the text File to Write

    if(textFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        text.clear();

        if(htmlFileVerifier() == true)
            text = ui->textEdit->toHtml();
        else
            text = ui->textEdit->toPlainText(); // You need to copy the text in plaintext to compare with the originaltext

        originalText = text;

        text.clear();

        if(htmlFileVerifier() == true)
            text = ui->textEdit->toPlainText();
        else
            text = ui->textEdit->toHtml();

        QTextStream textAppend(&textFile);

        textAppend << text;

        title = path;

        text.clear();

        isSaved = true;
        changedTitle = false;
    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        if(path.isEmpty())
            title = QDir::currentPath();
        else
            title = path;
    }

    textFile.close();

    this->setWindowTitle(title);

}
