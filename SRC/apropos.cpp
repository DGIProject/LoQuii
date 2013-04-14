#include "apropos.h"
#include "ui_apropos.h"

apropos::apropos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::apropos)
{
    ui->setupUi(this);

    setWindowTitle("LoQuiî - A Propos");
    setWindowIcon(QIcon("LoQuii/IMG/icone_lq.png"));
}

void apropos::on_BoutonOk_clicked()
{
    this->close();
}

apropos::~apropos()
{
    delete ui;
}
