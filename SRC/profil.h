#ifndef PROFIL_H
#define PROFIL_H

#include <QWidget>

namespace Ui {
class profil;
}

class profil : public QWidget
{
    Q_OBJECT
    
public:
    explicit profil(QWidget *parent = 0);
    ~profil();
    
private:
    Ui::profil *ui;
};

#endif // PROFIL_H
