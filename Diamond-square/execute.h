#ifndef EXECUTE_H
#define EXECUTE_H

#define DEBUG

#ifdef DEBUG
#include <iostream>
#include <string>
#endif
#include <QGraphicsScene>
#include <QGraphicsView>
#include <sstream>


#ifdef DEBUG
using std::cout;
using std::string;
#endif
using std::istringstream;
using std::ostringstream;

inline void log(const string& str){
    #ifdef DEBUG
        cout<<str<<"\n";
    #endif
}

template <typename  T>
inline string ToString(const T & data){
    ostringstream os;
    os << data;
    return os.str();
}

template <typename T>
inline T parse(QString str){
    istringstream os;
    os.str(str.toStdString());
    T res;
    os>> res;
    return res;
}

typedef double (*Fuinction)(double a);

class ExecuteDrow:public QObject{
    Q_OBJECT
    private:
        QGraphicsScene *image = nullptr;
        QGraphicsView * view = nullptr;
        Fuinction _f;
        QBrush _brush;
        QPen _pen;
        int _weigth;
        int _height;

        double _a=0;
        double _b=0;


    public:
        ExecuteDrow(QGraphicsView * view, int w, int h):view(view),_height(h), _weigth(w), _brush(Qt::red),_pen(Qt::gray){
            image = new QGraphicsScene(0,0,w,h,this);
            view->setScene(image);
        }
        void Draw(Fuinction f, double a, double b);
        inline void ReDraw(int w, int h){
            if (image == nullptr)
                return;
            delete image;
            image = new QGraphicsScene(0,0,w,h,this);
            view->setScene(image);
            _weigth = w;
            _height = h;
            Draw(_f, _a,_b);

        }

};

#endif // EXECUTE_H

