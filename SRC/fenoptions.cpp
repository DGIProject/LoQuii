#include "fenoptions.h"
#include "ui_fenoptions.h"
#include "fenprincipal.h"
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

using namespace std;

Fenoptions::Fenoptions(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Fenoptions)
{
    ui->setupUi(this);
    ui->fenoptionchat->hide();
    ui->fenoptionaudio->hide();
    ui->fenoptionvideo->hide();
    ui->Fengeneral->show();

    setWindowTitle("LoQuiî - Options");
    setWindowIcon(QIcon("icone.png"));

    Fenoptions::ActuIni();
}

void Fenoptions::ActuIni()
{
QSettings options("config_loquii.ini", QSettings::IniFormat);

if(options.value("lancerloquiiaudemarragedewindows", true).toBool() == true)
{
    ui->lancerlqaudem->setChecked(true);
}
else
{
    ui->lancerlqaudem->setChecked(false);
}

if(options.value("memontrerabsentauboutde5minutes", true).toBool() == true)
{
    ui->absauboutde5min->setChecked(true);
}
else
{
    ui->absauboutde5min->setChecked(false);
}

if(options.value("meconnecteraudemarragedeloquii", true).toBool() == true)
{
    ui->codemlq->setChecked(true);
}
else
{
    ui->codemlq->setChecked(false);
}

if(options.value("modesilencieux", true).toBool() == true)
{
    ui->modesilencieux->setChecked(true);
}
else
{
    ui->modesilencieux->setChecked(false);
}

if(options.value("activerlesmiseajour", true).toBool() == true)
{
    ui->activmaj->setChecked(true);
}
else
{
    ui->activmaj->setChecked(false);
}

if (options.value("choixdelalangue", "fr") == "fr")
{
    ui->choix_langue->setCurrentIndex(0);

}
else if (options.value("choixdelalangue", "en") == "en")
{
    ui->choix_langue->setCurrentIndex(1);

}
else if (options.value("choixdelalangue", "es") == "es")
{
    ui->choix_langue->setCurrentIndex(2);
}
else
{

}

if(options.value("couleurnot", "black") == "black")
{
    ui->ChoixCouleurNot->setCurrentIndex(0);
}
else if(options.value("couleurnot", "blue") == "blue")
{
    ui->ChoixCouleurNot->setCurrentIndex(1);
}
else if(options.value("couleurnot", "red") == "red")
{
    ui->ChoixCouleurNot->setCurrentIndex(2);
}
else if(options.value("couleurnot", "green") == "green")
{
    ui->ChoixCouleurNot->setCurrentIndex(3);
}
else if(options.value("couleurnot", "yellow") == "yellow")
{
    ui->ChoixCouleurNot->setCurrentIndex(4);
}
else if(options.value("couleurnot", "orange") == "orange")
{
    ui->ChoixCouleurNot->setCurrentIndex(5);
}
else if(options.value("couleurnot", "violet") == "violet")
{
    ui->ChoixCouleurNot->setCurrentIndex(6);
}
else if(options.value("couleurnot", "pink") == "pink")
{
    ui->ChoixCouleurNot->setCurrentIndex(7);
}
else
{
    QMessageBox::critical(this,"Erreur",tr(" Erreur interne au programe, veuillez supprimer le fichier config-loquii.ini ou contacter une personne sur loquii.alwaysdata.net dans l'onglet assistance\n Merci de votre compréhension."));
}

if (options.value("supprimerlesnotifications", "2s") == "2s")
{
    ui->Boutonsupprnot2s->setChecked(true);
}
else if (options.value("supprimerlesnotifications", "3j") == "3j")
{
    ui->Boutonsupprnot3j->setChecked(true);
}
else if (options.value("supprimerlesnotifications", "2m") == "2m")
{
    ui->Boutonsupprnot2m->setChecked(true);
}
else
{
    QMessageBox::critical(this,"Erreur",tr(" Erreur interne au programe, veuillez supprimer le fichier config-loquii.ini ou contacter une personne sur loquii.alwaysdata.net dans l'onglet assistance\n Merci de votre compréhension."));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

options.beginGroup("chat");

if(options.value("afficherlesemoticones", true).toBool() == true)
{
    ui->affemoticones->setChecked(true);
}
else
{
    ui->affemoticones->setChecked(false);
}

if (options.value("supprimerlesconversations", "3j") == "3j")
{
    ui->bout3j->setChecked(true);
}
else if (options.value("supprimerlesconversations", "2s") == "2s")
{
    ui->bout2s->setChecked(true);
}
else if (options.value("supprimerlesconversations", "2m") == "2m")
{
    ui->bout2m->setChecked(true);
}
else
{
    QMessageBox::critical(this,"Erreur",tr(" Erreur interne au programe, veuillez supprimer le fichier config-loquii.ini ou contacter une personne sur loquii.alwaysdata.net dans l'onglet assistance\n Merci de votre compréhension."));
}

if(options.value("montrerquandjecris", true).toBool() == true)
{
    ui->montrerqdecrit->setChecked(true);
}
else
{
    ui->montrerqdecrit->setChecked(false);
}

if(options.value("afficherlesnotificationspourlesmessages", true).toBool() == true)
{
    ui->notmsg->setChecked(true);
}
else
{
    ui->notmsg->setChecked(false);
}

options.endGroup();

options.beginGroup("Audio");

//ui->ChoixVolumeAudio->setCursor(QCursor(options.value("volumeaudio").Cursor));

ui->QuaAudio->setValue(options.value("qualiteaudio").toInt());

if(options.value("enregistrementaudio").toBool() == true)
{
    ui->CheckEnrAudio->setChecked(true);
}
else
{
    ui->CheckEnrAudio->setChecked(false);
}

options.endGroup();

options.beginGroup("Video");

ui->QuaVideo->setValue(options.value("qualitevideo").toInt());

if(options.value("demarrerautovideo").toBool() == true)
{
    ui->CheckDemAutoVideo->setChecked(true);
}
else
{
    ui->CheckDemAutoVideo->setChecked(false);
}

options.endGroup();

}

void Fenoptions::on_Boutonaffgeneral_clicked()
{
    ui->fenoptionchat->hide();
    ui->fenoptionaudio->hide();
    ui->fenoptionvideo->hide();
    ui->Fengeneral->show();
}

void Fenoptions::on_Boutonaffoptionchat_clicked()
{
    ui->Fengeneral->hide();
    ui->fenoptionaudio->hide();
    ui->fenoptionvideo->hide();
    ui->fenoptionchat->show();
}

void Fenoptions::on_Boutonaffoptionvoc_clicked()
{
    ui->Fengeneral->hide();
    ui->fenoptionchat->hide();
    ui->fenoptionvideo->hide();
    ui->fenoptionaudio->show();
}

void Fenoptions::on_Boutonaffoptionvid_clicked()
{
    ui->Fengeneral->hide();
    ui->fenoptionchat->hide();
    ui->fenoptionaudio->hide();
    ui->fenoptionvideo->show();
}

// ########################################################################
// Configuration du chat !
// ########################################################################


void Fenoptions::on_bout2s_clicked()
{
    if(ui->bout2s->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("supprimerlesconversations", "2s");
        options.endGroup();
    }
    else
    {

    }
}

void Fenoptions::on_bout3j_clicked()
{
    if(ui->bout3j->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("supprimerlesconversations", "3j");
        options.endGroup();
    }
    else
    {

    }
}

void Fenoptions::on_bout2m_clicked()
{
    if(ui->bout2m->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("supprimerlesconversations", "2m");
        options.endGroup();
    }
    else
    {

    }
}

void Fenoptions::on_affemoticones_clicked()
{
    if(ui->affemoticones->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("chat");
        options.setValue("afficherlesemoticones", true);
        options.endGroup();
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("chat");
        options.setValue("afficherlesemoticones", false);
        options.endGroup();
    }
}

void Fenoptions::on_montrerqdecrit_clicked()
{
    if(ui->montrerqdecrit->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("montrerquandjecris", true);
        options.endGroup();
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("montrerquandjecris", false);
        options.endGroup();
    }
}

void Fenoptions::on_notmsg_clicked()
{
    if(ui->notmsg->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("afficherlesnotificationspourlesmessages", true);
        options.endGroup();
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.beginGroup("chat");
        options.setValue("afficherlesnotificationspourlesmessages", false);
        options.endGroup();
    }
}

// ########################################################################
// Configuration générale !
// ########################################################################

void Fenoptions::on_ChoixCouleurNot_currentIndexChanged()
{
    QString choixcouleurnot = ui->ChoixCouleurNot->currentText();

    if(choixcouleurnot == "Noir")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "black");
    }
    else if(choixcouleurnot == "Bleu")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "blue");
    }
    else if(choixcouleurnot == "Rouge")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "red");
    }
    else if(choixcouleurnot == "Vert")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "green");
    }
    else if(choixcouleurnot == "Jaune")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "yellow");
    }
    else if(choixcouleurnot == "Orange")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "orange");
    }
    else if(choixcouleurnot == "Violet")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "violet");
    }
    else if(choixcouleurnot == "Rose")
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("couleurnot", "pink");
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("choixdelalangue", ui->ChoixCouleurNot->currentText());
    }
}

