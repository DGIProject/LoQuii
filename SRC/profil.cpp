#include "profil.h"
#include "ui_profil.h"

profil::profil(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::profil)
{
    ui->setupUi(this);
}

profil::~profil()
{
    delete ui;
}
