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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->htmlSourceCheckBox->setVisible(false);

    createLanguageMenu();
    setLanguages();

    if(QSslSocket::supportsSsl())
        std::cout << "Support SSL enabled" << std::endl;

    configFontSizeBox();

    ui->fontSizeBox->setCurrentText("12");

    ui->fontComboBox->setEditable(false);

    QFontDatabase fontDB;
    fontDB.addApplicationFont("/fonts/NotoEmoji-Regular.ttf");

    font.setStyleHint(QFont::Monospace);
    font.setStyleName("Monospace");

    ui->fontComboBox->setFont(font);
    ui->fontComboBox->setCurrentText("Monospace");

    ui->textEdit->setFont(font);
    ui->textEdit->setTextColor(Qt::black);
    ui->colorBtn->setStyleSheet("background-color: black");

    text = ui->textEdit->toPlainText();

    about = NULL;
    nam = NULL;

    isSaved = false, changedTitle = false, firstTime = true, cppOpened = false, canClear = true, replaced = false;

    selFilter = tr("Text Files (*.txt);; Html File(*.html)");

    title = QDir::currentPath();

    this->setWindowTitle(title);

    setSettings();

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

   QTextCursor cursor = ui->textEdit->textCursor();

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

    // When cursor is in col number 0 qt reset font, font size, color and other stuff.
    // This piece of code prevent this

    if(cursor.columnNumber() == 0 && cursor.hasSelection() == false)
    {
        ui->textEdit->mergeCurrentCharFormat(format);
        ui->textEdit->setTextColor(color);
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

    saveSettings();

    // If the user doesn't has type text or open any text file, so the program doesn't show the message box

    if(fileNotChanged() == false)
    {
        if(isSaved == false && changedTitle == true)
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question(this,tr("Clevit - Text Editor"),tr("Do you want to save the text file?"),QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

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

void MainWindow::on_actionAbout_Clevit_triggered()
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
    if(QDesktopServices::openUrl(QUrl("https://github.com/TigaxMT/Clevit/issues")) == false)
        QMessageBox::warning(this,"Error opening the link","Clevit could not open link ");
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
    color = QColorDialog::getColor(Qt::black,this,tr("Select a Font Color"));

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
        QString searchString = ui->search_TextEdit->text();
        document = ui->textEdit->document();

        if (searchString.isEmpty())
        {
            QMessageBox::information(this, tr("Empty Search Field"),
                    tr("The search field is empty. Please enter a word and click Find."));
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
                    canClear = true;
                    firstTime = false;
                    replaced = false;

                    highlightCursor.movePosition(QTextCursor::WordRight,
                                           QTextCursor::KeepAnchor);
                    highlightCursor.mergeCharFormat(colorFormat);
                }
            }

            cursor.endEditBlock();
        }
}

void MainWindow::on_replaceBtn_clicked()
{
    firstTime = true;

    QString searchString = ui->search_TextEdit->text();
    document = ui->textEdit->document();

    if (searchString.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Search Field"),
                tr("The search field is empty. Please enter a word and click Find."));
    }
    else
    {

        QString tmpword = QInputDialog::getText(this,tr("Replace Word"),tr("Insert a word/text to replace the\nword/text you type in WordFinder bar."));
        QString word;

        QTextCursor replaceCursor(document);
        QTextCursor cursor(document);

        cursor.beginEditBlock();

        QTextCharFormat plainFormat(replaceCursor.charFormat());

        while (!replaceCursor.isNull() && !replaceCursor.atEnd())
        {
            replaceCursor = document->find(searchString, replaceCursor, QTextDocument::FindWholeWords);

            if (!replaceCursor.isNull())
            {
                replaced = true;

                replaceCursor.movePosition(QTextCursor::WordRight,
                                       QTextCursor::KeepAnchor);
                QString word2replace = replaceCursor.selectedText();

                std::cout << word2replace.toStdString() << std::endl;

                if(QChar(word2replace[0]) == QChar(' '))
                    word = " " + tmpword;
                else
                    word = tmpword;

                if(QChar(word2replace[word2replace.length()-1]) == QChar(' '))
                    word = word + " ";
                else
                    word = word + "\n";

                replaceCursor.insertText(word,plainFormat);
            }
        }

        cursor.endEditBlock();
    }
}

void MainWindow::on_clearBtn_clicked()
{   
    if(ui->search_TextEdit->text()== "" && ui->textEdit->toPlainText() == "")
        return;

    ui->search_TextEdit->setText("");

    if(canClear == true && replaced == false)
    {
        document->undo();

        canClear = false;
        firstTime = true;
    }
}


void MainWindow::on_actionAdd_an_Image_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                      ".", tr("Bitmap Files (*.bmp)\n"
                                        "JPEG (*.jpg *jpeg)\n"
                                        "GIF (*.gif)\n"
                                        "PNG (*.png)\n"));
    QUrl Url ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader ( file ).read();

    QTextDocument * textDocument =ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Url, QVariant ( image ) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Url.toString() );
    cursor.insertImage(imageFormat);
}

