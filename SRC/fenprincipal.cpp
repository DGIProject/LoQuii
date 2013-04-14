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


#include "fenprincipal.h"
#include "fenmacumba.h"
#include "ui_fenprincipal.h"
#include "fenmacumba.h"
#include "apropos.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QtSql>
#include <QMovie>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

using namespace std;


FenPrincipal::FenPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FenPrincipal)
{
    ui->setupUi(this);

    ui->ButtonConnection->setShortcut(QKeySequence(Qt::Key_Return));

    setWindowIcon(QIcon("icon_loquii.png"));

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("mysql2.alwaysdata.com");
    db.setUserName("pox_site");
    db.setPassword("topsecret");
    db.setDatabaseName("pox_base");
    query_base = new QSqlQuery(db);

    QSettings settings("Pox", "LoQuii");

    if(settings.value("versionprogramme").toString() == "")
    {
        qDebug() << "Registre";

        QSettings options("Pox", "LoQuii");

        options.setValue("versionprogramme", "100");
        options.setValue("lancerloquiiaudemarragedewindows", false);
        options.setValue("activerlesmisesajour", true);

        options.beginGroup("Chat");
        options.setValue("montrerquandjecris", true);
        options.setValue("afficherlesnotificationspourlesmessages", true);
        options.endGroup();

        options.beginGroup("Audio");
        options.setValue("peripheriqueaudio", "Aucun peripherique audio");
        options.setValue("volumeaudio", "20");
        options.setValue("qualiteaudio", "80");
        options.setValue("enregistrementaudio", false);
        options.endGroup();

        options.beginGroup("Video");
        options.setValue("peripheriquevideo", "Aucun peripherique video");
        options.setValue("qualitevideo", "60");
        options.setValue("demarrerautovideo", false);
        options.endGroup();

    }
    else
    {
        QSettings options("Pox", "LoQuii");

        setWindowTitle("LoQuiî - " + options.value("versionprogramme").toString());
    }

    if(db.open())
    {
        QSettings options("Pox", "LoQuii");

        // ########################################################################
        // Fonction pour vérifier les mises à jour
        // ########################################################################

        /*query_base->prepare("SELECT version FROM update_prog WHERE version > ?");
        query_base->addBindValue(QString(options.value("versionprogramme").toString()));

         if(query_base->exec())
         {
             while(query_base->next())
             {
               if (query_base->record().value("version").isNull() == false)
               {
                    int question = QMessageBox::question(this,"Question",tr("Une nouvelle mise à jour est disponible, voulez-vous la télécharger ?"), QMessageBox::Yes | QMessageBox::No);

                    if(question = QMessageBox::Yes)
                    {
                        QProcess::execute("cmd /c start http://pox.alwaysdata.net/programs.php?p=loquii");
                    }
                    else
                    {

                    }
               }
             }
         }*/

        // ########################################################################
        // Fonction pour se connecter au demarrage de LoQuii !
        // ########################################################################

        query_base->prepare("SELECT * FROM loquii WHERE pseudo = ?");
        query_base->addBindValue(QString(options.value("user").toString()));

         if(query_base->exec())
         {
             while(query_base->next())
             {
               if (query_base->record().value("co_start").toInt() == 1)
               {
                   ConnectionAuto = 1;

                   ui->FenConnection->setEnabled(false);
                   ui->Pseudo->setText(QString(query_base->record().value("pseudo").toString()));
                   ui->Password->setText(QString(options.value("password").toString()));
                   ui->CheckSaveIdentifiant->setChecked(true);

                   FenPrincipal::on_ButtonConnection_clicked();
               }
             }
         }

    db.close();
    }
}

