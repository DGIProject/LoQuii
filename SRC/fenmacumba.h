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

#ifndef FENMACUMBA_H
#define FENMACUMBA_H

#include <QMainWindow>
#include <string>
#include <QFtp>
#include <QHttp>
#include <QFile>
#include <QPushButton>
#include <QTcpSocket>
#include <QtSql>
#include <QNetworkAccessManager>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QNetworkReply>
#include <QUrl>
#include <QSystemTrayIcon>


namespace Ui{
class FenMacumba;
}

class FenMacumba : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FenMacumba(QWidget *parent = 0);
    ~FenMacumba();

private slots:
    void ActuListeContact();

    //Button windows
    void on_ButtonHome_clicked();
    void on_ButtonNotifications_clicked();

    //FenProfil
    void ActuInfoUser();
    void on_StatutConnecte_clicked();
    void on_StatutOccupe_clicked();
    void on_StatutAbsent_clicked();
    void on_StatutInvisible_clicked();
    void Deconnection();
    void on_ButtonSaveMsgPerso_clicked();
    void on_ButtonSaveModifProfil_clicked();
    void on_CheckEmailP_clicked();
    void on_CheckAgeP_clicked();
    void on_ButtonChangeImg_clicked();
    void on_ButtonNavigateImg_clicked();
    void on_ButtonUploadImg_clicked();
    void ftpDataTransferProgress(qint64 done, qint64 total);
    void on_ButtonDeleteCompte_clicked();

    //FenHome
    void load_notification();
    void on_ButtonSexprime_clicked();
    void on_ButtonNewsLoQuii_clicked();
    void ActuListeDemandeContact();
    void on_ListeInvFriend_clicked();

    //FenContactClick
    void on_ListeContact_clicked();
    void SaveConversation();
    void on_ButtonSendMessage_clicked();
    void on_smile1_clicked();
    void on_smile2_clicked();
    void on_smile3_clicked();
    void on_smile4_clicked();
    void on_smile5_clicked();
    void on_smile6_clicked();
    void on_smile7_clicked();
    void on_ButtonBackText_clicked();
    void on_BoutonActuServ_clicked();
    void connecte();
    void deconnecte();
    void donneesRecues();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void error(QNetworkReply::NetworkError);
    void on_ButtonDeleteContact_clicked();

    //FenContact
    void on_ButtonSearchContact_clicked();
    void on_ListeContactPox_clicked();
    void on_ButtonAddCont_clicked();

    //MenuBar
    void on_actionProfile_triggered();
    void on_actionOption_triggered();
    void on_actionAide_triggered();
    void on_actionA_propos_de_LoQui_triggered();
    void on_actionAjouter_un_contact_triggered();
    void on_actionConnect_triggered();
    void on_actionOccupe_triggered();
    void on_actionAbsent_triggered();
    void on_actionInvisible_triggered();
    void on_actionDeconnection_triggered();




    void on_BoutonaddNot_clicked();
    void on_calendrier_clicked();


    void ActuContactConnection();
    void ActuListContactDemande();
    void on_list_not_clicked();
    void on_BoutonAffAffNot_clicked();
    void on_BoutonAffAddNot_clicked();

public:
    Ui::FenMacumba *ui;

public:
    //Windows
    QString UserCont;
    QString ListFriend;
    QString ListFriendDiscution;
    QString ListFriendEvent;




    QString path_img_string;
    QFile *page;
    QHttp *http;
    QTcpSocket *socket;
    QSqlDatabase dbd;
    QSqlQuery* query_base;
    quint16 tailleMessage;
    QString conv_id;
    int *discution_id;

    QXmlStreamReader xml;
    QString currentTag;
    QString linkString;
    QString titleString;
    QString Datestring;
    QString liste_ami;
    QString liste_ami_cont;
    QString chemin_utilisateur;
    QString fichier_pseudo;
    QString RetourTexte;
    QString client_dif;

    QNetworkAccessManager manager;
    QNetworkReply *currentReply;

    QSystemTrayIcon *sticon;
    QMenu *stmenu;
    QAction *menuquitter;
    QAction *menuaide;
    QMenu *menustatut;
    QAction *connectee;
    QAction *absent;
    QAction *occupe;
    QAction *invisible;

protected:
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event, QPoint  *pos);

};

#endif // FENMACUMBA_H