void Fenoptions::on_lancerlqaudem_clicked()
{
    if(ui->lancerlqaudem->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("lancerloquiiaudemarragedewindows", true);
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("lancerloquiiaudemarragedewindows", false);
    }
}

void Fenoptions::on_codemlq_clicked()
{
    if(ui->codemlq->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("meconnecteraudemarragedeloquii", true);
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("meconnecteraudemarragedeloquii", false);
    }
}

void Fenoptions::on_absauboutde5min_clicked()
{
    if(ui->absauboutde5min->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("memontrerabsentauboutde5minutes", true);
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("memontrerabsentauboutde5minutes", false);
    }
}

void Fenoptions::on_modesilencieux_clicked()
{
    if(ui->modesilencieux->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("modesilencieux", true);
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("modesilencieux", false);
    }
}

void Fenoptions::on_activmaj_clicked()
{
    if(ui->activmaj->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("activerlesmisesajour", true);
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("activerlesmisesajour", false);
    }
}

void Fenoptions::on_choix_langue_currentIndexChanged()
{
    QString choixlangue = ui->choix_langue->currentText();

    QString fr = "Français";
    QString en = "Anglais";
    QString es = "Espagnol";

    QString fr_en = "French";
    QString en_en = "English";
    QString es_en = "Spannish";


    if(choixlangue == fr || choixlangue == fr_en)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("choixdelalangue", "fr");

        QMessageBox::information(this,"Information - Información"," Vous devez redemarrer le programme pour appliquer le changement de langue\nYou must restart the program to apply the language change\nDebe reiniciar el programa para aplicar el cambio de idioma");
    }
    else if(choixlangue == en || choixlangue == en_en)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("choixdelalangue", "en");

        QMessageBox::information(this,"Information - Información"," Vous devez redemarrer le programme pour appliquer le changement de langue\nYou must restart the program to apply the language change\nDebe reiniciar el programa para aplicar el cambio de idioma");
    }
    else if(choixlangue == es || choixlangue == es_en)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("choixdelalangue", "es");

        QMessageBox::information(this,"Information - Información"," Vous devez redemarrer le programme pour appliquer le changement de langue\nYou must restart the program to apply the language change\nDebe reiniciar el programa para aplicar el cambio de idioma");
    }
    else
    {
        QMessageBox::critical(this,"Erreur",tr("Aucune langue ne correspond à votre choix."));
    }
}

