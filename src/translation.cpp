
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
