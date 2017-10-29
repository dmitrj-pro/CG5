#ifndef EDITER_H
#define EDITER_H

#include <QMainWindow>

#ifdef DP_DEBUG
#include <iostream>
#endif

#ifdef DP_DEBUG
using std::cout;
#endif


template <typename T>
inline void log(const T& x){
    #ifdef DP_DEBUG
        cout <<x << " ";
        cout.flush();
    #endif
}
template <typename T>
inline void Dlog(const T& x){
    #ifdef DP_DEBUG
        cout <<x << " ";
        cout.flush();
    #endif
}

template <typename T>
inline void logl(const T& x){
    #ifdef DP_DEBUG
        cout <<x << "\n";
        cout.flush();
    #endif
}


namespace Ui {
class Editer;
}

class Editer : public QMainWindow
{
    Q_OBJECT

public:
    //explicit Editer(QWidget *parent = 0);
    explicit Editer(QWidget *parent = 0);
    ~Editer();
    void Open();

private:
    Ui::Editer *ui;
    QString file_name;

public:
    inline void SetFile(QString filename){
        file_name = filename;
    }

public slots:
    void save();
    void compile();
};

#endif // EDITER_H
