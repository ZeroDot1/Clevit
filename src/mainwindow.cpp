
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->htmlSourceCheckBox->setVisible(false);

    createLanguageMenu();

    configFontSizeBox();

    ui->fontSizeBox->setCurrentText("12");

    ui->fontComboBox->setEditable(false);

    font.setStyleHint(QFont::Monospace);
    font.setStyleName("Monospace");

    ui->fontComboBox->setFont(font);
    ui->fontComboBox->setCurrentText("Monospace");

    ui->textEdit->setFont(font);
    ui->textEdit->setTextColor(Qt::black);
    ui->colorBtn->setStyleSheet("background-color: black");

    text = ui->textEdit->toPlainText();

    about = NULL;

    isSaved = false, changedTitle = false, firstTime = true;

    selFilter = tr("Text Files (*.txt);; Html File(*.html)");

    title = QDir::currentPath();

    this->setWindowTitle(title);

    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(ui->boldBtn,SIGNAL(clicked(bool)),this,SLOT(bold()));
    connect(ui->italicBtn,SIGNAL(clicked(bool)),this,SLOT(italic()));
    connect(ui->underlineBtn,SIGNAL(clicked(bool)),this,SLOT(underline()));
    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::textChanged()
{
   isSaved = false;

    text = ui->textEdit->toPlainText();

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
    int res = QString::compare(text,originalText,Qt::CaseSensitive);

    if(res == 0)
        return true;
    else
        return false;

}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection() == true)
    {
        cursor.mergeCharFormat(format);

        ui->textEdit->mergeCurrentCharFormat(format);
    }
}


void MainWindow::configFontSizeBox()
{
    for(int i = 1;i <= 70;i++)
    {
        if(i >= 18)
        {
            i = i+4;

            ui->fontSizeBox->addItem(QString::number(i));
        }
        else
            ui->fontSizeBox->addItem(QString::number(i));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    text = ui->textEdit->toPlainText();

    // If the user doesn't has type text or open any text file, so the program doesn't show the message box

    if(fileNotChanged() == false)
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

/// QAction Slots and Signals

void MainWindow::on_actionCopy_triggered()
{
    // Copy

    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    // Paste

    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    // Cut

    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    // Undo

    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    // Redo

    ui->textEdit->redo();
}

void MainWindow::on_actionExit_triggered()
{
    // Quit

    QApplication::quit();
}

void MainWindow::on_actionAbout_TPad_triggered()
{
    if(about == NULL)
    {
        about = new About(this);

        about->show();
    }
    else
        about->show();
}

void MainWindow::on_actionReport_Bugs_triggered()
{
    if(QDesktopServices::openUrl(QUrl("https://github.com/TigaxMT/TPad/issues")) == false)
        QMessageBox::warning(this,"Error opening the link","TPad could not open link ");
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    font = f;

    font.setPixelSize(QString(ui->fontSizeBox->currentText()).toInt());

    format.setFont(font);

    if(ui->boldBtn->isChecked() == true)
        format.setFontWeight(QFont::Bold);
    if(ui->italicBtn->isChecked() == true)
        format.setFontItalic(true);
    if(ui->underlineBtn->isChecked() == true)
        format.setFontUnderline(true);

    ui->textEdit->mergeCurrentCharFormat(format);

    mergeFormatOnWordOrSelection(format);

    f.cleanup();
}

void MainWindow::on_fontSizeBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    font.setPixelSize(QString(ui->fontSizeBox->currentText()).toInt());

    format.setFont(font);

    if(ui->boldBtn->isChecked() == true)
        format.setFontWeight(QFont::Bold);
    if(ui->italicBtn->isChecked() == true)
        format.setFontItalic(true);
    if(ui->underlineBtn->isChecked() == true)
        format.setFontUnderline(true);

    ui->textEdit->mergeCurrentCharFormat(format);

    mergeFormatOnWordOrSelection(format);
}


void MainWindow::on_colorBtn_clicked()
{
    color = QColorDialog::getColor(Qt::black,this,"Select a Font Color");

    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);

        colorBtn_str = QString("background-color: %1").arg(color.name());

        ui->colorBtn->setStyleSheet(colorBtn_str);
    }
    else
    {
        ui->textEdit->setTextColor(Qt::black);
        ui->colorBtn->setStyleSheet("background-color: black");
    }

}


void MainWindow::bold()
{
    if(ui->boldBtn->isChecked() == true)
    {
        format.setFontWeight(QFont::Bold);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
    else
    {
        format.setFontWeight(QFont::Normal);
        ui->textEdit->mergeCurrentCharFormat(format);
    }

    mergeFormatOnWordOrSelection(format);
}

void MainWindow::italic()
{
    if(ui->italicBtn->isChecked() == true)
    {
        format.setFontItalic(true);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
    else
    {
        format.setFontItalic(false);
        ui->textEdit->mergeCurrentCharFormat(format);
    }

    mergeFormatOnWordOrSelection(format);
}

void MainWindow::underline()
{
    if(ui->underlineBtn->isChecked() == true)
    {
        format.setFontUnderline(true);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
    else
    {
        format.setFontUnderline(false);
        ui->textEdit->mergeCurrentCharFormat(format);
    }

    mergeFormatOnWordOrSelection(format);
}
void MainWindow::on_htmlSourceCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);


    if(ui->htmlSourceCheckBox->isChecked() == true)
    {
        text = ui->textEdit->toPlainText();

        tmp = text;

        ui->textEdit->setHtml(text);
    }
    else
    {
        text = tmp;

        data = text.toUtf8();

        codec = Qt::codecForHtml(data);

        text = codec->toUnicode(data);

        ui->textEdit->setPlainText(text);
    }

    text.clear();
    tmp.clear();
}

void MainWindow::on_actionPrint_triggered()
{
    printer.setPageSize(QPrinter::Letter);
    printer.setColorMode(QPrinter::Color);
    printer.setFullPage(true);
    printer.setResolution(96);
    printer.setPageMargins(1.0,1.0,1.0,1.0,QPrinter::Inch);

    QPrintDialog dialog(&printer,this);

    dialog.setWindowTitle(tr("Print a document"));

    if(dialog.exec() != QDialog::Accepted)
        return;

    ui->textEdit->document()->print(&printer);

    dialog.close();
}
void MainWindow::on_searchBtn_clicked()
{
    if(firstTime == false)
    {
        document->undo();
        firstTime = true;
    }

        firstTime = false;

        QString searchString = ui->search_TextEdit->toPlainText();
        document = ui->textEdit->document();

        if (searchString.isEmpty())
        {
            QMessageBox::information(this, tr("Empty Search Field"),
                    "The search field is empty. Please enter a word and click Find.");
        }
        else
        {

            QTextCursor highlightCursor(document);
            QTextCursor cursor(document);

            cursor.beginEditBlock();

            QTextCharFormat plainFormat(highlightCursor.charFormat());
            QTextCharFormat colorFormat = plainFormat;
            colorFormat.setBackground(Qt::yellow);

            while (!highlightCursor.isNull() && !highlightCursor.atEnd())
            {
                highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

                if (!highlightCursor.isNull())
                {
                    highlightCursor.movePosition(QTextCursor::WordRight,
                                           QTextCursor::KeepAnchor);
                    highlightCursor.mergeCharFormat(colorFormat);
                }
            }

            cursor.endEditBlock();
        }
    }
