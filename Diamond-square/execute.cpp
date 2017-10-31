#include "execute.h"
#include <vector>

using std::vector;

struct Point{
    double x;
    double y;
    Point(double x, double y):x(x),y(y){}
};

vector<Point> getFalues(Fuinction f, double a, double b, double step){
    vector<Point> res;
    for (double i = a; i<=b; i+=step){
        double tmp = f(i);
        Point x (i, tmp);
        res.push_back(x);
    }
    return res;
}

void ExecuteDrow::Draw(Fuinction f, double a, double b){
    //Для отображения на интерфейсе при изменении размера
    _a = a;
    _b =b;
    _f =f;
    if (a==b)
        return;
    image->clear();

    int weight = _weigth-40;
    int height = _height-40;

    double step = weight/ ((double)(b-a));
    step/=2;
    auto points = getFalues(f,a,b,step);

    double minX=points[0].x;
    double minY=points[0].y;
    double maxX = points[0].x;
    double maxY = points[0].y;

    for (auto x = points.begin(); x!= points.end(); x++){
        if (minX > x->x)
            minX = x->x;
        if (maxX < x->x)
            maxX = x->x;
        if (minY > x->y)
            minY = x->y;
        if (maxY < x->y)
            maxY = x->y;
    }

    double delX = minX<0 ? (-1)*minX:0;
    double delY = minY<0 ? (-1)*minY:0;


    double stepX = weight / (maxX-minX);
    double stepY = height / (120 - 0);

    Point prev = points[0];

    for (auto x = points.begin(); x!= points.end(); x++){
        image->addLine(stepX* (prev.x + delX), height - stepY*(prev.y+delY), stepX*(x->x+delX), height - stepY*(x->y+delY),_pen);
        image->addLine(stepX* (prev.x + delX), height - stepY*(prev.y+delY), stepX*(x->x+delX), height - stepY*(0+delY),_pen);
        prev = *x;
    }

}
