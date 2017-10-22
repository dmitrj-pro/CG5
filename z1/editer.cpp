#include "editer.h"
#include "ui_editer.h"
#include <fstream>
#include "compiler.h"
#include <string>

using std::ifstream;
using std::string;
using std::ofstream;


Editer::Editer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editer), file_name("")
{
    ui->setupUi(this);
    this->setCentralWidget(ui->verticalLayoutWidget);
    connect(ui->save, SIGNAL(released()), this, SLOT(save()));
    connect(ui->compile, SIGNAL(released()), this, SLOT(compile()));
}

void Editer::save(){
    ofstream os;
    os.open(file_name.toStdString());
    os << ui->TextEdit->document()->toPlainText().toStdString();
    os.close();
}

void Editer::compile(){
    Compile(file_name, "fff.txt");
}

void Editer::Open(){
    if (file_name == "" )
        return;

    ifstream os;
    os.open(file_name.toStdString());
    if (os.fail()){
        log("Fail open file");
        throw 1;
    }

    QString str="";

    while (!os.eof()){
        string tmp;
        getline(os, tmp);
        str+=QString::fromStdString(tmp)+"\n";
    }
    os.close();
    ui->TextEdit->document()->setPlainText(str);

}

Editer::~Editer()
{
    delete ui;
}
