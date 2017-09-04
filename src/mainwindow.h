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

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontSizeBox_currentIndexChanged(int index);

    void slotLanguageChanged(QAction* action); // this slot is called by the language menu actions

private:
    Ui::MainWindow *ui;

    bool isSaved;
    bool changedTitle;

    QString path; // Path of text file
    QString text; // Text of text file
    QString originalText;
    QString selFilter; // Text file extension filter
    QString title;

    QTranslator m_translator; // contains the translations for this application
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.

    QActionGroup* langGroup;

    QTextCharFormat format;

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
