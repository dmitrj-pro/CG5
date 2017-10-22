#ifndef DRAW_H
#define DRAW_H

#include <QMainWindow>
#include "editer.h"

namespace Ui {
class Draw;
}

class Draw : public QMainWindow
{
    Q_OBJECT

public:
    explicit Draw(QWidget *parent = 0);
    ~Draw();

public slots:
    void edit();
    void open();
    void Exec();
private:
    Ui::Draw *ui;
    Editer * ed;
    QString file_name;

};

#endif // DRAW_H
