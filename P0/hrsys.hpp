#ifndef HRSYS_H
#define HRSYS_H
#include <clocale>
#include <ctime>

class Hrsys {
public:
    explicit Hrsys();
    Hrsys(int d,int m,int a,int n = 0);
    int dia() const;
    int mes() const;
    int anio() const;
    const char * toString() const;
private:
    std::time_t t1;
    std::tm* t2;
};

inline Hrsys::Hrsys() : t1(std::time(nullptr)),t2(std::localtime(&t1)) {
    std::setlocale(LC_TIME, "es_ES.UTF-8");
}

inline Hrsys::Hrsys(int d,int m,int a,int n) : t1(std::time(nullptr)),t2(new std::tm) {
    *t2 ={0};
    t2->tm_mday = (d+n);
    t2->tm_mon = m-1;
    t2->tm_year = a-1900;
    std::mktime(t2);
}

inline int Hrsys::dia() const {return t2->tm_mday;}

inline int Hrsys::mes() const {return t2->tm_mon+1;}

inline int Hrsys::anio() const {return t2->tm_year+1900;}

inline const char * Hrsys::toString() const {
        auto * s = new char[40];
        std::strftime(s, 40, "%A %d de %B de %Y", t2);
        return s;
}

#endif
