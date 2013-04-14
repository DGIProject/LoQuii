/*

    LoquÃ®Ã® is a chat program who can help you to speak by writting to a friend or someone
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

#include "fenoptions.h"
#include "apropos.h"
#include "fenprincipal.h"
#include "fenmacumba.h"
#include "ui_fenmacumba.h"
#include <QtSql>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
#include <QtNetwork/QFtp>
#include <QFile>
#include <QFileDialog>
#include <QtGui>
#include <QHttp>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QTcpSocket>
#include <QTimer>
#include <QSystemTrayIcon>


using namespace std;

FenMacumba::FenMacumba(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FenMacumba)
{
    ui->setupUi(this);
    ui->FenProfil->hide();
    ui->FenNotifications->hide();
    ui->FenContactClick->hide();
    ui->FenContact->hide();
    ui->FenUploadImgContact->hide();
    ui->ButtonBackText->hide();
    ui->GroupBoxInfoContactPox->hide();
    ui->FenHome->show();

    ui->ButtonSexprime->setShortcut(QKeySequence(Qt::Key_Return));
    ui->ButtonBackText->setShortcut(QKeySequence(Qt::Key_Up));
    ui->ButtonSendMessage->setShortcut(QKeySequence(Qt::Key_Return));


    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QAction *actionQuitter = menuFichier->addAction("Quitter");
    ui->ButtonNotifications->setMenu(menuFichier);

    ui->webView->load(QUrl("http://pox.alwaysdata.net/program/event.php"));

    ui->fenaddnot->hide();

    ui->smile1->setIcon(QIcon("heureux.png"));
    ui->smile2->setIcon(QIcon("blink.gif"));
    ui->smile3->setIcon(QIcon("clin.png"));
    ui->smile4->setIcon(QIcon("langue.png"));
    ui->smile5->setIcon(QIcon("smile.png"));
    ui->smile6->setIcon(QIcon("siffle.png"));
    ui->smile7->setIcon(QIcon("mechant.png"));

    setWindowTitle("LoQuiî");
    setWindowIcon(QIcon("icon_loquii.png"));

// ########################################################################
// Connection à la base de données !
// ########################################################################

    dbd = QSqlDatabase::addDatabase("QMYSQL");
    dbd.setHostName("mysql2.alwaysdata.com");
    dbd.setUserName("pox_site");
    dbd.setPassword("topsecret");
    dbd.setDatabaseName("pox_base");
    query_base = new QSqlQuery(dbd);

    // ########################################################################
    // Pour afficher le menu dans la barre des taches !
    // ########################################################################

        sticon = new QSystemTrayIcon(QIcon("icone.png"), this);

        stmenu = new QMenu(this);
        menustatut = stmenu->addMenu("Mon statut");
        menuaide = stmenu->addAction("Aide");
        menuquitter = stmenu->addAction("Quitter LoQuiî");

        connectee = menustatut->addAction("Connecté");
        absent = menustatut->addAction("Absent");
        occupe = menustatut->addAction("Occupé");
        invisible = menustatut->addAction("Invisible");


        stmenu->addMenu(menustatut);
        stmenu->addAction(menuaide);
        stmenu->addAction(menuquitter);
        sticon->setContextMenu(stmenu);
        sticon->show();

        connect(menuquitter, SIGNAL(triggered()), this, SLOT(Deconnection()));
        connect(menuaide, SIGNAL(triggered()), this, SLOT(on_actionAide_triggered()));
        connect(connectee, SIGNAL(triggered()), this, SLOT(on_StatutConnecte_clicked()));
        connect(absent, SIGNAL(triggered()), this, SLOT(on_StatutAbsent_clicked()));
        connect(occupe, SIGNAL(triggered()), this, SLOT(on_StatutOccupe_clicked()));
        connect(invisible, SIGNAL(triggered()), this, SLOT(on_StatutInvisible_clicked()));

        QSettings options("pox", "loquii");

        if(options.value("welcome", "") != "")
        {
            QString titre = "LoQuiî";
            QString message = QString(options.value("welcome").toString());
            sticon->QSystemTrayIcon::showMessage(titre, message);

            options.setValue("welcome", "");
        }

        ui->TxtVersionProgram->setText("V. " + options.value("versionprogramme").toString());

        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

        FenMacumba::on_BoutonActuServ_clicked();

       FenMacumba::ActuInfoUser();
       FenMacumba::ActuListeDemandeContact();
       FenMacumba::ActuListeContact();
       FenMacumba::ActuContactConnection();
}

void FenMacumba::ActuListeContact()
{
    if(dbd.open())
    {
        query_base->prepare("SELECT friendwith FROM contact WHERE pseudmd = ?");
        query_base->addBindValue(ui->Pseudo->text());

        if (query_base->exec())
        {
                while(query_base->next())
                           {

                               for(int x=0; x < 1; ++x)
                               {
                                       ui->ListeContact->addItem(query_base->record().value("friendwith").toString());

                                       liste_ami += QString("pseudo='") + query_base->record().value("friendwith").toString() + QString("' or ");
                                       ListFriend += QString(" -+--") + query_base->record().value("friendwith").toString() + QString("--+-");
                                       ListFriendDiscution += QString(" -+-") + query_base->record().value("friendwith").toString() + QString("-+-");
                               }
                           }
        }

        FenMacumba::load_notification();

    dbd.close();
    }
}

//#######################################################################################################
//#######################################################################################################
// Button windows
//#######################################################################################################
//#######################################################################################################

void FenMacumba::on_actionProfile_triggered()
{
    ui->FenHome->hide();
    ui->FenNotifications->hide();
    ui->FenContactClick->hide();
    ui->FenContact->hide();
    ui->FenProfil->show();
}

void FenMacumba::on_ButtonHome_clicked()
{
    ui->FenProfil->hide();
    ui->FenNotifications->hide();
    ui->FenContactClick->hide();
    ui->FenContact->hide();
    ui->FenHome->show();

    FenMacumba::load_notification();
}

void FenMacumba::on_ButtonNotifications_clicked()
{
    ui->FenContactClick->hide();
    ui->FenHome->hide();
    ui->FenProfil->hide();
    ui->FenNotifications->show();
}

//#######################################################################################################
//#######################################################################################################
// FenProfil
//#######################################################################################################
//#######################################################################################################

void FenMacumba::ActuInfoUser()
{
    if(dbd.open())
    {
        QSettings options("pox", "loquii");

        query_base->prepare("SELECT * FROM user WHERE pseudo = ?");
        query_base->addBindValue(QString(options.value("user").toString()));

        if(query_base->exec())
        {
            while(query_base->next())
                       {

                           for(int x=0; x < 1; ++x)
                           {
                               ui->Pseudo->setText(QString(query_base->record().value("pseudo").toString()));
                               ui->ButtonHome->setText(ui->Pseudo->text());
                               ui->Email->setText(QString(query_base->record().value("email").toString()));
                               ui->Age->setValue(query_base->record().value("age").toInt());
                               ui->MessagePerso->setText(QString(query_base->record().value("msg_perso").toString()));

                               if(query_base->record().value("email_p").toInt() == 0)
                               {
                                   ui->CheckEmailP->setChecked(false);
                               }
                               else
                               {
                                   ui->CheckEmailP->setChecked(true);
                               }

                               if(query_base->record().value("age_p").toInt() == 0)
                               {
                                   ui->CheckAgeP->setChecked(false);
                               }
                               else
                               {
                                   ui->CheckAgeP->setChecked(true);
                               }

                               QString filename = "http://pox.alwaysdata.net/program/img_profile/" + query_base->record().value("img_name").toString();
                               ui->ImgProfil->setUrl(filename);

                               QString recup_pay = query_base->record().value("pays").toString();

                               if (recup_pay == "France")
                               {
                                   ui->Pays->setCurrentIndex(0);

                               }
                               else if (recup_pay == "Belgique")
                               {
                                   ui->Pays->setCurrentIndex(1);

                               }
                               else if (recup_pay == "Suisse")
                               {
                                   ui->Pays->setCurrentIndex(2);

                               }
                               else if (recup_pay == "Angleterre")
                               {
                                   ui->Pays->setCurrentIndex(3);

                               }
                               else if(recup_pay == "Portugal")
                               {
                                   ui->Pays->setCurrentIndex(4);

                               }
                               else if(recup_pay == "Irlande")
                               {
                                   ui->Pays->setCurrentIndex(5);

                               }
                               else if (recup_pay == "Pays-Bas")
                               {
                                   ui->Pays->setCurrentIndex(6);

                               }
                               else if (recup_pay == "Pologne")
                               {
                                   ui->Pays->setCurrentIndex(7);

                               }
                               else if (recup_pay == "Italie")
                               {
                                   ui->Pays->setCurrentIndex(8);

                               }
                               else if (recup_pay == "Russie")
                               {
                                   ui->Pays->setCurrentIndex(9);

                               }
                               else if (recup_pay == "Etats-Unis")
                               {
                                   ui->Pays->setCurrentIndex(10);

                               }
                               else if (recup_pay == "Mexique")
                               {
                                   ui->Pays->setCurrentIndex(11);

                               }
                               else if (recup_pay == "Alaska")
                               {
                                   ui->Pays->setCurrentIndex(12);

                               }
                               else if (recup_pay == "Brezil")
                               {
                                   ui->Pays->setCurrentIndex(13);

                               }
                               else if (recup_pay == "Argentine")
                               {
                                   ui->Pays->setCurrentIndex(14);

                               }
                               else if (recup_pay == "Espagne")
                               {
                                   ui->Pays->setCurrentIndex(15);

                               }
                               else if (recup_pay == "Luxembourg")
                               {
                                   ui->Pays->setCurrentIndex(16);

                               }
                               else if (recup_pay == "Chine")
                               {
                                   ui->Pays->setCurrentIndex(17);

                               }
                               else if (recup_pay == "Australie")
                               {
                                   ui->Pays->setCurrentIndex(18);

                               }
                               else if (recup_pay == "Nouvelle-Zélande")
                               {
                                   ui->Pays->setCurrentIndex(19);

                               }
                               else if (recup_pay == "Maroc")
                               {
                                   ui->Pays->setCurrentIndex(20);

                               }
                               else if (recup_pay == "Tunisie")
                               {
                                   ui->Pays->setCurrentIndex(21);

                               }
                               else if (recup_pay == "Algérie")
                               {
                                   ui->Pays->setCurrentIndex(22);

                               }
                               else if (recup_pay == "Norvège")
                               {
                                   ui->Pays->setCurrentIndex(23);

                               }
                               else if (recup_pay == "Islande")
                               {
                                   ui->Pays->setCurrentIndex(24);

                               }
                               else if (recup_pay == "Madagascar")
                               {
                                   ui->Pays->setCurrentIndex(25);

                               }
                               else if (recup_pay == "Pays-inconnu")
                               {
                                   ui->Pays->setCurrentIndex(26);

                               }
                               else
                               {

                               }
                           }
                       }
        }

    ui->StatutConnecte->animateClick();

    dbd.close();
    }
}

void FenMacumba::on_StatutConnecte_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("UPDATE user SET statut='connecte' WHERE pseudo = ? ");
        query_base->addBindValue(ui->Pseudo->text());

        query_base->exec();

    dbd.close();
    }

    ui->StatutConnecte->setChecked(true);
}

void FenMacumba::on_StatutOccupe_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("UPDATE user SET statut='occupe' WHERE pseudo = ? ");
        query_base->addBindValue(ui->Pseudo->text());

        query_base->exec();

    dbd.close();
    }

    ui->StatutOccupe->setChecked(true);
}

void FenMacumba::on_StatutAbsent_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("UPDATE user SET statut='absent' WHERE pseudo = ? ");
        query_base->addBindValue(ui->Pseudo->text());

        query_base->exec();

    dbd.close();
    }

    ui->StatutAbsent->setChecked(true);
}

void FenMacumba::on_StatutInvisible_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("UPDATE user SET statut='deconnecte' WHERE pseudo = ? ");
        query_base->addBindValue(ui->Pseudo->text());

        query_base->exec();

    dbd.close();
    }

    ui->StatutInvisible->setChecked(true);
}

void FenMacumba::Deconnection()
{
    FenMacumba::SaveConversation();

    int question = QMessageBox::question(this,"Question",tr("Etes-vous sûr de vouloir vous deconnecter ?"), QMessageBox::Yes | QMessageBox::No);

    if(question == QMessageBox::Yes)
    {
        if(dbd.open())
        {
            query_base->prepare("UPDATE loquii SET co_start = '0' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            if(query_base->exec())
            {

            }

            query_base->prepare("UPDATE user SET statut='deconnecte' WHERE pseudo = ? ");
            query_base->addBindValue(ui->Pseudo->text());

            if(query_base->exec())
            {
                FenMacumba::close();
            }

        dbd.close();
        }
    }
}

void FenMacumba::on_ButtonSaveMsgPerso_clicked()
 {
    if (dbd.open())
    {
        query_base->prepare("UPDATE user SET msg_perso = :msg_perso WHERE pseudo = :pseudo ");
        query_base->bindValue(":msg_perso", ui->MessagePerso->toPlainText());
        query_base->bindValue(":pseudo", ui->Pseudo->text());

        if(query_base->exec())
        {

        }

        QDateTime timeensec;

        query_base->prepare("INSERT into event (pseudo,type,message,date) values (:pseudo,'changemsgperso',:message,:date)");
        query_base->bindValue(":pseudo", ui->Pseudo->text());
        query_base->bindValue(":message", ui->MessagePerso->toPlainText());
        query_base->bindValue(":date", QString::number(timeensec.currentDateTime().toTime_t()));

        if(query_base->exec())
        {

        }

    dbd.close();
    }
 }

void FenMacumba::on_ButtonSaveModifProfil_clicked()
{
    if(dbd.open())
    {
        if(ui->Password->text() != ui->ConfirmPassword->text())
        {
            QMessageBox::critical(this,tr("Erreur"),tr("Les mots de passes que vous avez entré ne sont pas identiques"));
        }
        else
        {
            if (ui->Password->text() == "" || ui->ConfirmPassword->text() == "")
            {
                query_base->prepare("UPDATE user SET email = :email, age = :age, country = :country WHERE pseudo = :pseudo");
                query_base->bindValue(":email", ui->Email->text());
                query_base->bindValue(":age", ui->Age->value());
                query_base->bindValue(":country", ui->Pays->currentText());
                query_base->bindValue(":pseudo", ui->Pseudo->text());

            }
            else
            {
                QString Password = ui->Password->text();
                QByteArray ba = QCryptographicHash::hash(Password.toUtf8(), QCryptographicHash::Sha1);
                QString PasswordHach = ba.toHex();

                query_base->prepare("UPDATE user SET password = :password, email = :email, age = :age, country = :country WHERE pseudo = :pseudo");
                query_base->bindValue(":password", PasswordHach);
                query_base->bindValue(":email", ui->Email->text());
                query_base->bindValue(":age", ui->Age->value());
                query_base->bindValue(":country", ui->Pays->currentText());
                query_base->bindValue(":pseudo", ui->Pseudo->text());
            }

            if (query_base->exec())
            {
                QMessageBox::information(this,"Information",tr("Les modifications que vous venez de faire ont été prises en compte"));
            }
        }

    dbd.close();
    }
}

void FenMacumba::on_CheckEmailP_clicked()
{
    if(dbd.open())
    {
        if(ui->CheckEmailP->isChecked() == true)
        {
            query_base->prepare("UPDATE user SET email_p = '1' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            query_base->exec();
        }
        else
        {
            query_base->prepare("UPDATE user SET email_p = '0' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            query_base->exec();
         }

    dbd.close();
    }
}

void FenMacumba::on_CheckAgeP_clicked()
{
    if(dbd.open())
    {
        if(ui->CheckAgeP->isChecked() == true)
        {
            query_base->prepare("UPDATE user SET age_p = '1' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            query_base->exec();
        }
        else
        {
            query_base->prepare("UPDATE user SET age_p = '0' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            query_base->exec();
         }

    dbd.close();
    }
}

// ########################################################################
// Ajouter une image de profil !
// ########################################################################

void FenMacumba::on_ButtonChangeImg_clicked()
{
   ui->ImgProfil->hide();
   ui->ButtonChangeImg->hide();
   ui->FenUploadImgContact->show();

   ui->ProgressUploadImg->hide();
   ui->TxtUploadImg->hide();
   ui->ButtonUploadImg->hide();
}

void FenMacumba::on_ButtonNavigateImg_clicked()
{

    QString link = QFileDialog::getOpenFileName(this,tr("Ajouter l'image"), QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    ui->TxtLink->setText(link);

    ui->ButtonNavigateImg->setEnabled(false);
    ui->ButtonUploadImg->show();

}

void FenMacumba::on_ButtonUploadImg_clicked()
{
    ui->ProgressUploadImg->show();
    ui->TxtUploadImg->show();

    QFtp *ft = new QFtp(this);
    ft->connectToHost("ftp.alwaysdata.com", 21);
    ft->login("loquii_site","topsecret");
    ft->cd("/www/program/img_profile");

    QString chemin = ui->TxtLink->text();
    QFile* file = new QFile(chemin);
    file->open(QIODevice::ReadOnly);
    QFileInfo fi(chemin);
    fi.fileName();
    QString filename;
    QString dot;
    dot = ".";
    filename = ui->Pseudo->text() + dot + fi.suffix();
    ft->put(file,filename , QFtp::Binary);
    connect(ft, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(ftpDataTransferProgress(qint64, qint64)));
    ft->close();

}

void FenMacumba::ftpDataTransferProgress(qint64 done, qint64 total)
{
    if(!ui->ProgressUploadImg)
    {

        return;
    }

    if (0 != total)
    {
        float downloadPercent;
        downloadPercent = done*100/total;
        ui->ProgressUploadImg->setValue(downloadPercent);
        if (downloadPercent == 100)
        {
            QString chemin = ui->TxtLink->text();
            QFileInfo fi(chemin);
            QString filename;
            QString dot;
            dot = ".";
            filename = ui->Pseudo->text() + dot + fi.suffix();

            if (dbd.open())
            {
                query_base->prepare("UPADATE user SET img_name = :img_name WHERE pseudo = :pseudo");
                query_base->bindValue(":img_name", filename);
                query_base->bindValue(":pseudo", ui->Pseudo->text());

                if (query_base->exec())
                {
                    ui->ProgressUploadImg->reset();
                    ui->ProgressUploadImg->hide();
                    ui->TxtUploadImg->hide();
                    ui->TxtLink->clear();

                    FenMacumba::ActuInfoUser();

                    QMessageBox::information(this,"Information",tr("Votre image a bien été envoyée"));
                }

            dbd.close();
            }

            ui->FenUploadImgContact->hide();
            ui->ImgProfil->show();
            ui->ButtonChangeImg->show();
        }
    }
}

void FenMacumba::on_ButtonDeleteCompte_clicked()
{
    if(dbd.open())
    {
        int confir_del = QMessageBox::question(this, "Information",tr("Etes-vous sur de vouloir vous desinscrire de LoQuii ?"), QMessageBox::Yes | QMessageBox::No);
        int confir_del1 = QMessageBox::question(this, "Information",tr("Attention : cette action est irreversible ! Etes vous sur de votre choix ?"), QMessageBox::Yes | QMessageBox::No);

        if (confir_del == QMessageBox::Yes)
        {
            if (confir_del1 == QMessageBox::Yes)
            {
                query_base->prepare("DELETE FROM user WHERE pseudo = ?");
                query_base->addBindValue(ui->Pseudo->text());

                if(query_base->exec())
                {
                    query_base->prepare("DELETE FROM contact WHERE friendwith = ? or pseudmd = ?");
                    query_base->addBindValue(ui->Pseudo->text());
                    query_base->addBindValue(ui->Pseudo->text());

                    if(query_base->exec())
                    {
                        QMainWindow::close();
                    }
                }
             }
        }
        else if (confir_del == QMessageBox::No)
        {

        }

    dbd.close();
    }
}

//#######################################################################################################
//#######################################################################################################
// FenHome
//#######################################################################################################
//#######################################################################################################

void FenMacumba::load_notification()
{
    ui->listenotweb->clear();

    if(dbd.open())
    {
        QDateTime datefromsec, CurentTimes;

        uint sec;
        uint sec2s = 3600*24*2*7;
        uint sec3j = 3600*24*3;
        uint sec2m = 3600*24*31*2;
        uint secnot2s;
        uint secnot3j;
        uint secnot2m;

        bool valid;

        QString html_text;

        QSettings options("pox", "loquii");

        QString requette_actunot;
        requette_actunot = QString("SELECT * from event WHERE ") + ListFriendEvent + QString(" pseudo='") + ui->Pseudo->text()+ QString("'");

        if (query_base->exec(requette_actunot))
        {
            while(query_base->next())
                       {
                           for(int x=0; x < 1; ++x)
                           {




                                   sec = query_base->record().value("date").toUInt();

                                   secnot2s = sec + sec2s;
                                   secnot3j = sec + sec3j;
                                   secnot2m = sec + sec2m;


                                   if (options.value("supprimerlesnotifications", "2s") == "2s")
                                   {
                                       if (secnot2s < CurentTimes.currentDateTime().toTime_t())
                                       {
                                           valid = true;
                                       }
                                       else
                                       {
                                            valid = false;
                                       }

                                   }
                                   else if (options.value("supprimerlesnotifications", "3j") == "3j")
                                   {
                                       if (secnot3j > CurentTimes.currentDateTime().toTime_t())
                                       {
                                           valid = false;
                                       }
                                       else
                                       {
                                           valid = true;
                                       }
                                   }
                                   else if (options.value("supprimerlesnotifications", "2m") == "2m")
                                   {
                                       if (secnot2m < CurentTimes.currentDateTime().toTime_t())
                                       {
                                           valid = true;
                                       }
                                       else
                                       {
                                           valid = false;
                                       }
                                   }


                                   if (valid !=  true)
                                   {
                                                                      if (query_base->record().value("type").toString() == "addcontact")
                                                                      {
                                                                          html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> est devenu ami avec <em>") + query_base->record().value("message").toString() + QString("</em></span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                          ui->listenotweb->append(html_text);
                                                                      }
                                                                      else if(query_base->record().value("type").toString() == "sexprime")
                                                                      {
                                                                          html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> : <input type=button value=cliquez ici><em>") + query_base->record().value("message").toString() + QString("</em></span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                          ui->listenotweb->append(html_text);
                                                                      }
                                                                      else if(query_base->record().value("type").toString() == "SupprContact")
                                                                      {
                                                                          html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> vient de supprimer <input type=button value=cliquez ici><em>") + query_base->record().value("message").toString() + QString("</em> comme contact</span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                          ui->listenotweb->append(html_text);
                                                                      }
                                                                      else if (query_base->record().value("type").toString() == "changemsgperso")
                                                                      {
                                                                          html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> a changé son message personnel : <em>") + query_base->record().value("message").toString() + QString("</em></span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                          ui->listenotweb->append(html_text);
                                                                      }
                                                                      else if (query_base->record().value("type").toString() == "1")
                                                                      {
                                                                           html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> à une notification aujourd'hui : <em>") + query_base->record().value("message").toString() + QString("</em></span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                           ui->listenotweb->append(html_text);
                                                                      }
                                                                      else if(query_base->record().value("type").toString() == "0")
                                                                      {
                                                                          QString clientquienv = query_base->record().value("client_env").toString();

                                                                          if(clientquienv == ui->Pseudo->text())
                                                                          {
                                                                            html_text = QString("<br/><div style=\"width : 250px;font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"><strong> > ") + query_base->record().value("pseudo").toString() + QString("</strong> a changé son message personnel : <em>") + query_base->record().value("message").toString() + QString("</em></span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                            ui->listenotweb->append(html_text);
                                                                          }
                                                                          else
                                                                          {

                                                                          }
                                                                      }
                                                                      else
                                                                      {
                                                                          if(options.value("afficherlesnotificationspourlesmessages", true).toBool() == true)
                                                                          {
                                                                              QString motif_id = query_base->record().value("motif").toString();
                                                                              bool contenir_id = conv_id.contains(motif_id, Qt::CaseInsensitive);

                                                                              if(contenir_id)
                                                                              {
                                                                                  if(query_base->record().value("motif").toInt() > 1)
                                                                                  {
                                                                                        html_text = QString("<br/><div style=\"width : 250px;color :") + options.value("couleurnot").toString() + QString("; font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"> Vous avez <em>") + query_base->record().value("message").toString() + QString("</em> nouveaux messages à la conversation ") + query_base->record().value("type").toString() + QString("</span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                                        ui->listenotweb->append(html_text);
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                      html_text = QString("<br/><div style=\"width : 250px;color :") + options.value("couleurnot").toString() + QString("; font-size: 0.8em; position:relative;\"><fieldset><span align=\"left\"> Vous avez <em>") + query_base->record().value("message").toString() + QString("</em> nouveau message à la conversation ") + query_base->record().value("type").toString() + QString("</span><center></center><span style='margin-left: 200px;'>") + datefromsec.fromTime_t(sec).toString() + QString("</span></fieldset></div>");
                                                                                      ui->listenotweb->append(html_text);
                                                                                  }
                                                                              }
                                                                          }
                                                                          else
                                                                          {

                                                                          }
                                                                      }

                                   }


                           }
                       }
        }

    dbd.close();
    }
}

void FenMacumba::on_ButtonSexprime_clicked()
{
    if(dbd.open())
    {
        QDateTime timeensec;

        query_base->prepare("INSERT into event (pseudo,type,message,date) values (:pseudo,'sexprime',:message,:date)");
        query_base->bindValue(":pseudo", ui->Pseudo->text());
        query_base->bindValue(":message", ui->ChpSexprime->text());
        query_base->bindValue(":date", QString::number(timeensec.currentDateTime().toTime_t()));

        if(query_base->exec())
        {
            ui->ChpSexprime->clear();
            ui->ChpSexprime->setFocus();
        }

    dbd.close();
    }

    FenMacumba::load_notification();
}

void FenMacumba::on_ButtonNewsLoQuii_clicked()
{
    QProcess::execute("cmd /c start http://loquii.alwaysdata.net/Pox/desktop/news.php");
}

void FenMacumba::ActuListeDemandeContact()
{
    if(dbd.open())
    {
        query_base->prepare("SELECT * FROM event WHERE titre = ? AND type = 'invfriend'");
        query_base->addBindValue(ui->Pseudo->text());

        if (query_base->exec())
        {
            while(query_base->next())
            {
                for(int x=0; x < 1; ++x)
                {
                    ui->ListeInvFriend->addItem(query_base->record().value("pseudo").toString());
                }
        }
    }

    dbd.close();
    }
}

void FenMacumba::on_ListeInvFriend_clicked()
{
    int deconnexion = QMessageBox::question(this, "Information",tr("Cette personne veut vous ajouter dans sa liste de contact. Voulez-vous être amis avec elle ?"), QMessageBox::Yes | QMessageBox::No);

        if (deconnexion == QMessageBox::Yes)
        {
            if (dbd.open())
            {
                query_base->prepare("INSERT into contact (friendwith,pseudmd) values (:friendwith,:pseudmd)");
                query_base->bindValue(":friendwith", ui->Pseudo->text());
                query_base->bindValue(":pseudmd", ui->ListeInvFriend->currentItem()->text());

                query_base->exec();

                query_base->prepare("INSERT into contact (friendwith,pseudmd) values (:friendwith,:pseudmd)");
                query_base->bindValue(":friendwith", ui->ListeInvFriend->currentItem()->text());
                query_base->bindValue(":pseudmd", ui->Pseudo->text());

                query_base->exec();

                QDateTime timeensec;

                query_base->prepare("INSERT into event (pseudo,type,message,date) values (:pseudo,'newfriend',:message,:date)");
                query_base->bindValue(":pseudo", ui->Pseudo->text());
                query_base->bindValue(":message", ui->ListeInvFriend->currentItem()->text());
                query_base->bindValue(":date", QString::number(timeensec.currentDateTime().toTime_t()));

                query_base->exec();

                query_base->prepare("DELETE FROM event WHERE pseudo = :pseudo AND titre = :titre AND type = 'invfriend'");
                query_base->bindValue(":pseudo", ui->ListeInvFriend->currentItem()->text());
                query_base->bindValue(":titre", ui->Pseudo->text());

                query_base->exec();

                ui->ListeContact->addItem(ui->ListeInvFriend->currentItem());
                delete ui->ListeInvFriend->currentItem();

            dbd.close();
            }
        }
        else if (deconnexion == QMessageBox::No)
        {
           if (dbd.open())
           {
               query_base->prepare("DELETE FROM event WHERE pseudo = :pseudo AND titre = :titre AND type = 'invfriend'");
               query_base->bindValue(":pseudo", ui->ListeInvFriend->currentItem()->text());
               query_base->bindValue(":titre", ui->Pseudo->text());

               query_base->exec();

               delete ui->ListeInvFriend->currentItem();

           dbd.close();
           }
        }
}

//#######################################################################################################
//#######################################################################################################
// FenContactClick
//#######################################################################################################
//#######################################################################################################

void FenMacumba::on_ListeContact_clicked()
{
    FenMacumba::SaveConversation();

    ui->FenHome->hide();
    ui->FenNotifications->hide();
    ui->FenProfil->hide();
    ui->FenContactClick->show();

    if (dbd.open())
    {
        query_base->prepare("SELECT * FROM user WHERE pseudo = ?");
        query_base->addBindValue(ui->ListeContact->currentItem()->text());

       if (query_base->exec())
       {
           while(query_base->next())
                      {

                          for(int x=0; x < 1; ++x)
                          {
                              QString filename = "http://loquii.alwaysdata.net/ProgInfo/ImgContact/" + query_base->record().value("img_name").toString();
                              ui->ImgCont->setUrl(filename);

                              QString etat = query_base->record().value("statut").toString();


                              if (etat == "connecte")
                              {
                                  ui->StatutCont->load(QUrl("http://loquii.alwaysdata.net/Pox/desktop/img/statut-co.png"));
                              }
                              else if (etat == "occupe")
                              {
                                  ui->StatutCont->load(QUrl("http://loquii.alwaysdata.net/Pox/desktop/img/statut-occ.png"));
                              }
                              else if (etat == "absent")
                              {
                                  ui->StatutCont->load(QUrl("http://loquii.alwaysdata.net/Pox/desktop/img/statut-abs.png"));
                              }
                              else if (etat == "deconnecte" || etat == "invisible")
                              {
                                  ui->StatutCont->load(QUrl("http://loquii.alwaysdata.net/Pox/desktop/img/statut-deco.png"));
                              }

                              ui->PseudoCont->setText(QString(query_base->record().value("pseudo").toString()));
                              ui->PaysCont->setText(QString(query_base->record().value("pays").toString()));
                              ui->MsgPersoCont->setText(QString(query_base->record().value("msg_perso").toString()));

                              if(query_base->record().value("email_p").toInt() == 0)
                              {
                                  ui->EmailCont->setText("<em>Ne souhaite pas la rendre visible</em>");
                              }
                              else
                              {
                                ui->EmailCont->setText(QString(query_base->record().value("email").toString()));
                              }

                              if(query_base->record().value("age_p").toInt() == 0)
                              {
                                  ui->AgeCont->setText("<em>Non-définit</em>");
                              }
                              else
                              {
                                  ui->AgeCont->setText(QString(query_base->record().value("age").toString()) + " ans");
                              }
                          }
                      }
       }

      query_base->prepare("SELECT * FROM discution WHERE client1 = :client1 AND client2 = :client2 or client1 = :client11 AND client2 = :client22");
      query_base->bindValue(":client1", ui->Pseudo->text());
      query_base->bindValue(":client2", ui->ListeContact->currentItem()->text());
      query_base->bindValue(":client11", ui->ListeContact->currentItem()->text());
      query_base->bindValue(":client22", ui->Pseudo->text());

     if (query_base->exec())
     {
         while(query_base->next())
                    {

                        for(int x=0; x < 1; ++x)
                        {
                            ui->DiscutionId->setText(QString(query_base->record().value("id").toString()));
                            ui->ListeMessages->setText(QString(query_base->record().value("message").toString()));
                        }
                    }
      }

    dbd.close();
    }

    UserCont = ui->ListeContact->currentItem()->text();
}

void FenMacumba::SaveConversation()
{
    if(dbd.open())
    {
        query_base->prepare("UPDATE discution SET message = :message WHERE client1 = :client1 AND client2 = :client2 or client1 = :client11 AND client2 = :client22");
        query_base->bindValue(":message", ui->ListeMessages->toPlainText());
        query_base->bindValue(":client1", ui->Pseudo->text());
        query_base->bindValue(":client2", UserCont);
        query_base->bindValue(":client11", UserCont);
        query_base->bindValue(":client22", ui->Pseudo->text());

        if(query_base->exec())
        {

        }

    dbd.close();
    }

    ui->ListeMessages->clear();
}

// ########################################################################
// Fonction pour envoyer un message !
// ########################################################################

void FenMacumba::on_ButtonSendMessage_clicked()
{
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);



        QDateTime DateTime;
        QString messageAEnvoyer, id_compliquer, pseudo_compliquer;
        id_compliquer = QString("-+-") + ui->DiscutionId->text() + QString("-+-");
        pseudo_compliquer = QString("-+-") + ui->Pseudo->text() + QString("-+-");
        messageAEnvoyer= id_compliquer + " " + pseudo_compliquer + " " + tr("<strong>")+ ui->ButtonHome->text() + tr("</strong> : ") + ui->message_ecrit->text() + tr("       <em>") + DateTime.currentDateTime().toString() + tr("</em>");
        RetourTexte = ui->message_ecrit->text();

        out << (quint16) 0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet);

        ui->message_ecrit->clear();
        ui->message_ecrit->setFocus();
}

void FenMacumba::on_smile1_clicked()
{
    QString smile1 = " xD ";
    ui->message_ecrit->insert(QString(smile1));
}

void FenMacumba::on_smile2_clicked()
{
    QString smile2 = " oO ";
    ui->message_ecrit->insert(QString(smile2));
}

void FenMacumba::on_smile3_clicked()
{
    QString smile3 = " ;) ";
    ui->message_ecrit->insert(QString(smile3));
}

void FenMacumba::on_smile4_clicked()
{
    QString smile4 = " =)- ";
    ui->message_ecrit->insert(QString(smile4));
}

void FenMacumba::on_smile5_clicked()
{
    QString smile5 = " =) ";
    ui->message_ecrit->insert(QString(smile5));
}

void FenMacumba::on_smile6_clicked()
{
    QString smile6 = " =o) ";
    ui->message_ecrit->insert(QString(smile6));
}

void FenMacumba::on_smile7_clicked()
{
    QString smile7 = " =( ";
    ui->message_ecrit->insert(QString(smile7));
}

void FenMacumba::on_ButtonBackText_clicked()
{
    ui->message_ecrit->setText(RetourTexte);
}

// ########################################################################
// Fonction pour actualiser le connexion au serveur !
// ########################################################################

void FenMacumba::on_BoutonActuServ_clicked()
{
    tailleMessage = 0;

    ui->ListeMessages->append(tr("<em>Connexion à notre serveur le plus proche ...</em>"));

    socket->abort();
    socket->connectToHost("localhost", 50885);
    //"kiwii.no-ip.org"
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Connexion au serveur et transfert de données avec les erreurs ///////////////////////////////////////////////////////////////


// ########################################################################
// Fonctions pour dire si on est connecté ou déconnecté au serveur !
// ########################################################################

void FenMacumba::connecte()
{
    ui->ListeMessages->append(tr("<em>Connexion réussie !</em>"));
}

void FenMacumba::deconnecte()
{
    ui->ListeMessages->append(tr("<em>Déconnecté du serveur.</em>"));
    ui->ListeMessages->append(tr("<strong>Cliquez sur le bouton 'Serveur'. Si le problème persiste, merci de bien vouloir attendre.</strong>"));
}

// ########################################################################
// Fonction pour la gestion des erreurs si problème avec le serveur !
// ########################################################################

void FenMacumba::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
        case QAbstractSocket::RemoteHostClosedError:
            ui->ListeMessages->append(tr("<em>ERREUR : La connexion a été coupée, cliquez sur le bouton 'Serveur'.</em>"));
            break;
        default:
            ui->ListeMessages->append(tr("<em>ERREUR : ") + socket->errorString() + tr(".</em>"));
    }
}

void FenMacumba::error(QNetworkReply::NetworkError)
{
    qWarning("error retrieving RSS feed");
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
}


// ########################################################################
// Fonction pour reçevoir les messages des autres utilisateurs !
// ########################################################################


void FenMacumba::donneesRecues()
{
        QDataStream in(socket);
        QString smile1,smile2,smile3,smile4,smile5,smile6,smile7;
        smile1 = " xD ";
        smile2 = " oO ";
        smile3 = " ;) ";
        smile4 = " =)- ";
        smile5 = " =) ";
        smile6 = " =o) ";
        smile7 = " =( ";

        if (tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                 return;

            in >> tailleMessage;
        }

        if (socket->bytesAvailable() < tailleMessage)
            return;


        QString messageRecu;
        in >> messageRecu;

        qDebug() << messageRecu;

        QString id_compliquer = QString("-+-") + ui->DiscutionId->text() + QString("-+-");

            if(messageRecu.contains(id_compliquer, Qt::CaseSensitive))
            {
                ui->ListeMessages->append(messageRecu);
            }
            else if(messageRecu.contains(ListFriendDiscution, Qt::CaseSensitive))
            {
                sticon->showMessage("Nouveau message",messageRecu);
            }

        if (ListFriend.contains(messageRecu, Qt::CaseSensitive))
        {
            sticon->showMessage("Connection",messageRecu + " vient de se connecter");
        }

        if (messageRecu.contains(smile1, Qt::CaseSensitive))
        {
            messageRecu.replace(smile1," <img src=\"statut-abs.png\"> ", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile2, Qt::CaseSensitive))
        {
            messageRecu.replace(smile2,"code html2", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile3, Qt::CaseSensitive))
        {
            messageRecu.replace(smile3,"code html3", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile4, Qt::CaseSensitive))
        {
            messageRecu.replace(smile4,"code html4", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile5, Qt::CaseSensitive))
        {
            messageRecu.replace(smile5,"code html5", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile6, Qt::CaseSensitive))
        {
            messageRecu.replace(smile6,"code html6", Qt::CaseSensitive);
        }
        if (messageRecu.contains(smile7, Qt::CaseSensitive))
        {
            messageRecu.replace(smile7,"code html7", Qt::CaseSensitive);
        }


        if(messageRecu.contains("AF", Qt::CaseSensitive))
        {
            FenMacumba::ActuListeDemandeContact();
        }

        tailleMessage = 0;
}

void FenMacumba::on_ButtonDeleteContact_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("DELETE FROM contact WHERE friendwith = :friendwith AND pseudmd = :pseudmd");
        query_base->bindValue(":friendwith", ui->Pseudo->text());
        query_base->bindValue(":pseudmd", ui->ListeContact->currentItem()->text());

        query_base->exec();

        query_base->prepare("DELETE FROM contact WHERE friendwith = :friendwith AND pseudmd = :pseudmd");
        query_base->bindValue(":friendwith", ui->ListeContact->currentItem()->text());
        query_base->bindValue(":pseudmd", ui->Pseudo->text());

        query_base->exec();

        ui->FenContactClick->hide();
        ui->FenHome->show();

        delete ui->ListeContact->currentItem();

    dbd.close();
    }
}

//#######################################################################################################
//#######################################################################################################
// FenContact
//#######################################################################################################
//#######################################################################################################

void FenMacumba::on_ButtonSearchContact_clicked()
{
    if(dbd.open())
    {
        QSettings options("pox", "loquii");

        QString search = "%" + ui->ChpPseudoSearch->text() + "%";

        query_base->prepare("SELECT pseudo FROM user WHERE pseudo LIKE ?");
        query_base->addBindValue(search);

        if(query_base->exec())
        {
            while(query_base->next())
                       {

                           for(int x=0; x < 1; ++x)
                           {
                               ui->ListeContactPox->addItem(QString(query_base->record().value("pseudo").toString()));
                           }
                       }
        }

    dbd.close();
    }
}

void FenMacumba::on_ListeContactPox_clicked()
{
    if (dbd.open())
    {
        query_base->prepare("SELECT * FROM user WHERE pseudo = ?");
        query_base->addBindValue(ui->ListeContactPox->currentItem()->text());

       if (query_base->exec())
       {
           while(query_base->next())
                      {

                          for(int x=0; x < 1; ++x)
                          {
                              QString filename = "http://loquii.alwaysdata.net/ProgInfo/ImgContact/" + query_base->record().value("img_name").toString();
                              ui->ImgContPox->setUrl(filename);

                              ui->PseudoContPox->setText(QString(query_base->record().value("pseudo").toString()));
                              ui->PaysContPox->setText(QString(query_base->record().value("pays").toString()));
                          }
                      }
        }

    dbd.close();
    }

    ui->GroupBoxInfoContactPox->show();
}

void FenMacumba::on_ButtonAddCont_clicked()
{
   if (dbd.open())
   {
       if (ui->PseudoContPox->text() == ui->Pseudo->text())
       {

           QMessageBox::critical(this,"Erreur",tr("Vous ne pouvez pas être amis avec vous même"));
       }
       else
       {
           if(ListFriend.contains(ui->ListeContactPox->currentItem()->text(), Qt::CaseInsensitive))
           {
               QMessageBox::critical(this,"Erreur",tr("Vous ne pouvez pas ajouter deux fois le même contact"));
           }
           else
           {
               QDateTime timeensec;

               query_base->prepare("INSERT into event (pseudo,type,titre,date) values (:pseudo,'invfriend',:titre,:date)");
               query_base->bindValue(":pseudo", ui->Pseudo->text());
               query_base->bindValue(":titre", ui->PseudoContPox->text());
               query_base->bindValue(":date", QString::number(timeensec.currentDateTime().toTime_t()));

               if(query_base->exec())
               {
                   QMessageBox::information(this,"Information",tr("Nous venons de lui faire part de votre demande"));

                   FenMacumba::ActuListContactDemande();
               }
           }
       }

       dbd.close();
       }
}

void FenMacumba::ActuListContactDemande()
{
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        // On prépare le paquet à envoyer
        QString messageAEnvoyer = "AF";

        out << (quint16) 0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet); // On envoie le paquet
}

//#######################################################################################################
//#######################################################################################################
// MenuBar
//#######################################################################################################
//#######################################################################################################

void FenMacumba::on_actionOption_triggered()
{
    Fenoptions *d = new Fenoptions;
    d->show();
}

void FenMacumba::on_actionAide_triggered()
{
    QProcess::execute("cmd /c start http://loquii.alwaysdata.net/Pox/desktop/faq.php");
}

void FenMacumba::on_actionA_propos_de_LoQui_triggered()
{
    apropos *d = new apropos(this);
    d->show();
}

void FenMacumba::on_actionAjouter_un_contact_triggered()
{
    ui->FenHome->hide();
    ui->FenNotifications->hide();
    ui->FenContactClick->hide();
    ui->FenProfil->hide();
    ui->FenContact->show();
}

void FenMacumba::on_actionConnect_triggered()
{
    FenMacumba::on_StatutConnecte_clicked();
}

void FenMacumba::on_actionOccupe_triggered()
{
    FenMacumba::on_StatutOccupe_clicked();
}

void FenMacumba::on_actionAbsent_triggered()
{
    FenMacumba::on_StatutAbsent_clicked();
}

void FenMacumba::on_actionInvisible_triggered()
{
    FenMacumba::on_StatutInvisible_clicked();
}

void FenMacumba::on_actionDeconnection_triggered()
{
    FenMacumba::Deconnection();
}


//#######################################################################################################
//#######################################################################################################
// Fonction d'actualisation et de récupération de listes du programme !
//#######################################################################################################
//#######################################################################################################

// ########################################################################
// Fonction pour récuperer notre liste des contacts !
// ########################################################################



// ########################################################################
// Fonction pour envoyer un paquet lorsqu'on se connecte !
// ########################################################################

void FenMacumba::ActuContactConnection()
{
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        QString messageAEnvoyer = "-+--" + ui->Pseudo->text() + "--+-";

        out << (quint16) 0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet);
}

//#######################################################################################################
//#######################################################################################################
// Notifications !
//#######################################################################################################
//#######################################################################################################

// ########################################################################
// Fonction pour récuperer la liste des notifications !
// ########################################################################



void FenMacumba::on_BoutonAffAffNot_clicked()
{
    ui->fenaddnot->hide();
    ui->fenaffnot->show();
}

void FenMacumba::on_BoutonAffAddNot_clicked()
{
    ui->fenaffnot->hide();
    ui->fenaddnot->show();
}

// ########################################################################
// Fonction pour récuperer la liste de nos notifications du calendrier !
// ########################################################################

void FenMacumba::on_calendrier_clicked()
{
    ui->list_not->clear();
    ui->txt_titre->clear();
    ui->txt_note->clear();
    ui->txt_date->clear();

    ui->date_not->setText(QString(ui->calendrier->selectedDate().toString()));

    if(dbd.open())
    {
        QDateTime datetime_calendrier;
        datetime_calendrier.setDate(ui->calendrier->selectedDate());

        QString requette_calendrier = QString("SELECT * FROM calendrier_not WHERE date='") + QString::number(datetime_calendrier.toTime_t()) + QString("' AND personne_env='") + ui->Pseudo->text() + QString("'");

        if(query_base->exec(requette_calendrier))
        {
            while(query_base->next())
                       {
                           for(int x=0; x < 1; ++x)
                           {
                               ui->list_not->addItem(query_base->record().value("heure").toString());
                           }
                       }
        }

    dbd.close();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Critical, tr("Erreur"),tr("Connexion impossible à la base de données."),QMessageBox::Ok );
        msgBox.setStyleSheet("QMessageBox {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "}"
                             "QPushButton {"
                             "border: 2px solid #8f8f91;"
                             "border-radius: 6px;"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "width: 30px;"
                             "height: 27px;"
                             "}"

                             "QPushButton:pressed {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
                             "}");
        msgBox.exec();
    }
}

// #######################################################################################################
// Fonction qui affiche les informations de la notification séléctionnée !
// #######################################################################################################

void FenMacumba::on_list_not_clicked()
 {
    if (dbd.open())
    {
       QString requette_notcalendrier;
       requette_notcalendrier = QString("SELECT * FROM calendrier_not WHERE personne_env='") + ui->Pseudo->text() + QString("' AND heure='") + ui->list_not->currentItem()->text() + QString("'");

       if (query_base->exec(requette_notcalendrier))
       {
           while(query_base->next())
                      {

                          for(int x=0; x < 1; ++x)
                          {
                              ui->txt_titre->setText(QString(query_base->record().value("titre").toString()));
                              ui->txt_note->setText(QString(query_base->record().value("information").toString()));
                              ui->txt_heure->setText(QString(query_base->record().value("heure").toString()));
                              ui->txt_date->setText(QString(ui->calendrier->selectedDate().toString()));
                          }
                      }
        }

    dbd.close();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Critical, tr("Erreur"),tr("Connexion impossible à la base de données."),QMessageBox::Ok );
        msgBox.setStyleSheet("QMessageBox {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "}"
                             "QPushButton {"
                             "border: 2px solid #8f8f91;"
                             "border-radius: 6px;"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "width: 30px;"
                             "height: 27px;"
                             "}"

                             "QPushButton:pressed {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
                             "}");
        msgBox.exec();
    }
 }

// ########################################################################
// Fonction pour ajouter une notification au calendrier !
// ########################################################################

void FenMacumba::on_BoutonaddNot_clicked()
{
    if (dbd.open())
    {
        QDateTime datetime_not;
        datetime_not.setDate(ui->calendrier->selectedDate());

        QString requette_addnot;
        requette_addnot = QString("INSERT INTO calendrier_not VALUE ('','") + ui->titre_not->text() + QString("', '") + ui->note_not->toPlainText() + QString("', '") + ui->Pseudo->text() + QString("', '") + ui->liste_heure->currentText() + QString("', '") + QString::number(datetime_not.toTime_t()) + QString("')");

        if (query_base->exec(requette_addnot))
        {
            QMessageBox msgBox(QMessageBox::Information, tr("Information"),tr("Votre notification a bien été publiée"),QMessageBox::Ok );
            msgBox.setStyleSheet("QMessageBox {"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                                 "}"
                                 "QPushButton {"
                                 "border: 2px solid #8f8f91;"
                                 "border-radius: 6px;"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                                 "width: 30px;"
                                 "height: 27px;"
                                 "}"

                                 "QPushButton:pressed {"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
                                 "}");
            msgBox.exec();

            QString motif;

            if(ui->rendrepubnot->isChecked() == true)
            {
                motif = "1";
            }
            else
            {
                motif = "0";
            }

            QString info_not = ui->titre_not->text() + " pour le : " + ui->calendrier->selectedDate().toString();
            QDateTime timeensec;

            QString requette_actu_not;
            requette_actu_not = QString("INSERT INTO actu VALUE ('','") + ui->Pseudo->text() + QString("', '") + motif + QString("', '") + info_not + QString("', '") + QString::number(timeensec.currentDateTime().toTime_t()) + QString("')");

            if(query_base->exec(requette_actu_not))
            {
                ui->titre_not->clear();
                ui->note_not->clear();
                ui->liste_heure->setCurrentIndex(0);
                ui->rendrepubnot->setChecked(false);
            }
            else
            {

            }
        }
        else
        {
            QMessageBox msgBox(QMessageBox::Critical, tr("Erreur"),tr("Impossible d'éxecuter cette requete"),QMessageBox::Ok );
            msgBox.setStyleSheet("QMessageBox {"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                                 "}"
                                 "QPushButton {"
                                 "border: 2px solid #8f8f91;"
                                 "border-radius: 6px;"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                                 "width: 30px;"
                                 "height: 27px;"
                                 "}"

                                 "QPushButton:pressed {"
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
                                 "}");
            msgBox.exec();
        }

     dbd.close();
     }
     else
     {
        QMessageBox msgBox(QMessageBox::Critical, tr("Erreur"),tr("Connexion impossible à la base de données."),QMessageBox::Ok );
        msgBox.setStyleSheet("QMessageBox {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "}"
                             "QPushButton {"
                             "border: 2px solid #8f8f91;"
                             "border-radius: 6px;"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                             "width: 30px;"
                             "height: 27px;"
                             "}"

                             "QPushButton:pressed {"
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
                             "}");
        msgBox.exec();
     }
}

//#######################################################################################################
//#######################################################################################################
// Gestion du chat !
//#######################################################################################################
//#######################################################################################################

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion du chat pour les utilisateurs ///////////////////////////////////////////////////////////////



// ########################################################################
// Fonctions pour l'affichage des smiley !
// ########################################################################



//#######################################################################################################
//#######################################################################################################
// Event
//#######################################################################################################
//#######################################################################################################

void FenMacumba::contextMenuEvent(QContextMenuEvent *event, QPoint  *pos)
{
    QMenu *menu = new QMenu(this);
    QAction *docAction1 = menu->addAction(tr("Discuter"));
    QAction *docAction2 = menu->addAction(tr("Jouer"));
    QAction *docAction3 = menu->addAction(tr("Profil"));
    QAction *docAction4 = menu->addAction(tr("Supprimer des contacts"));
    //menu->move(ui->liste_contact->mapToGlobal(pos));
    menu->exec(ui->ListeContactPox->mapToGlobal(event->globalPos()));

    connect(docAction1, SIGNAL(triggered()), this, SLOT(on_BoutonDeco_clicked()));
    connect(docAction2, SIGNAL(triggered()), this, SLOT(on_actionAide_triggered()));
    connect(docAction3, SIGNAL(triggered()), this, SLOT(on_statut_connec_clicked()));
    connect(docAction4, SIGNAL(triggered()), this, SLOT(on_statut_abs_clicked()));
}

void FenMacumba::closeEvent(QCloseEvent *event)
{
    FenMacumba::SaveConversation();

    int question = QMessageBox::question(this,"Question",tr("Etes-vous sûr de vouloir vous deconnecter ?"), QMessageBox::Yes | QMessageBox::No);

    if(question == QMessageBox::Yes)
    {
        if(dbd.open())
        {
            query_base->prepare("UPDATE loquii SET co_start = '0' WHERE pseudo = ?");
            query_base->addBindValue(ui->Pseudo->text());

            if(query_base->exec())
            {

            }

            query_base->prepare("UPDATE user SET statut='deconnecte' WHERE pseudo = ? ");
            query_base->addBindValue(ui->Pseudo->text());

            if(query_base->exec())
            {
                event->accept();
            }

        dbd.close();
        }
    }
    else
    {
        event->ignore();
    }
}

FenMacumba::~FenMacumba()
{
    delete ui;
}