void MainWindow::on_actionWindow_Layout_Color_triggered()
{
    QColor lColor = QColorDialog::getColor(Qt::white,this,tr("Select a Window Layout Color"));

    if(lColor.isValid())
    {
        this->setStyleSheet(QString("background-color: %1").arg(lColor.name()));
        ui->fontComboBox->setStyleSheet(QString("background-color: %1").arg(lColor.name()));
        ui->fontSizeBox->setStyleSheet(QString("background-color: %1").arg(lColor.name()));
        ui->Toolbar->setStyleSheet(QString("background-color: %1").arg(lColor.name()));
    }
    else
    {
        this->setStyleSheet("");
        ui->fontComboBox->setStyleSheet("");
        ui->fontSizeBox->setStyleSheet("");
        ui->Toolbar->setStyleSheet("");
    }
}

void MainWindow::on_actionNo_Theme_2_triggered()
{
    this->setStyleSheet("");
    ui->fontComboBox->setStyleSheet("");
    ui->fontSizeBox->setStyleSheet("");
    ui->Toolbar->setStyleSheet("");
}

void MainWindow::on_actionWood_Theme_2_triggered()
{
    theme = "Wood";

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));");
    ui->fontComboBox->setStyleSheet("background-color: rgb(245, 121, 0);");
    ui->fontSizeBox->setStyleSheet("background-color: rgb(245, 121, 0);");
    ui->Toolbar->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));");

}

void MainWindow::on_actionWave_Theme_2_triggered()
{
    theme = "Wave";

    this->setStyleSheet("background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(0, 169, 255, 147), stop:0.497326 rgba(0, 0, 0, 147), stop:1 rgba(0, 169, 255, 147));");
    ui->fontComboBox->setStyleSheet("background-color: rgb(114, 159, 207);");
    ui->fontSizeBox->setStyleSheet("background-color: rgb(114, 159, 207);");
    ui->Toolbar->setStyleSheet("background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(0, 169, 255, 147), stop:0.497326 rgba(0, 0, 0, 147), stop:1 rgba(0, 169, 255, 147));");

}

void MainWindow::on_actionRainbow_Theme_2_triggered()
{
    theme = "Rainbow";

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
    ui->fontComboBox->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
    ui->fontSizeBox->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
    ui->Toolbar->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");

}

void MainWindow::on_actionHide_WordFinder_triggered()
{
    if(ui->search_TextEdit->isVisible() == true)
    {
        ui->search_TextEdit->setVisible(false);
        ui->searchBtn->setVisible(false);
        ui->clearBtn->setVisible(false);

        ui->actionHide_WordFinder->setText(tr("Show WordFinder"));
    }
    else
        if(ui->search_TextEdit->isVisible() == false)
        {
            ui->search_TextEdit->setVisible(true);
            ui->searchBtn->setVisible(true);
            ui->clearBtn->setVisible(true);

            ui->actionHide_WordFinder->setText(tr("Hide WordFinder"));
        }
}

void MainWindow::on_translateBtn_clicked()
{
    realTimeTranslation();
}

void MainWindow::on_actionHide_Translation_bar_triggered()
{
    if(ui->fromLangBox->isVisible() == true)
    {
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);
        ui->fromLangBox->setVisible(false);
        ui->toLangBox->setVisible(false);
        ui->translateBtn->setVisible(false);

        ui->actionHide_Translation_bar->setText(tr("Show Translation"));
    }
    else
        if(ui->fromLangBox->isVisible() == false)
        {
            ui->label->setVisible(true);
            ui->label_2->setVisible(true);
            ui->fromLangBox->setVisible(true);
            ui->toLangBox->setVisible(true);
            ui->translateBtn->setVisible(true);

            ui->actionHide_Translation_bar->setText(tr("Hide Translation"));
        }
}

void MainWindow::on_actionReset_Default_Layout_triggered()
{
    ui->search_TextEdit->setVisible(true);
    ui->searchBtn->setVisible(true);
    ui->clearBtn->setVisible(true);

    ui->actionHide_WordFinder->setText(tr("Hide WordFinder"));

    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->fromLangBox->setVisible(true);
    ui->toLangBox->setVisible(true);
    ui->translateBtn->setVisible(true);

    ui->actionHide_Translation_bar->setText(tr("Hide Translation"));

    this->setStyleSheet("");
    ui->Toolbar->setStyleSheet("");
    ui->fontComboBox->setStyleSheet("");
    ui->fontSizeBox->setStyleSheet("");
    ui->textEdit->setStyleSheet("");
    txtEditColor.clear();
}

void MainWindow::on_actionText_Edit_Color_triggered()
{
    tColor = QColorDialog::getColor(Qt::white,this,tr("Select a Text Edit Color"));

    if(tColor.isValid())
    {
        ui->textEdit->setStyleSheet(QString("background-color: %1").arg(tColor.name()));

        txtEditColor = tColor.name();
    }
}

void MainWindow::on_highlightBtn_clicked()
{
    QColor hColor= QColorDialog::getColor(Qt::white,this,tr("Select a Highlight Color"));

    if(hColor.isValid())
    {
        ui->textEdit->setTextBackgroundColor(hColor);
    }
    else
    {
        ui->textEdit->setTextBackgroundColor(Qt::white);
    }
}
