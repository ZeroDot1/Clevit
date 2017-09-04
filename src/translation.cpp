#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Translate functions

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
        QLocale::setDefault(QLocale(m_currLang));
        switchTranslator(m_translator, QString("TranslationTPad_%1.qm").arg(rLanguage));
    }
}

/// Translations slot

void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action)
    {
      // load the language dependant on the action content
      loadLanguage(action->data().toString());
      setWindowIcon(action->icon());
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
