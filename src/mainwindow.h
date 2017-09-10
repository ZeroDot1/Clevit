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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "about.h"

#include <iostream>
#include <QMainWindow>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QTranslator>
#include <QActionGroup>
#include <QDesktopServices>
#include <QColorDialog>
#include <QTextCodec>
#include <QPrinter>
#include <QTextDocumentWriter>

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

    void on_actionSave_as_triggered();

    void on_actionNew_File_triggered();

    void on_actionAbout_TPad_triggered();

    void bold();

    void italic();

    void underline();

    void textChanged(); // If text change the program add a '*' in the title

    bool htmlFileVerifier();

    bool textFileVerifier();

    bool odfFileVerifier();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontSizeBox_currentIndexChanged(int index);

    void slotLanguageChanged(QAction* action); // this slot is called by the language menu actions

    void on_actionReport_Bugs_triggered();

    void on_colorBtn_clicked();

    void on_htmlSourceCheckBox_stateChanged(int arg1);

    void on_actionExport_to_PDF_triggered();

    void on_actionExport_to_Formatting_Txt_File_triggered();

private:
    Ui::MainWindow *ui;
    About *about;

    bool isSaved;
    bool changedTitle;

    QString path; // Path of text file
    QString text; // Text of text file
    QString originalText;
    QString selFilter; // Text file extension filter
    QString title;
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.
    QString colorBtn_str; // convert QColor in QString to create a stylesheet
    QString tmp; // A auxilar qstring to verify if a file is or not a Html file

    QTranslator m_translator; // contains the translations for this application

    QActionGroup* langGroup;

    QTextCharFormat format;

    QByteArray data;

    QTextCodec *codec;

    QColor color;

    QPrinter printer;

    QTextDocumentWriter writer;

    QFont font;

    bool fileNotChanged(); // Verify if the text of the open file was modified or not
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void configFontSizeBox();
    void loadLanguage(const QString& rLanguage); // loads a language by the given language shortcur (e.g. de, en)
    void createLanguageMenu(void); // creates the language menu dynamically from the content of m_langPath
    void switchTranslator(QTranslator& translator, const QString& filename);
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent*);// this event is called, when a new translator is loaded or the system language is changed
};

#endif // MAINWINDOW_H
