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
    QStringList fileNames = dir.entryList(QStringList("TranslationClevit_*.qm"));

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

    ///Translate the buttons of Clevit

    if(filename.contains("de") == true)
    {
        if (qtTranslator.load("qt_de.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            std::cout << "Qt translations loaded" << std::endl;
            qApp->installTranslator(&qtTranslator);
        }

        if (qtBaseTranslator.load("qtbase_de.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
                std::cout << "QtBase translations loaded" << std::endl;
                qApp->installTranslator(&qtBaseTranslator);
        }
    }
    else
        if(filename.contains("pt") == true)
        {
            if (qtTranslator.load("qt_pt.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
             {
                 std::cout << "Qt translations loaded" << std::endl;
                 qApp->installTranslator(&qtTranslator);
             }

             if (qtBaseTranslator.load("qtbase_pt.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
             {
                 std::cout << "QtBase translations loaded" << std::endl;
                 qApp->installTranslator(&qtBaseTranslator);
             }
        }
        else
        {
            if (qtTranslator.load("qt_en.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            {
                std::cout << "Qt translations loaded" << std::endl;
                qApp->installTranslator(&qtTranslator);
            }

            if (qtBaseTranslator.load("qtbase_en.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            {
                    std::cout << "QtBase translations loaded" << std::endl;
                    qApp->installTranslator(&qtBaseTranslator);
            }
        }
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage)
    {
        m_currLang = rLanguage;
        QLocale::setDefault(QLocale(m_currLang));
        switchTranslator(m_translator, QString("TranslationClevit_%1.qm").arg(rLanguage));
    }
}

/// Translations slot

void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action)
    {
      // load the language dependant on the action content
      loadLanguage(action->data().toString());
      setWindowIcon(QIcon(":/icons/icons/icon.png"));
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

/// Real Time Translation

void MainWindow::setLanguages()
{
    if (languages.size()==0)
    {
            languages[tr("French")] = tr("fr");
            languages[tr("Spanish")] = tr("es");
            languages[tr("Afrikaans")] = tr("af");
            languages[tr("Albanian")] = tr("sq");

            languages[tr("Arabic")] = tr("ar");
            languages[tr("Belarusian")] = tr("be");
            languages[tr("Bulgarian")] = tr("bg");
            languages[tr("Catalan")] = tr("ca");
            languages[tr("Chinese")] = tr("zh-cn");

            languages[tr("Croatian")] = tr("hr");
            languages[tr("Czech")] = tr("cs");
            languages[tr("Danish")] = tr("da");
            languages[tr("Dutch")] = tr("nl");
            languages[tr("English")] = tr("en");
            languages[tr("Estonian")] = tr("et");

            languages[tr("Filipino")] = tr("tl");
            languages[tr("Finnish")] = tr("fi");
            languages[tr("French")] = tr("fr");
            languages[tr("Galician")] = tr("gl");
            languages[tr("German")] = tr("de");
            languages[tr("Greek")] = tr("el");

            languages[tr("Haitian creole alpha")] = tr("ht");
            languages[tr("Hebrew")] = tr("iw");
            languages[tr("Hindi")] = tr("hi");
            languages[tr("Hungarian")] = tr("hu");
            languages[tr("Icelandic")] = tr("is");

            languages[tr("Indonesian")] = tr("id");
            languages[tr("Irish")] = tr("ga");
            languages[tr("Italian")] = tr("it");
            languages[tr("Japanese")] = tr("ja");
            languages[tr("Korean")] = tr("ko");

            languages[tr("Latvian")] = tr("lv");
            languages[tr("Lithuanian")] = tr("lt");
            languages[tr("Macedonian")] = tr("mk");
            languages[tr("Malay")] = tr("ms");
            languages[tr("Maltese")] = tr("mt");

            languages[tr("Norwegian")] = tr("no");
            languages[tr("Persian")] = tr("fa");
            languages[tr("Polish")] = tr("pl");
            languages[tr("Portuguese")] = tr("pt");
            languages[tr("Romanian")] = tr("ro");

            languages[tr("Russian")] = tr("ru");
            languages[tr("Serbian")] = tr("sr");
            languages[tr("Slovak")] = tr("sk");
            languages[tr("Slovenian")] = tr("sl");
            languages[tr("Spanish")] = tr("es");

            languages[tr("Swahili")] = tr("sw");
            languages[tr("Swedish")] = tr("sv");
            languages[tr("Thai")] = tr("th");
            languages[tr("Turkish")] = tr("tr");
            languages[tr("Ukrainian")] = tr("uk");

            languages[tr("Vietnamese")] = tr("vi");
            languages[tr("Welsh")] = tr("cy");
            languages[tr("Yiddish")]= tr("yi");
        }

        ui->fromLangBox->addItems(languages.keys());
        ui->toLangBox->addItems(languages.keys());

        ui->fromLangBox->setCurrentIndex(ui->fromLangBox->findText(tr("English")));
        ui->toLangBox->setCurrentIndex(ui->toLangBox->findText(tr("English")));
}

void MainWindow::realTimeTranslation()
{
    nam = new QNetworkAccessManager(this);

    QStringList words = ui->textEdit->toPlainText().split(QRegExp("\\s+"));
    QString gtUrl = tr("http://translate.google.com/translate_t?langpair=%1|%2&text=").arg(languages[ui->fromLangBox->currentText()]).arg(languages[ui->toLangBox->currentText()]) + words.join("+");

    QUrl url(gtUrl);

    QNetworkRequest qnr(url);

    qnr.setRawHeader("User-Agent", "");

    // Make the request.
    nam->get(qnr);

    connect(nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

    ui->textEdit->selectAll();
    ui->textEdit->setFocus();
}

void MainWindow::finishedSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QRegExp rx(tr("input type=hidden name=mainwindow value=\"(.*)\"><div id=sug_exp>"));
        QString rep(reply->readAll());
        rep.replace("\n", " ");

        if (rx.indexIn(rep) != -1)
        {
            QString tmp = rx.cap(1);
            tmp.replace(tr("&amp;"), tr("&"));

            ui->textEdit->clear();
            ui->textEdit->setHtml(tmp);

        }
    }

    reply->deleteLater();
    nam->deleteLater();
}
