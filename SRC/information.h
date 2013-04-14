#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>

namespace Ui {
class information;
}

class information : public QWidget
{
    Q_OBJECT
    
public:
    explicit information(QWidget *parent = 0);
    ~information();
    
private:
    Ui::information *ui;
};

#endif // INFORMATION_H
