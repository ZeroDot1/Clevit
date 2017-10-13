/************************************************************************************************************
*    											                                                            *
*    Clevit -  The only smart text editor in this galaxy                                                 *
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
#include "highlighter.h"


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
    path = dialog.getOpenFileName(this,tr("Select a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html)"),&selFilter);

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

            syntaxHighlighter = new Highlighter(ui->textEdit->document());
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
                    if(cppFileVerifier() == true)
                    {
                        ui->textEdit->setPlainText(text);
                        ui->htmlSourceCheckBox->setVisible(false);
                        originalText = ui->textEdit->toPlainText();

                        syntaxHighlighter = new Highlighter(ui->textEdit->document());
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
        QMessageBox::warning(this,tr("Open File Error"),textFile.errorString());

        title = QDir::currentPath();
    }

    this->setWindowTitle(title);

    isSaved = false;

    textFile.close();

    dialog.close();
}

void MainWindow::on_actionSave_triggered()
{
    if(path.isEmpty())
    {
        path = dialog.getSaveFileName(this,tr("Save a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html);;Odf Files (*.odf)"),&selFilter);

        if(QFileInfo(path).suffix().isEmpty() == true)
        {
            if(QString::compare(selFilter,"Text Files (*.txt)") == 0){ path.append(".txt"); }
            else if(QString::compare(selFilter,"Html Files (*.html)") == 0){ path.append(".html"); }
            else if(QString::compare(selFilter,"Odf Files (*.odf)") == 0){ path.append(".odf"); }
        }
    }

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
            QMessageBox::warning(this,tr("Save File Error"),textFile.errorString());

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
        QMessageBox::warning(this,tr("Save File Error"),tr("Save canceled"));


    dialog.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    path = dialog.getSaveFileName(this,tr("Save a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html);;Odf Files (*.odf)"),&selFilter);

    if(path.isEmpty())
        return;

    if(QFileInfo(path).suffix().isEmpty() == true)
    {
        if(QString::compare(selFilter,"Text Files (*.txt)") == 0){ path.append(".txt"); }
        else if(QString::compare(selFilter,"Html Files (*.html)") == 0){ path.append(".html"); }
        else if(QString::compare(selFilter,"Odf Files (*.odf)") == 0){ path.append(".odf"); }
    }

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
        QMessageBox::warning(this,tr("Save File Error"),textFile.errorString());

        if(path.isEmpty())
            title = QDir::currentPath();
        else
            title = path;
    }

    this->setWindowTitle(title);

    textFile.close();

    dialog.close();
}

void MainWindow::on_actionExport_to_Formatting_Txt_File_triggered()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,tr("Formatting TextFile export"),tr("When exporting the formatted text file, you can only open it with Clevit "
                                                               "\nor other software that can read html code. "
                                                               "\nAre you sure you want to continue?\n"),QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if(res == QMessageBox::No)
        return;
    else
        if(res == QMessageBox::Yes)
        {

            path = QFileDialog::getSaveFileName(this,tr("Export File to Formatting Text File"),QDir::currentPath(), tr("All Files (*.*);;Text Files (*.txt)"),&selFilter);

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
    path = dialog.getSaveFileName(this,tr("Save a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);;Html Files (*.html);;Odf Files (*.odf)"),&selFilter);


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

    dialog.close();

}


bool MainWindow::htmlFileVerifier()
{
    cppOpened = true;

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

bool MainWindow::cppFileVerifier()
{
    cppOpened = true;

    if(QString::compare(QFileInfo(path).suffix(),"cpp") == 0)
        return true;

    return false;
}

void MainWindow::saveSettings()
{
    QDir createPath;

    createPath.mkdir("Clevit_Files");

    QFile saveSets(createPath.absolutePath()+"/Clevit_Files/lset.txt");

    if(saveSets.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textAppend(&saveSets);

        if(ui->search_TextEdit->isVisible())
            textAppend << "1\n";
        else
            textAppend << "0\n";

        if(ui->translateBtn->isVisible())
            textAppend << "1\n";
        else
            textAppend << "0\n";

        if(this->styleSheet() == "")
            textAppend << "0\n";
        else
            if(theme.isEmpty() == true)
                textAppend << this->styleSheet()+"\n";
            else
                textAppend << theme+"\n";

        if(txtEditColor.isEmpty() == true)
            textAppend << "0\n";
        else
            textAppend << txtEditColor+"\n";
    }
    else
    {
        QMessageBox::warning(this,tr("Error Saving Settings"),tr("Clevit can't save your settings"));
        return;
    }

    saveSets.close();
}

void MainWindow::setSettings()
{
    QFile setSets("Clevit_Files/lset.txt");

    if(setSets.exists())
        if(setSets.open((QIODevice::ReadOnly | QIODevice::Text)))
        {
            QString str = setSets.readAll();

            int i, j = 0;
            QChar ch;
            QString style;

            if(str[0] == '1')
            {
                ui->search_TextEdit->setVisible(true);
                ui->searchBtn->setVisible(true);
                ui->clearBtn->setVisible(true);
                ui->replaceBtn->setVisible(true);

                ui->actionHide_WordFinder->setText(tr("Hide WordFinder"));
            }
            else
                if(str[0] == '0')
                {
                    ui->search_TextEdit->setVisible(false);
                    ui->searchBtn->setVisible(false);
                    ui->clearBtn->setVisible(false);
                    ui->replaceBtn->setVisible(false);

                    ui->actionHide_WordFinder->setText(tr("Show WordFinder"));
                }

            if(str[2] == '1')
            {
                ui->label->setVisible(true);
                ui->label_2->setVisible(true);
                ui->fromLangBox->setVisible(true);
                ui->toLangBox->setVisible(true);
                ui->translateBtn->setVisible(true);

                ui->actionHide_Translation_bar->setText(tr("Hide Translation"));
            }
            else
                if(str[2] == '0')
                {
                    ui->label->setVisible(false);
                    ui->label_2->setVisible(false);
                    ui->fromLangBox->setVisible(false);
                    ui->toLangBox->setVisible(false);
                    ui->translateBtn->setVisible(false);

                    ui->actionHide_Translation_bar->setText(tr("Show Translation"));
                }
            i = 4;
            ch = str[i];

            for(;ch != '\n';i++,j++)
            {
                ch = str[i];

                style.insert(j,str[i]);
            }

            if(QString::compare(style,"Wood\n") == 0)
            {
                this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));");
                ui->fontComboBox->setStyleSheet("background-color: rgb(245, 121, 0);");
                ui->fontSizeBox->setStyleSheet("background-color: rgb(245, 121, 0);");
                ui->Toolbar->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));");

            }
            else
                if(QString::compare(style,"Wave\n") == 0)
                {
                    this->setStyleSheet("background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(0, 169, 255, 147), stop:0.497326 rgba(0, 0, 0, 147), stop:1 rgba(0, 169, 255, 147));");
                    ui->fontComboBox->setStyleSheet("background-color: rgb(114, 159, 207);");
                    ui->fontSizeBox->setStyleSheet("background-color: rgb(114, 159, 207);");
                    ui->Toolbar->setStyleSheet("background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(0, 169, 255, 147), stop:0.497326 rgba(0, 0, 0, 147), stop:1 rgba(0, 169, 255, 147));");

                }
                else
                    if(QString::compare(style,"Rainbow\n") == 0)
                    {
                        this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
                        ui->fontComboBox->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
                        ui->fontSizeBox->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
                        ui->Toolbar->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");

                    }
                    else
                        this->setStyleSheet(style);

            if(str[i] == '0')
                ;
            else
            {
                j = 0;

                for(;i < str.length();i++,j++)
                {
                    if(str[i] == '\n')
                        continue;
                    else
                        txtEditColor.insert(j,str[i]);
                }

                ui->textEdit->setStyleSheet(QString("background-color: %1").arg(txtEditColor));
            }
        }

    setSets.close();
}
