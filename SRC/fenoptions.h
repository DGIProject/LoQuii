#ifndef FENOPTIONS_H
#define FENOPTIONS_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class Fenoptions;
}

class Fenoptions : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Fenoptions(QWidget *parent = 0);
    ~Fenoptions();

private slots :
    void on_Boutonaffoptionchat_clicked();
    void on_Boutonaffgeneral_clicked();
    void on_Boutonaffoptionvoc_clicked();
    void on_Boutonaffoptionvid_clicked();
    void on_bout2s_clicked();
    void on_bout3j_clicked();
    void on_bout2m_clicked();
    void on_lancerlqaudem_clicked();
    void on_absauboutde5min_clicked();
    void on_modesilencieux_clicked();
    void on_codemlq_clicked();
    void on_affemoticones_clicked();
    void on_montrerqdecrit_clicked();
    void on_notmsg_clicked();
    void on_activmaj_clicked();
    void on_choix_langue_currentIndexChanged();
    void ActuIni();
    void on_Boutonsupprnot3j_clicked();
    void on_Boutonsupprnot2m_clicked();
    void on_Boutonsupprnot2s_clicked();
    void on_CheckEnrAudio_clicked();
    void on_ListePeriAudio_currentIndexChanged();
    void on_QuaAudio_valueChanged();
    void on_ChoixVolumeAudio_valueChanged();
    void on_ListePeriVideo_currentIndexChanged();
    void on_QuaVideo_valueChanged();
    void on_CheckDemAutoVideo_clicked();
    void on_ChoixCouleurNot_currentIndexChanged();

private:
    Ui::Fenoptions *ui;
};

#endif // FENOPTIONS_H
