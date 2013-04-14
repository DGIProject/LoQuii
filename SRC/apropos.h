#ifndef APROPOS_H
#define APROPOS_H

#include <QDialog>

namespace Ui {
class apropos;
}

class apropos : public QDialog
{
    Q_OBJECT
    
public:
    explicit apropos(QWidget *parent = 0);
    ~apropos();

private slots:

    void on_BoutonOk_clicked();
    
private:
    Ui::apropos *ui;
};

#endif // APROPOS_H
