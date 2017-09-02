#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "about.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QTimer>
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
    void switchTranslator(QTranslator& translator,const QString& filename);

protected:
    void changeEvent(QEvent *) override;

protected slots:
    void slotLanguageChanged(QAction* action);

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

    void on_timeout(); // Signal and slots in real time

    void on_fontComboBox_currentFontChanged(const QFont &f);

private:
    Ui::MainWindow *ui;

    bool isSaved;
    bool changedTitle;

    QString path; // Path of text file
    QString text; // Text of text file
    QString originalText;
    QString selFilter; // Text file extension filter
    QString title;
    QString m_currLang;
    QString m_langPath;

    QActionGroup* langGroup;
    QTranslator m_translator;
    QTranslator m_translatorQt;


    QTimer timer;

    bool fileNotChanged(); // Verify if the text of the open file was modified or not
    void createLanguageMenu(void);
    void loadLanguage(const QString& rLanguage);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
