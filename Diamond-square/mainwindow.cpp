#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <cstdlib>

using std::string;
using std::vector;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->verticalLayout_2);
    connect(ui->openDialog, SIGNAL (released()),this, SLOT (showMessage()));
    image = new ExecuteDrow(ui->graphicsView, 800,800);
}

MainWindow::~MainWindow()
{
    delete ui;
}

template <typename T>
struct Point{
    T x;
    T y;
    Point(){}
    Point(const T& x, const T& y):x(x),y(y){}
};


std::list<Point<double>> points;

inline int rand(int a, int b){
    return rand()%(b-a)+a;
}

void generate(size_t depth, double kof){
    points.clear();
    points.push_back(Point<double>(0,0));
    points.push_back(Point<double>(512,0));
    points.push_back(Point<double>(1024,0));

    for (auto x= points.begin(); x!=points.end();x++){
        x->y = rand(0,100);
    }

    auto & tmff = points;
    int k=0;
    while (k<depth){
        auto it = points.begin();
        auto prev = *it;
        it++;
        while (it != points.end()){
            auto th = *it;
            auto tmp = th;
            tmp.x = (th.x + prev.x)/2;
            double ttt = (th.y + prev.y)/2.0;
            tmp.y = ttt + (rand(-100,100))*kof;
            if (tmp.y <=0)
                tmp.y=0;

            points.insert(it, tmp);

            prev=th;

            it++;
        }
        k++;
    }

}

double demon(double xy){
    auto prev = *points.begin();
    for (auto x = points.begin(); x!= points.end(); x++){
        if (xy < x->x)
            return (prev.y + x->y)/2;
        prev = *x;
    }
}

void MainWindow::showMessage(){
    double val = ui->horizontalSlider->value();
    val = val/1000.0;
    generate(12, val);
    image->Draw(demon, 0, 1000);
    on_MainWindow_iconSizeChanged();
}

void MainWindow::resizeEvent ( QResizeEvent*)
{
    on_MainWindow_iconSizeChanged();
}

void MainWindow::on_MainWindow_iconSizeChanged()
{
    if (ui !=nullptr && ui->graphicsView!=nullptr && image != nullptr){
        log("ReSize");
        log(ToString(ui->graphicsView->size().width()));
        image->ReDraw(ui->graphicsView->size().width(),ui->graphicsView->size().height());

    }

}
