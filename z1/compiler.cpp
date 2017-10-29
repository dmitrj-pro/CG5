#include <fstream>
#include <string>
#include <Parser/SmartParser.h>
#include <QString>
#include <Converter/Conv.h>
#include "compiler.h"
#include <map>
#include <ostream>
#include <vector>

using std::ifstream;
using std::string;
using std::ofstream;
using std::map;
using std::ostream;
using DP::Parser::SmartParser;


enum class DEF{Null, LEFT, RIGHT, BEGIN, END, Draw, Exec};

DEF parseSym(char c){
    switch(c){
        case '[':
            return DEF::BEGIN;
        case ']':
            return DEF::END;
        case '+':
            return DEF::LEFT;
        case '-':
            return DEF::RIGHT;
        default:
            return DEF::Draw;
    }
    return DEF::Null;
}

class Symbol{
    public:
        DEF type;
        char c;
        Symbol(DEF type = DEF::Null, char c=' '):type(type), c(c){}

        string func;
        Symbol(string com):func(com), type(DEF::Exec){}
};

int numer_func=1;

bool ParseComands(const string& name, const string& cmd, ostream& os, string& er, int&pos){
    vector<Symbol> commands;
    bool isExFun = false;
    for (int i = pos; i<cmd.size(); i++){
        DEF type = parseSym(cmd[i]);
        if (type!=DEF::Draw){
            if (type == DEF::BEGIN){
                string str = cmd.substr(i);
                string func_name = name + "_" + DP::Converter::toString(numer_func++);
                commands.push_back(Symbol(DEF::BEGIN));
                commands.push_back(Symbol(func_name));
                if (!ParseComands(func_name , cmd, os, er, ++i)){
                    return false;
                } else
                    isExFun = true;
                continue;
            }
            if (type==DEF::END){
                pos = i;
                commands.push_back(Symbol(type));
                break;
            }
            commands.push_back(Symbol(type));
        } else
            commands.push_back(Symbol(type, cmd[i]));
    }

    os<<""<<name<<":\n";
    for (int i = 0; i<commands.size(); i++){
        Symbol c = commands[i];
        os << "\t";
        if (c.type == DEF::LEFT)
            os << "LEFT";
        if (c.type == DEF::Draw){
            string label_name = "label_"+name + "_" + DP::Converter::toString(numer_func++);
            os << "SAVE " << label_name <<"\n\t";
            if (commands[i-1].type == DEF::BEGIN){
                os << "PUSH_STACK\n\t";
                os << "SAVE_POS\n\t";
            }
            os << "EXEC "<< c.c;
            os <<"\n"<<label_name<<":\n\t";
            //os << "LOAD";
        }
        //if (c.type==DEF::END){
        //    os << "LOAD";
        //}
        if (c.type == DEF::RIGHT)
            os << "RIGHT";
        if (c.type==DEF::Exec){
            string label_name = "label_"+name + "_" + DP::Converter::toString(numer_func++);
            os << "SAVE " << label_name <<"\n\t";
            if (commands[i-1].type == DEF::BEGIN){
                os << "SAVE_POS\n\t";
                os << "PUSH_STACK\n\t";
            }
            os << "EXEC "<< c.func;
            os <<"\n"<<label_name<<":\n\t";
            os << "LOAD";
        }

        os <<"\n";
    }
    if (isExFun)
        os << "\tPOP\n";
    os << "\tRET\n";
    return true;
}

QVector<QString> Compile(const string& in_file, const string& out_file){
    QVector<QString> errors;
    ifstream in;
    in.open(in_file);
    if (in.fail()){
        errors.push_back("Fail open file: " + QString::fromStdString(in_file));
        return errors;
    }
    ofstream os;
    os.open(out_file);
    if (os.fail()){
        errors.push_back("Fail open file: " + QString::fromStdString(in_file));
        return errors;
    }
    SmartParser define("${name}=${value}");
    SmartParser func("${name}=>${func}");
    int line_numer=0;
    map<string, string> val;
    while (!in.eof()){
        line_numer++;
        string line;
        getline(in, line);
        if (func.Scan(line)){
            string err;
            int sdff=0;
            if (!ParseComands(func.GetParam("name"), func.GetParam("func"), os,err,sdff))
                errors.push_back(QString::fromStdString(err));
        } else {
            if (define.Scan(line)){
                val[define.GetParam("name")] = define.GetParam("value");
            } else {
                errors.push_back("Error in line " + QString::number(line_numer)+": fail parse '" + QString::fromStdString(line)+"'");
            }
        }
    }
    os << "main:\n";
    string main="F";
    string drawable="F";
    for (auto x = val.begin(); x!=val.end(); x++){
        if (x->first == "main"){
            main = x->second;
            continue;
        }
        if (x->first == "draw"){
            drawable = x->second;
            continue;
        }
        os << "\tSET "<<(*x).first<<" " << x->second<<"\n";
    }
    os << "\tSET exec " << drawable << "\n";
    os << "\tSAVE end\n";
    os << "\tEXEC " << main << "\n";
    os <<"end:";

    return errors;
}

QVector<QString> Compile(QString file, QString file_out){
    return Compile(file.toStdString(), file_out.toStdString());
}
