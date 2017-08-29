/************************************************************************************************************
*    											                                                            *
*    TPad - A pratical text editor									    *
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

#include <QMainWindow>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>

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

    void textChanged();

    void on_timeout();

private:
    Ui::MainWindow *ui;

    bool isSaved;
    bool changedTitle;

    QString path; // Path of text file
    QString text; // Text of text file
    QString originalText;
    QString selFilter; // Text file extension filter
    QString title;

    QTimer timer;

    bool fileNotChanged();
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
