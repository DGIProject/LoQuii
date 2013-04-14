/*

    Loquîî is a chat program who can help you to speak by writing to a friend or with someone else
    Copyright (C) 2012  Guillaume Villena Dylan Delaporte

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/

#ifndef FENPRINCIPAL_H
#define FENPRINCIPAL_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include <QMovie>

namespace Ui {
class FenPrincipal;
}

class FenPrincipal : public QMainWindow
{
    Q_OBJECT  

public:
    explicit FenPrincipal(QWidget *parent = 0);
    ~FenPrincipal();

private slots:
    void on_ButtonConnection_clicked();
    void on_ButtonForgotIdentifiant_clicked();
    void on_ButtonRegister_clicked();

    void on_MBRegister_triggered();
    void on_MBUpdate_triggered();
    void on_MBHelp_triggered();
    void on_MBAPropos_triggered();

public :
    //Connection
    int ConnectionAuto;
    QString PseudoRech;
    QString MotDePasseRech;
    QString PasswordHache;

    QSqlDatabase db;
    QSqlQuery* query_base;

    int entier;

private:
    Ui::FenPrincipal *ui;


};

#endif // FENPRINCIPAL_H