void FenPrincipal::on_ButtonConnection_clicked()
{
    ui->statusBar->showMessage(tr("Connexion en cours ..."));

    if (ui->Pseudo->text() == "" || ui->Password->text() == "")
    {
        ui->statusBar->showMessage(tr("Erreur : Vous devez remplir des identifiants"));
    }
    else
    {
        QString PseudoConnexion = ui->Pseudo->text();

        if(ConnectionAuto != 1)
        {
            QString Password = ui->Password->text();
            QByteArray ba = QCryptographicHash::hash(Password.toUtf8(), QCryptographicHash::Sha1); // on la hache
            PasswordHache=ba.toHex(); // on convertit le hash en QString
        }
        else
        {
            PasswordHache = ui->Password->text();
        }

        if (db.open())
        {

            query_base->prepare("SELECT * FROM user WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

                    if(query_base->exec())
                    {
                        while(query_base->next())
                        {
                            for(int x=0; x < 1; ++x)
                            {
                                if(query_base->record().value("pseudo").toString() == "" && query_base->record().value("password").toString() == "")
                                {
                                     ui->statusBar->showMessage(tr("Erreur : Vous n'êtes pas inscrit"));

                                     ui->Pseudo->clear();
                                     ui->Password->clear();

                                     ui->Pseudo->setFocus();
                                }
                                else
                                {
                                    if (PasswordHache == query_base->record().value("password").toString() && PseudoConnexion == query_base->record().value("pseudo").toString())
                                    {
                                        ui->statusBar->showMessage(tr("Chargement des données en cours ..."));

                                        QSettings options("Pox", "LoQuii");
                                        options.setValue("user", ui->Pseudo->text());
                                        options.setValue("password", PasswordHache);

                                        if(ui->CheckSaveIdentifiant->isChecked() == true)
                                        {
                                            query_base->prepare("UPDATE loquii SET co_start = '1' WHERE pseudo = ?");
                                            query_base->addBindValue(ui->Pseudo->text());

                                            query_base->exec();
                                        }
                                        else
                                        {
                                            query_base->prepare("UPDATE loquii SET co_start = '0' WHERE pseudo = ?");
                                            query_base->addBindValue(ui->Pseudo->text());

                                            query_base->exec();
                                        }


                                        FenMacumba *d = new FenMacumba(this);
                                        d->show();

                                        ui->statusBar->showMessage(tr("Connexion réussit !"));
                                    }
                                    else
                                    {
                                        ui->statusBar->showMessage(tr("ERREUR : Votre pseudo ou mot de passe est incorrect"));

                                        ui->Password->clear();
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        ui->statusBar->showMessage(tr("ERREUR : Requette impossible"));
                    }

            db.close();
            }
            else
            {
                ui->statusBar->showMessage(tr("ERREUR : Connection impossible à la base de donnée"));
            }
    }
}

void FenPrincipal::on_ButtonForgotIdentifiant_clicked()
{
    QProcess::execute("cmd /c start http://pox.alwaysdata.net/newpassword.php");
}

void FenPrincipal::on_ButtonRegister_clicked()
{
    QProcess::execute("cmd /c start http://pox.alwaysdata.net/register.php");
}

void FenPrincipal::on_MBRegister_triggered()
{
    QProcess::execute("cmd /c start http://pox.alwaysdata.net/register.php");
}

void FenPrincipal::on_MBUpdate_triggered()
{
    if(db.open())
    {
        QSettings options("Pox", "LoQuii");

        query_base->prepare("SELECT version FROM update_prog WHERE version > ?");
        query_base->addBindValue(QString(options.value("versionprogramme").toString()));

         if(query_base->exec())
         {
             while(query_base->next())
             {
               if (query_base->record().value("version").isNull() == false)
               {
                    QProcess::execute("cmd /c start http://pox.alwaysdata.net/programs.php?p=loquii&a=dl");
               }
               else
               {
                   QString message = "La version " + options.value("versionprogramme").toString() + " est la version à jour pour le moment.";
                   QMessageBox::information(this,"Information", message);
               }
             }
         }

    db.close();
    }
}

void FenPrincipal::on_MBHelp_triggered()
{
    QProcess::execute("cmd /c start http://loquii.alwaysdata.net/Pox/desktop/faq.php");
}

void FenPrincipal::on_MBAPropos_triggered()
{
    apropos *d = new apropos(this);
    d->show();
}

FenPrincipal::~FenPrincipal()
{
    delete ui;
}