void Fenoptions::on_Boutonsupprnot2s_clicked()
{

        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("supprimerlesnotifications", "2s");

}

void Fenoptions::on_Boutonsupprnot3j_clicked()
{

        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("supprimerlesnotifications", "3j");

}

void Fenoptions::on_Boutonsupprnot2m_clicked()
{
        QSettings options("config_loquii.ini", QSettings::IniFormat);
        options.setValue("supprimerlesnotifications", "2m");
}

// ########################################################################
// Configuration de l'audo !
// ########################################################################

void Fenoptions::on_CheckEnrAudio_clicked()
{
    if(ui->CheckEnrAudio->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("Audio");
        options.setValue("enregistrementaudio", true);
        options.endGroup();
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("Audio");
        options.setValue("enregistrementaudio", false);
        options.endGroup();
    }
}

void Fenoptions::on_ListePeriAudio_currentIndexChanged()
{
    QSettings options("config_loquii.ini", QSettings::IniFormat);
    options.beginGroup("Audio");

    options.setValue("peripheriqueaudio", ui->ListePeriAudio->currentText());

    options.endGroup();
}

void Fenoptions::on_QuaAudio_valueChanged()
{
    QSettings options("config_loquii.ini", QSettings::IniFormat);
    options.beginGroup("Audio");

    options.setValue("qualiteaudio", ui->QuaAudio->value());

    options.endGroup();
}

void Fenoptions::on_ChoixVolumeAudio_valueChanged()
{
    ui->ChoixVolumeAudio->setMaximum(100);

    QSettings options("config_loquii.ini", QSettings::IniFormat);
    options.beginGroup("Audio");

    options.setValue("volumeaudio", ui->ChoixVolumeAudio->value());
    ui->ProgressVolumeAudio->setValue(ui->ChoixVolumeAudio->value());
    options.endGroup();
}

void Fenoptions::on_ListePeriVideo_currentIndexChanged()
{
    QSettings options("config_loquii.ini", QSettings::IniFormat);
    options.beginGroup("Video");

    options.setValue("peripheriquevideo", ui->ListePeriVideo->currentText());

    options.endGroup();
}

void Fenoptions::on_QuaVideo_valueChanged()
{
    QSettings options("config_loquii.ini", QSettings::IniFormat);
    options.beginGroup("Video");

    options.setValue("qualitevideo", ui->QuaVideo->value());

    options.endGroup();
}

void Fenoptions::on_CheckDemAutoVideo_clicked()
{
    if(ui->CheckDemAutoVideo->isChecked() == true)
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("Video");
        options.setValue("demarrerautovideo", true);
        options.endGroup();
    }
    else
    {
        QSettings options("config_loquii.ini", QSettings::IniFormat);

        options.beginGroup("Video");
        options.setValue("demarrerautovideo", false);
        options.endGroup();
    }
}

Fenoptions::~Fenoptions()
{
    delete ui;
}
