#include "draw.h"
#include "ui_draw.h"
#include <QFileDialog>
#include <IAS/Assembler.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <list>
#include <sstream>
#include "compiler.h"
#include <string>
#include <cmath>

#ifdef DP_DEBUG
#include <iostream>

using std::cout;
#endif

using namespace DP::AS;
using std::string;
using std::istringstream;


/*template <typename T>
T parse(const string& str){
    istringstream os;
    os.str(str);
    T res;
    os >> res;
    return res;
}*/


namespace Lan{
    QGraphicsScene * scen;
    struct Point{
        double x;
        double y;
        Point(double x, double y):x(x),y(y){}
        Point(){}
    };

    int depth=0;
    int maxdepth=3;
    int step=20;
    Point start(40,40);
    double ugol = 30;
    string drawFunc="F";
    double L=ugol;
    std::list<std::pair<double, Point>> Stack;
    std::list<string> labels;

    Point prev=start;

    void clear(){
        depth=0;
        maxdepth=3;
        step=20;
        start=Point(40,40);
        ugol = 0;
        drawFunc="F";
        L=ugol;
        Stack.clear();
        labels.clear();
        prev=start;
    }

    Error SET(Language& , const vector<string>&par){
        string name = par[0];
        string val=par[1];
        if (name == "depth"){
            maxdepth = parse<int>(val);
            return Error();
        }
        if (name == "step"){
            step=parse<int>(val);
            return Error();
        }
        if (name == "ugol"){
            double tmp = parse<int>(val);
            tmp = tmp*3.1415/180;
            L = tmp;
            return Error();
        }
        return Error(Error::TypeError::CRYTICAL, "Non found parametr");
    }
    Error SAVE(Language& , const vector<string>&par){
        Stack.push_back(std::pair<double, Point>(ugol, prev));
        labels.push_back(par[0]);
        return Error();
    }
    Error RET(Language&lan, const vector<string>&g){
        auto fdf=labels;
        depth--;
        string lab = labels.back();
        labels.pop_back();
        vector<string> x;
        x.push_back(lab);
        DP::AS::jmp(lan,x);
    }
    Error EXEC(Language& lan, const vector<string>&par){
        string name = par[0];
        if (name == drawFunc){
            double x= step*std::cos(ugol);
            double y = step*std::sin(ugol);
            scen->addLine(prev.x,prev.y, x+prev.x, y + prev.y);
            Dlog(x);
            Dlog(y);
            Dlog(std::sqrt(x*x+y*y));
            Dlog(prev.x);
            Dlog(prev.y);
            logl("");
            prev=Point(x+prev.x, y + prev.y);
        }
        depth++;
        if (depth > maxdepth)
            return RET(lan, par);
        DP::AS::jmp(lan,par);
        return Error();
    }
    Error LOAD(Language&, const vector<string>&){
        auto tmp = Stack.back();
        Stack.pop_back();
        ugol=tmp.first;
        prev=tmp.second;
        return Error();
    }
    Error LEFT(Language&, const vector<string>&){
        ugol+=L;
    }
    Error RIGHT(Language&, const vector<string>&){
        ugol-=L;
    }



}
void LoadCommandUser(DP::AS::Language& as){
    as.addFunction("SET", Lan::SET,2);
    as.addFunction("SAVE", Lan::SAVE,1);
    as.addFunction("EXEC", Lan::EXEC, 1);
    as.addFunction("LOAD", Lan::LOAD,0);
    as.addFunction("LEFT", Lan::LEFT,0);
    as.addFunction("RIGHT", Lan::RIGHT,0);
    as.addFunction("RET", Lan::RET,0);


}

Draw::Draw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Draw)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->verticalLayoutWidget);
    connect(ui->open, SIGNAL(released()), this, SLOT(open()));
    connect(ui->edit, SIGNAL(released()), this, SLOT(edit()));
    connect(ui->Draw_2, SIGNAL(released()), this, SLOT(Exec()));
    ed= new Editer();
}

void Draw::Exec(){
    Lan::clear();
    Compile(file_name, "fff.txt");
    DP::AS::Language l;
    l.loadCore();
    l.read("fff.txt");
    Lan::scen = new QGraphicsScene(ui->graphicsView->sceneRect(),this);
    ui->graphicsView->setScene(Lan::scen);
#ifdef DP_DEBUG
    cout << l;
#endif
    l.exec("main");

}

void Draw::edit(){
    ed->SetFile(file_name);
    ed->Open();
    ed->show();
}

inline bool getUserFile(QString & res){
    QFileDialog * d =new QFileDialog();
    res = d->getOpenFileName(0, "Open Dialog", "", "*.txt *.conf *.cfg *.log");
    delete d;
}

void Draw::open(){
    getUserFile(file_name);
}

Draw::~Draw()
{
    delete ui;
}
