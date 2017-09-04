#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(this)
{
    ui->setupUi(this);

    createLanguageMenu();

    ui->fontComboBox->setEditable(false);

    text = ui->textEdit->toPlainText();

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
    //Connect signals to slots

    connect(ui->textEdit,&QTextEdit::textChanged,this,&MainWindow::textChanged);
    connect(ui->boldBtn,&QToolButton::clicked,this,&MainWindow::bold);
    connect(ui->italicBtn,&QToolButton::clicked,this,&MainWindow::italic);
    connect(ui->underlineBtn,&QToolButton::clicked,this,&MainWindow::underline);
    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));
}


void MainWindow::textChanged()
{
    disconnect(ui->textEdit,&QTextEdit::textChanged,this,&MainWindow::textChanged);

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
    int res = QString::compare(originalText,text,Qt::CaseSensitive);

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    text = ui->textEdit->toPlainText();

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

void MainWindow::on_actionNew_File_triggered()
{
    path.clear();
    text.clear();
    originalText.clear();
    ui->textEdit->clear();

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

        ui->textEdit->setHtml(text);

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

        text = ui->textEdit->toHtml();

        QTextStream textAppend(&textFile);

        textAppend << text;

        originalText = text;

        title = path;

        text.clear();
    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        title = QDir::currentPath();
    }

    textFile.close();

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

        text = ui->textEdit->toHtml();

        QTextStream textAppend(&textFile);

        textAppend << text;

        originalText = text;

        title = path;

        text.clear();
    }
    else
    {
        QMessageBox::warning(this,"Open File Error",textFile.errorString());

        title = QDir::currentPath();
    }

    textFile.close();

    this->setWindowTitle(title);

    isSaved = true;
    changedTitle = false;
}

void MainWindow::on_actionAbout_TPad_triggered()
{
    About *about = new About(this);

    about->show();
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont ft;

    ft = f;

    ft.setPointSize(18);

    ui->textEdit->setFont(ft);
}

void MainWindow::bold()
{
    disconnect(ui->boldBtn,&QToolButton::clicked,this,&MainWindow::bold);

    QTextCharFormat format;

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
    disconnect(ui->italicBtn,&QToolButton::clicked,this,&MainWindow::italic);

    QTextCharFormat format;

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
    disconnect(ui->underlineBtn,&QToolButton::clicked,this,&MainWindow::underline);

    QTextCharFormat format;

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

/// Translate functions and slots

void MainWindow::createLanguageMenu(void)
{
    langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    // format systems language
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/src/languages/"); //

    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("TranslationTPad_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);

         QString lang = QLocale::languageToString(QLocale(locale).language());
         QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

         QAction *action = new QAction(ico, lang, this);
         action->setCheckable(true);
         action->setData(locale);

         ui->menuLanguage->addAction(action);
         langGroup->addAction(action);

         // set default translators and language checked
         if (defaultLocale == locale)
            action->setChecked(true);
     }
}

void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action)
    {
      // load the language dependant on the action content
      loadLanguage(action->data().toString());
      setWindowIcon(action->icon());
    }
}

void MainWindow::switchTranslator(QTranslator& translator, const QString& filename)
{
    qApp->removeTranslator(&translator);

    // load the new translator
     if(translator.load(QString(QApplication::applicationDirPath()+"/src/languages/%1").arg(filename))) //
     {
         qApp->installTranslator(&translator);

         std::cout << "Loaded translations successfuly!" << std::endl;
         std::cout << "Path: " << (QString(QApplication::applicationDirPath()+"/src/languages/%1").arg(filename)).toStdString() << std::endl;
     }
    else
        std::cout << "Cannot load translations." << std::endl;
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage)
    {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("TranslationTPad_%1.qm").arg(rLanguage));
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event)
    {
        if(event->type() == QEvent::LanguageChange)
            ui->retranslateUi(this);

        else
            if(event->type() == QEvent::LocaleChange)
            {
                QString locale = QLocale::system().name();
                locale.truncate(locale.lastIndexOf('_'));
                loadLanguage(locale);
            }
    }

    QMainWindow::changeEvent(event);
}
