#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "execute.h"


using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void draw(int a, int b,const string& fun);
private slots:

    void showMessage();
    void on_MainWindow_iconSizeChanged();

private:
    Ui::MainWindow *ui;
    ExecuteDrow *image = nullptr;

signals:
    void resizeEvent ( QResizeEvent*);


};

#endif // MAINWINDOW_H
