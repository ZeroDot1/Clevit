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

    ui->htmlSourceCheckBox->setVisible(false);

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
        data = textFile.readAll();

        codec = Qt::codecForHtml(data);

        text = codec->toUnicode(data);

        if(htmlFileVerifier() == true)
        {
            ui->textEdit->setPlainText(text);
            ui->htmlSourceCheckBox->setVisible(true);
            originalText = ui->textEdit->toPlainText();
        }
        else
            if(Qt::mightBeRichText(text) == true)
            {
                ui->textEdit->setHtml(text);
                ui->htmlSourceCheckBox->setVisible(false);
                originalText = ui->textEdit->toPlainText();
            }
            else
            {
                text = QString::fromLocal8Bit(data);
                ui->textEdit->setPlainText(text);
                ui->htmlSourceCheckBox->setVisible(false);
                originalText = ui->textEdit->toPlainText();
            }


        text.clear();
        data.clear();
        codec = nullptr;

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

            if(htmlFileVerifier() == true && ui->htmlSourceCheckBox->isChecked() == true)
                text = ui->textEdit->toHtml();
            else
                text = ui->textEdit->toPlainText(); // You need to copy the text in plaintext to compare with the originaltext

            originalText = text;

            text.clear();

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

void MainWindow::on_actionExport_to_PDF_triggered()
{
    if(path.isEmpty() == true)
        path = QFileDialog::getSaveFileName(this,"Save as a text file",QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html)"),&selFilter);
    if(path.isEmpty() == true)
        return;
    if (QFileInfo(path).suffix().isEmpty())
    {
        path.append(".pdf");
    }
    else
    {
        path.truncate(path.lastIndexOf('.'));
        path.append(".pdf");
    }

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::Letter);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setFullPage(true);
    printer.setResolution(96);
    printer.setPageMargins(1.0, 1.0, 1.0, 1.0, QPrinter::Inch);
    printer.setOutputFileName(path);

    ui->textEdit->document()->print(&printer);
}
