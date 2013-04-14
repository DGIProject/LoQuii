#ifndef FENDEM_H
#define FENDEM_H

#include <QWidget>

namespace Ui {
class FenDem;
}

class FenDem : public QWidget
{
    Q_OBJECT
    
public:
    explicit FenDem(QWidget *parent = 0);
    ~FenDem();
    
private:
    Ui::FenDem *ui;
};

#endif // FENDEM_H
