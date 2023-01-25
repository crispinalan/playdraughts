/***************************************************************************
 *   Author Alan Crispin                 *
 *   crispinalan@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Play Draughts");
    setWindowIcon(QIcon(":/icons/icon.png"));

    QFont appfont = QApplication::font();
    appfont.setPixelSize(18); //DPI
    QApplication::setFont(appfont);

    statusBarLabel =new QLabel(this);
    statusBarLabel->setText("White to move.");
    ui->statusBar->insertPermanentWidget(0,statusBarLabel,2);

    connect(this->ui->widget,
            SIGNAL(outgoingText(QString)),this,SLOT(incomingTextSlot(QString)));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incomingTextSlot(QString str){
  //qDebug()<<"MainWindow: incomingTextslot str = "<<str;
  statusBarLabel->setText(str);
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionNew_Game_triggered(){
  this->ui->widget->newGame();
}



void MainWindow::on_actionReset_Player_triggered() {

  this->ui->widget->resetPlayer();

}



void MainWindow::on_actionAbout_triggered()
{
  //qDebug()<<"About draughts\n";
  QMessageBox msgBox(this);
  msgBox.setWindowTitle("About Play Draughts");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText("Play Draughts has been written using C++ and Qt. <a href='https://github.com/crispinalan/playdraughts'> Play Draughts website.</a> Alan Crispin (2023)");
  msgBox.exec();
}
