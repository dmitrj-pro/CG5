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
    std::list<int> StackDepth;
    int maxStackDepth=maxdepth;
    std::list<string> labels;
    string thisfunc="";

    Point prev=start;

    string fghld(){
        string res="";
        for (auto x = labels.cbegin(); x!= labels.cend(); x++){
            res += *x+"\n";
        }
        return res;
    }

    void clear(){
        depth=0;
        maxdepth=3;
        step=20;
        start=Point(40,40);
        ugol = 0;
        drawFunc="F";
        L=ugol;
        maxStackDepth = maxdepth;
        Stack.clear();
        StackDepth.clear();
        labels.clear();
        prev=start;
    }

    Error SET(Language& , const vector<string>&par){
        string name = par[0];
        string val=par[1];
        if (name == "depth"){
            maxdepth = parse<int>(val);
            maxStackDepth = maxdepth;
            return Error();
        }
        if (name == "step"){
            step=parse<int>(val);
            return Error();
        }
        if (name == "main"){
            drawFunc = val;
            return Error();
        }
        if (name == "start_ugol"){
            double tmp = parse<int>(val);
            tmp = tmp*3.1415/180;
            ugol= tmp;
            return Error();
        }
        if (name == "point"){
            start = Point(parse<double>(val), parse<double>(par[2]));
            prev=start;
            return Error();
        }
        if (name == "exec"){
            drawFunc = val;
            return Error();
        }
        if (name == "ugol"){
            double tmp = parse<double>(val);
            tmp = tmp*3.1415/180;
            L = tmp;
            return Error();
        }
        return Error(Error::TypeError::CRYTICAL, "Non found parametr");
    }
    Error SAVE(Language& , const vector<string>&par){
        logl("save");
        labels.push_back(par[0]);
        return Error();
    }
    Error SAVE_POS(Language&, const vector<string>&){
        logl("save position");
        Stack.push_back(std::pair<double, Point>(ugol, prev));
        return Error();
    }
    Error LOAD_POS(Language&lan, const vector<string>&){
        logl("load position");
#ifdef DP_DEBUG
        auto sss = Stack;
        auto ddd = labels;
#endif
        if (Stack.size() == 0){
            vector<string> tmp;
            tmp.push_back("end");
            DP::AS::jmp(lan, tmp);
            return Error(DP::AS::Error::TypeError::NoError, "Stack is empty");
        }
        auto tmp = Stack.back();
        ugol=tmp.first;
        prev=tmp.second;
        Stack.pop_back();
        return Error();
    }

    Error RET(Language&lan, const vector<string>&g){
        logl("ret");
        //if (StackDepth.size() == (maxStackDepth-1)){
          //  labels.pop_back();
        //}
#ifdef DP_DEBUG
        auto fdf=labels;
        auto sssdd = StackDepth;
        string ddd =fghld();
#endif
        if (labels.size() == 0){
            vector<string> tmp;
            tmp.push_back("end");
            DP::AS::jmp(lan, tmp);
            return Error(DP::AS::Error::TypeError::NoError, "Stack is empty");
        }
        //if (thisfunc == drawFunc)
            depth--;
        string lab = labels.back();
        labels.pop_back();
        vector<string> x;
        x.push_back(lab);
        DP::AS::jmp(lan,x);
        return Error();
    }

    Error POP(Language&lan, const vector<string>&g){
        logl("ret");
        labels.pop_back();
        return Error();
    }
    Error RET_no_depth(Language&lan, const vector<string>&g){
        logl("ret_no_depth");
        #ifdef DP_DEBUG
            string ddd =fghld();
        #endif
        string lab = labels.back();
        labels.pop_back();
        vector<string> x;
        x.push_back(lab);
        DP::AS::jmp(lan,x);
        return Error();
    }
    Error DRAW(Language&, const vector<string> &){
        logl("draw");
        double x= ((double)step)*std::cos(ugol);
        double y = ((double)step)*std::sin(ugol);
        double tmp = std::sqrt(x*x+y*y);
        scen->addLine(prev.x,prev.y, x+prev.x, y + prev.y);
        logl("");
        prev=Point(x+prev.x, y + prev.y);
        return Error();
    }

    Error RUN(Language& lan, const vector<string>&par) {
        Dlog("run " + par[1]);
        string save= par[0];
        string run = par[1];
#ifdef DP_DEBUG
        auto& fdf=labels;
        auto& sssdd = StackDepth;
        string ddd =fghld();
#endif
        Stack.push_back(std::pair<double, Point>(ugol, prev));
        StackDepth.push_back(depth);

        depth++;
        if (depth > (maxdepth+1)){
            vector<string> tmp;
            tmp.push_back(save);
            return DP::AS::jmp(lan, tmp);
        }

        if ((StackDepth.size()) > maxStackDepth){
            vector<string> tmp;
            tmp.push_back(save);
            return DP::AS::jmp(lan, tmp);
        }
        depth=StackDepth.size()+1;
        labels.push_back(save);
        vector<string> tmp;
        tmp.push_back(run);
        return DP::AS::jmp(lan, tmp);
    }

    Error EXEC(Language& lan, const vector<string>&par){
        string name = par[0];
        Dlog("exec");
        logl(name);
        string ddd =fghld();
        //if (StackDepth.size() >= maxStackDepth)
          //  return RET_no_depth(lan, par);
        if (name == drawFunc && depth == maxdepth){
            logl("draw");
            double x= ((double)step)*std::cos(ugol);
            double y = ((double)step)*std::sin(ugol);
            double tmp = std::sqrt(x*x+y*y);
            scen->addLine(prev.x,prev.y, x+prev.x, y + prev.y);
            logl("");
            prev=Point(x+prev.x, y + prev.y);
        }
        //if (name == drawFunc)
            depth++;
        if (depth > maxdepth)
            return RET(lan, par);
        thisfunc = par[0];
        DP::AS::jmp(lan,par);
        return Error();
    }
    Error LOAD(Language&lan, const vector<string>&){
        logl("load");
#ifdef DP_DEBUG
        auto sss = Stack;
        auto ddd = labels;
#endif
        if (Stack.size() == 0){
            vector<string> tmp;
            tmp.push_back("end");
            DP::AS::jmp(lan, tmp);
            return Error(DP::AS::Error::TypeError::NoError, "Stack is empty");
        }
        auto tmp = Stack.back();
        ugol=tmp.first;
        prev=tmp.second;
        Stack.pop_back();

        int tmp_depth = StackDepth.back();
        depth = tmp_depth;
        StackDepth.pop_back();

        return Error();
    }
    Error PUSH_STACK(Language& lan, const vector<string>&f){
        if ((StackDepth.size()) >= maxStackDepth){
            return RET_no_depth(lan, f);
        }
        logl("reset_depth");
        StackDepth.push_back(depth);
        depth=-1;
        return Error();
    }

    Error LEFT(Language&, const vector<string>&){
        logl("left");
        ugol+=L;
        return Error();
    }
    Error RIGHT(Language&, const vector<string>&){
        logl("right");
        ugol-=L;
        return Error();
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
    as.addFunction("POP", Lan::POP,0);
    as.addFunction("PUSH_STACK", Lan::PUSH_STACK, 0);
    as.addFunction("LOAD_POS", Lan::LOAD_POS, 0);
    as.addFunction("SAVE_POS", Lan::SAVE_POS, 0);
    as.addFunction("DRAW", Lan::DRAW, 0);
    as.addFunction("RUN", Lan::RUN, 2);
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
    Lan::scen = new QGraphicsScene(0,0, this->size().rwidth(), this->size().rheight(),this);
    ui->graphicsView->setScene(Lan::scen);
    Dlog(l);
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
