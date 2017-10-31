#include <fstream>
#include <string>
#include <Parser/SmartParser.h>
#include <QString>
#include <Converter/Conv.h>
#include "compiler.h"
#include <map>
#include <ostream>
#include <vector>
#include <IAS/Assembler.h>
#include <list>

using std::ifstream;
using std::string;
using std::list;
using std::ofstream;
using std::map;
using DP::AS::parse;
using std::ostream;
using DP::Parser::SmartParser;


enum class DEF{Null, LEFT, RIGHT, BEGIN, END, Draw, Exec, SYS_DRAW, SYS_SAVE,SYS_LOAD};

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

map<string, list<Symbol>> _functions;

bool ParseComandsNoStream(const string& name, const string& cmd, string& err, int&pos){
    list<Symbol> commands;
    bool isExFun = false;
    for (int i = pos; i<cmd.size(); i++){
        DEF type = parseSym(cmd[i]);
        if (type!=DEF::Draw){
            if (type == DEF::BEGIN){
                string str = cmd.substr(i);
                string func_name = name + "_" + DP::Converter::toString(numer_func++);
                commands.push_back(Symbol(DEF::BEGIN));
                commands.push_back(Symbol(func_name));
                if (!ParseComandsNoStream(func_name , cmd, err, ++i)){
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
    _functions[name]=commands;
    return true;
}

bool generateCodesPart_1(list<Symbol>& result, bool& isOptimal, int depth, int maxdepth, const string & drawable, string&err){
    for (auto x= result.begin(); x!= result.end(); x++){
        Symbol cmd = *x;
        switch (cmd.type){
            case DEF::LEFT:
                continue;
            case DEF::RIGHT:
                continue;
            case DEF::SYS_DRAW:
                continue;
            case DEF::SYS_LOAD:
                continue;
            case DEF::SYS_SAVE:
                continue;
            case DEF::BEGIN:
                result.insert(x, DEF::SYS_SAVE);
                {
                    auto tmp = x;
                    tmp--;
                    result.erase(x);
                    x=tmp;
                }
                //!!!
                continue;
                break;
            case DEF::END:
                result.insert(x, DEF::SYS_LOAD);
                {
                    auto tmp = x;
                    tmp--;
                    result.erase(x);
                    x=tmp;
                }
                //!!!
                continue;
                break;
        }
        if (cmd.type == DEF::Draw || cmd.type == DEF::Exec){
            isOptimal = false;
            if (cmd.type == DEF::Draw)
                if (cmd.c == drawable[0])
                    if (depth >= maxdepth){
                        result.insert(x, DEF::SYS_DRAW);
                        isOptimal=true;
                        {
                            auto tmp = x;
                            tmp--;
                            result.erase(x);
                            x=tmp;
                        }
                    }
            if (cmd.type == DEF::Exec && cmd.func == drawable && depth >= maxdepth){
                result.insert(x, DEF::SYS_DRAW);
                isOptimal=true;
                {
                    auto tmp = x;
                    tmp--;
                    result.erase(x);
                    x=tmp;
                }
            }
            if (!isOptimal){

                string nameF = "";
                if (cmd.type == DEF::Draw)
                    nameF += cmd.c;
                else
                    nameF = cmd.func;

                if (!DP::Converter::ConteinsKey(_functions, nameF)){
                    err = "Function '" + nameF + "' is not found";
                    return false;
                }
                {
                    auto & tmp = _functions[nameF];
                    auto it = x;
                    for (auto y = tmp.rbegin(); y!=tmp.rend(); y++){
                        it = result.insert(it, *y);
                    }
                }
                {
                    auto tmp = x;
                    tmp--;
                    result.erase(x);
                    x=tmp;
                    if (x == result.end())
                        x++;
                }
            }
        }
    }
    return true;
}

bool generateCodes(ostream& os , map<string, string>& val, string &err){
    os << "main:\n";
    string main="F";
    string drawable="F";
    if (DP::Converter::ConteinsKey(val, "main"))
        main = val["main"];
    if (DP::Converter::ConteinsKey(val, "draw"))
        drawable = val["draw"];
    list<Symbol> result;
    if (!DP::Converter::ConteinsKey(_functions, main)){
        err = "Function '" + main + "' is not found";
        return false;
    }
    int depth = 0;
    int maxdepth=3;
    if (DP::Converter::ConteinsKey(val, "depth"))
        maxdepth = parse<int>(val["depth"]);

    while (true){
        bool isOptimal=true;
        auto _sys_fun = _functions;
        for (auto x = _sys_fun.begin(); x!=_sys_fun.end(); x++){
            if (!generateCodesPart_1(x->second, isOptimal, depth, maxdepth, drawable, err))
                return false;
        }
        _functions= _sys_fun;
        if (depth > maxdepth)
            break;
        depth++;
        if (isOptimal)
            break;
    }
    {
        auto & tmp = _functions[main];
        for (auto x = tmp.cbegin(); x!=tmp.cend(); x++){
            result.push_back(*x);
        }
    }



    for (auto x = val.begin(); x!=val.end(); x++){
        if (x->first == "main"){
            continue;
        }
        if (x->first == "draw"){
            continue;
        }
        if (x->first == "force_inline")
            continue;
        os << "\tSET "<<(*x).first<<" " << x->second<<"\n";
    }


    for (auto x = result.cbegin(); x!= result.cend(); x++){
        Symbol c = *x;
        if (c.type == DEF::Draw || c.type == DEF::Exec){
            string nameF = "";
            if (c.type == DEF::Draw)
                nameF += c.c;
            else
                nameF = c.func;
            if (nameF == drawable)
                os << "DRAW";
        }
        os << "\t";
        if (c.type == DEF::LEFT)
            os << "LEFT";
        if (c.type == DEF::RIGHT)
            os << "RIGHT";
        if (c.type == DEF::SYS_DRAW)
            os << "DRAW";
        if (c.type == DEF::SYS_LOAD)
            os << "LOAD_POS";
        if (c.type == DEF::SYS_SAVE)
            os << "SAVE_POS";
        os <<"\n";
    }
    os << "end:";
}

bool generateCodesFast(ostream& os , map<string, string>& val, string &err){
    os << "main:\n";
    string main="F";
    string drawable="F";
    if (DP::Converter::ConteinsKey(val, "main"))
        main = val["main"];
    if (DP::Converter::ConteinsKey(val, "draw"))
        drawable = val["draw"];
    list<Symbol> result;
    if (!DP::Converter::ConteinsKey(_functions, main)){
        err = "Function '" + main + "' is not found";
        return false;
    }
    {
        auto & tmp = _functions[main];
        for (auto x = tmp.cbegin(); x!=tmp.cend(); x++){
            result.push_back(*x);
        }
    }
    int depth = 0;
    int maxdepth=3;
    if (DP::Converter::ConteinsKey(val, "depth"))
        maxdepth = parse<int>(val["depth"]);

    while (true){
        bool isOptimal=true;
        if (!generateCodesPart_1(result, isOptimal, depth, maxdepth, drawable, err))
            return false;
        if (depth > maxdepth)
            break;
        depth++;
        if (isOptimal)
            break;
    }
    for (auto x = val.begin(); x!=val.end(); x++){
        if (x->first == "main"){
            continue;
        }
        if (x->first == "draw"){
            continue;
        }
        if (x->first == "fast")
            continue;
        if (x->first == "force_inline")
            continue;
        os << "\tSET "<<(*x).first<<" " << x->second<<"\n";
    }


    for (auto x = result.cbegin(); x!= result.cend(); x++){
        Symbol c = *x;
        if (c.type == DEF::Draw || c.type == DEF::Exec){
            string nameF = "";
            if (c.type == DEF::Draw)
                nameF += c.c;
            else
                nameF = c.func;
            if (nameF == drawable)
                os << "DRAW";
        }
        os << "\t";
        if (c.type == DEF::LEFT)
            os << "LEFT";
        if (c.type == DEF::RIGHT)
            os << "RIGHT";
        if (c.type == DEF::SYS_DRAW)
            os << "DRAW";
        if (c.type == DEF::SYS_LOAD)
            os << "LOAD_POS";
        if (c.type == DEF::SYS_SAVE)
            os << "SAVE_POS";
        os <<"\n";
    }
    os << "end:";
}

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
        if (c.type==DEF::BEGIN){
            i++;
            string comName="";
            if (commands[i].type==DEF::Exec)
                comName = commands[i].func;
            else
                comName = commands[i].c;
            string label_name = "label_"+name + "_" + DP::Converter::toString(numer_func++);

            os << "RUN " << label_name << " " << comName<<"\n";
            os <<label_name<<":\n\t";
            os <<"LOAD";
        }
        if (c.type == DEF::Draw){
            string label_name = "label_"+name + "_" + DP::Converter::toString(numer_func++);
            os << "SAVE " << label_name <<"\n\t";
            os << "EXEC "<< c.c;
            os <<"\n"<<label_name<<":";
        }
        //if (c.type==DEF::END){
        //    os << "LOAD";
        //}
        if (c.type == DEF::RIGHT)
            os << "RIGHT";
        if (c.type==DEF::Exec){
            string label_name = "label_"+name + "_" + DP::Converter::toString(numer_func++);
            os << "SAVE " << label_name <<"\n\t";
            os << "EXEC "<< c.func;
            os <<"\n"<<label_name<<":";
        }
        os <<"\n";
    }
    //if (isExFun)
      //  os << "\tPOP\n";
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
    _functions.clear();
    map<string, string> val;
    while (!in.eof()){
        line_numer++;
        string line;
        getline(in, line);
        if (func.Scan(line)){
            string err;
            int sdff=0;
            if (DP::Converter::ConteinsKey(val, "force_inline")){
                ParseComandsNoStream(func.GetParam("name"), func.GetParam("func"), err, sdff);
            } else
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
    if (DP::Converter::ConteinsKey(val, "force_inline")){
        string er;
        if (DP::Converter::ConteinsKey(val, "fast")){
            if (generateCodesFast(os, val, er))
                errors.push_back(QString::fromStdString(er));
        } else
            if (!generateCodes(os, val,  er))
                errors.push_back(QString::fromStdString(er));
        return errors;
    } else {
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
}

QVector<QString> Compile(QString file, QString file_out){
    return Compile(file.toStdString(), file_out.toStdString());
}
