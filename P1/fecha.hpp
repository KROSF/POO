#ifndef FECHA_H
#define FECHA_H
#include <iostream>
#include <cstring>
using namespace std;
class Fecha
{
public:
    //Clase Para manejar las exepciones
    class Invalida {
    public:
        explicit Invalida(const char * msg) : _msg(msg){}
        const char * por_que() const { return _msg; }
    private:
        const char * _msg;
    };
    //Constantes
    static const int AnnoMinimo = 1902;
    static const int AnnoMaximo = 2037;
    //Constructores
    explicit Fecha(int d = 0,int m = 0,int a = 0);
    Fecha(const char * );
    //Metodos observadores
    int dia() const noexcept;
    int mes() const noexcept;
    int anno() const noexcept;

    Fecha& operator +=(int);
    Fecha& operator -=(int);
    Fecha operator + (int) const;
    Fecha operator - (int) const;
    Fecha& operator ++ ();//prefijo
    Fecha operator ++ (int);
    Fecha& operator -- ();
    Fecha operator -- (int);
    //operador de conversión
    operator const char *() const;
private:
    int _d,_m,_a;
    //Validador fecha
    void es_valida() const;
};
bool operator == (const Fecha&, const Fecha&);
bool operator != (const Fecha&, const Fecha&);
bool operator < (const Fecha&, const Fecha&);
bool operator > (const Fecha&, const Fecha&);
bool operator <= (const Fecha&, const Fecha&);
bool operator >= (const Fecha&, const Fecha&);
#endif
