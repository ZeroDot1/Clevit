
/************************************************************************************************************
*    											                                                            *
*    TPad -  A text editor written on C++ with Qt Framework                                                 *
*											                                                                *
*    Copyright (C) 2017  Tiago Martins                        				                                *
*											                                                                *
*    This program is free software: you can redistribute it and/or modify		                            *
*    it under the terms of the GNU General Public License as published by                                   *
*    the Free Software Foundation, either version 3 of the License, or                                      *
*    (at your option) any later version. 					                                                *
*											                                                                *
*    This program is distributed in the hope that it will be useful,			                            *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of			                                *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			                                *
*    GNU General Public License for more details.					                                        *
*											                                                                *
*    You should have received a copy of the GNU General Public License			                            *
*    along with this program. If not, see <http://www.gnu.org/licenses/>.                                   *
*											                                                                *
*************************************************************************************************************/

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
                if(textFileVerifier() == true)
                {
                    text = QString::fromLocal8Bit(data);
                    ui->textEdit->setPlainText(text);
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
        path = QFileDialog::getSaveFileName(this,"Save a text file",QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html);;Odf Files (*.odf)"),&selFilter);

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

            if(odfFileVerifier() == true)
            {
                writer.setFileName(path);
                writer.setFormat("odf");
                writer.write(ui->textEdit->document());
            }
            else
                if(htmlFileVerifier() == true)
                {
                    data = ui->textEdit->toPlainText().toUtf8();

                    codec = Qt::codecForHtml(data);

                    text = codec->toUnicode(data);

                    QTextStream textAppend(&textFile);

                    textAppend << text;
                }
                else
                    if(QString::compare(QFileInfo(path).suffix(),"html.txt") == 0)
                    {
                        text = ui->textEdit->toHtml();
                        ui->htmlSourceCheckBox->setVisible(false);
                        originalText = ui->textEdit->toPlainText();

                        QTextStream textAppend(&textFile);

                        textAppend << text;
                    }
                    else
                    {
                        text = ui->textEdit->toPlainText();

                        QTextStream textAppend(&textFile);

                        textAppend << text;
                    }

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
    path = QFileDialog::getSaveFileName(this,"Save as a text file",QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html);; Odf Files (*.odf)"),&selFilter);

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

        if(odfFileVerifier() == true)
        {
            writer.setFileName(path);
            writer.setFormat("odf");
            writer.write(ui->textEdit->document());
        }
        else
            if(htmlFileVerifier() == true)
            {
                data = ui->textEdit->toPlainText().toUtf8();

                codec = Qt::codecForHtml(data);

                text = codec->toUnicode(data);

                QTextStream textAppend(&textFile);

                textAppend << text;
            }
            else
                if(QString::compare(QFileInfo(path).suffix(),"html.txt") == 0)
                {
                    text = ui->textEdit->toHtml();
                    ui->htmlSourceCheckBox->setVisible(false);
                    originalText = ui->textEdit->toPlainText();

                    QTextStream textAppend(&textFile);

                    textAppend << text;
                }
                else
                {
                    text = ui->textEdit->toPlainText();

                    QTextStream textAppend(&textFile);

                    textAppend << text;
                }

        title = path;

        text.clear();

        isSaved = true;
        changedTitle = false;
    }
    else
    {
        QMessageBox::warning(this,"Save File Error",textFile.errorString());

        if(path.isEmpty())
            title = QDir::currentPath();
        else
            title = path;
    }

    textFile.close();

    this->setWindowTitle(title);

}

void MainWindow::on_actionExport_to_Formatting_Txt_File_triggered()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,"Formatting TextFile export",tr("When exporting the formatted text file, you can only open it with TPad "
                                                               "\nor other software that can read html code. "
                                                               "\nAre you sure you want to continue?\n"),QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if(res == QMessageBox::No)
        return;
    else
        if(res == QMessageBox::Yes)
        {

            path = QFileDialog::getSaveFileName(this,"Export File to Formatting Text File",QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt)"),&selFilter);

            if(path.isEmpty() == true)
                return;
            if (QFileInfo(path).suffix().isEmpty())
            {
                path.append(".html.txt");
            }
            else
            {
                path.truncate(path.lastIndexOf('.'));
                path.append(".html.txt");
            }

            QFile textFile(path);

            if(textFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                text.clear();

                originalText = ui->textEdit->toPlainText();

                text = ui->textEdit->toHtml();

                QTextStream textAppend(&textFile);

                textAppend << text;
            }
            else
            {
                QMessageBox::warning(this,"Export File Error",textFile.errorString());

                if(path.isEmpty())
                    title = QDir::currentPath();
                else
                    title = path;
            }

            title = path;

            textFile.close();

            isSaved = true;
            changedTitle = false;
        }

    this->setWindowTitle(title);
}

void MainWindow::on_actionExport_to_PDF_triggered()
{
    path = QFileDialog::getSaveFileName(this,"Export File to PDF",QDir::currentPath(), tr("All Files (*.*);;Pdf Files (*.pdf)"),&selFilter);

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
    printer.setColorMode(QPrinter::Color);
    printer.setFullPage(true);
    printer.setResolution(96);
    printer.setPageMargins(1.0, 1.0, 1.0, 1.0, QPrinter::Inch);
    printer.setOutputFileName(path);

    ui->textEdit->document()->print(&printer);

    isSaved = true;
    changedTitle = false;

}


bool MainWindow::htmlFileVerifier()
{
    if(QString::compare(QFileInfo(path).suffix(),"html") == 0)
        return true;

    return false;
}

bool MainWindow::textFileVerifier()
{
    if(QString::compare(QFileInfo(path).suffix(),"txt") == 0)
        return true;

    return false;

}

bool MainWindow::odfFileVerifier()
{
    if(QString::compare(QFileInfo(path).suffix(),"odf") == 0)
        return true;

    return false;
}
