#ifndef FECHA_H
#define FECHA_H
#include <iostream>
class Fecha
{
public:
    /* EXEPCIONES */
    class Invalida {
    public:
        explicit Invalida(const char * msg) : _msg(msg){}
        const char * por_que() const { return _msg; }
    private:
        const char * _msg;
    };
    /* CONSTANTES */
    static const int AnnoMinimo = 1902;
    static const int AnnoMaximo = 2037;
    /* CONSTRUCTORES */
    explicit Fecha(int d = 0,int m = 0,int a = 0);
    Fecha(const char * );
    /* METODOS */
    int dia() const noexcept;
    int mes() const noexcept;
    int anno() const noexcept;
    const char* cadena() const;
    /* OPERADORES */
    Fecha& operator +=(int);
    Fecha& operator -=(int);
    Fecha operator + (int) const;
    Fecha operator - (int) const;
    Fecha& operator ++ ();//prefijo
    Fecha operator ++ (int);//posfijo
    Fecha& operator -- ();
    Fecha operator -- (int);
private:
    int _d,_m,_a;
    void es_valida() const;
};
/* OPERADORES DE COMPARACIÓN */
bool operator == (const Fecha&, const Fecha&);
bool operator != (const Fecha&, const Fecha&);
bool operator < (const Fecha&, const Fecha&);
bool operator > (const Fecha&, const Fecha&);
bool operator <= (const Fecha&, const Fecha&);
bool operator >= (const Fecha&, const Fecha&);
/* OPERADORES DE FLUJO */
std::istream& operator >>(std::istream& is, Fecha& f);
std::ostream& operator <<(std::ostream& os, const Fecha& f);
#endif
