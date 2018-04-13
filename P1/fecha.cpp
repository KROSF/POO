#include <cstdio>
#include "fecha.hpp"
/* CONSTRUCTORES */

Fecha::Fecha(int d, int m, int a) : _d(d),_m(m),_a(a) {
    if(_d==0||_m==0||_a==0) {
        Fecha::Hrsys hrsys;//Objeto con la fecha del sistema
        if(_d == 0) _d = hrsys.dia();
        if(_m == 0) _m = hrsys.mes();
        if(_a == 0) _a = hrsys.anio();
    }
    es_valida();
}

Fecha::Fecha(const char * fecha) : _d(22),_m(22),_a(22) {
    if(std::sscanf(fecha,"%d/%d/%d",&_d,&_m,&_a) != 3) throw Invalida("Error de conversión");
    if(_d==0||_m==0||_a==0) {
        Fecha::Hrsys hrsys;//Objeto con la fecha del sistema
        if(_d==0) _d = hrsys.dia();
        if(_m==0) _m = hrsys.mes();
        if(_a==0) _a = hrsys.anio();
    }
    es_valida();
}

/* METODOS PRIVADOS */

void Fecha::es_valida() const {
    if( _a < Fecha::AnnoMinimo || _a > Fecha:: AnnoMaximo ) throw Invalida("Año Invalido");
    if( _m < 1 || _m > 12 ) throw Invalida("Mes Incorrecto");
    int bisiesto = static_cast<int>(_a % 4 == 0 && ( _a % 400 == 0 || _a % 100 != 0));
    static const int DiaMes[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(_d < 1 || _d > (DiaMes[_m]+bisiesto) ) throw Invalida("Dia Incorrecto");
}

/* METODOS PUBLICOS */

inline int Fecha::dia() const noexcept { return _d; }

inline int Fecha::mes() const noexcept { return _m; }

inline int Fecha::anno() const noexcept { return _a; }

const char* Fecha::cadena() const{
  Fecha::Hrsys shrsys(this->dia(),this->mes(),this->anno());
  return shrsys.toString();
}

/* OPERADORES DE ASIGNACIÓN */

Fecha& Fecha::operator+=(int n) {
  if(n != 0){
    Fecha::Hrsys nhrsys(this->_d,this->_m,this->_a,n);//Obejeto con la suma de ndias a una fecha
    this->_d=nhrsys.dia();
    this->_m=nhrsys.mes();
    this->_a=nhrsys.anio();
    es_valida();
  }
  return *this;
}

Fecha& Fecha::operator-=(int n) { return *this += (-n);}

/* OPERADORES ARIMETICOS */

Fecha Fecha::operator + (int n) const { return Fecha(*this) += n;}

Fecha Fecha::operator - (int n) const { return Fecha(*this) += -n;}

/* OPERADORES DE INCRIMETO Y  DECREMENTO */

/* Prefijo */

Fecha& Fecha::operator ++ () { return *this += 1; }

Fecha& Fecha::operator -- () { return *this+=-1; }

/* Posfijo */

Fecha Fecha::operator ++ (int) {
  Fecha t = *this;
  *this += 1;
  return t;
}

Fecha Fecha::operator -- (int) {
  Fecha t= *this;
  *this+=-1;
  return t;
}

/* OPERADORES DE COMPARACIÓN */

bool operator == (const Fecha& fecha1,const Fecha& fecha2) noexcept {
  return (fecha1.dia()==fecha2.dia())
      && (fecha1.mes()==fecha2.mes())
      && (fecha1.anno()==fecha2.anno());
}
bool operator != (const Fecha& fecha1, const Fecha& fecha2) noexcept {return !( fecha1 == fecha2 );}

bool operator < (const Fecha& fecha1, const Fecha& fecha2) noexcept
{
    if ( fecha1.anno() < fecha2.anno() ) return true;
    else if ( fecha1.anno() > fecha2.anno() ) return false;
         else if ( fecha1.mes() < fecha2.mes() ) return true;
                else if ( fecha1.mes() > fecha2.mes() ) return false;
                     else return fecha1.dia() < fecha2.dia();
}
bool operator > (const Fecha& fecha1, const Fecha& fecha2) noexcept {return fecha2 < fecha1;}

bool operator <= (const Fecha& fecha1, const Fecha& fecha2) noexcept
{return ((fecha1 < fecha2) || (fecha1 == fecha2 ));}

bool operator >= (const Fecha& fecha1, const Fecha& fecha2 ) noexcept {return !(fecha1 < fecha2);}

/* OPERADORES DE FLUJO */

std::istream& operator >>(std::istream& is, Fecha& fecha)
{
  char tmp[11];
  is.getline(tmp,11);
  try{
    fecha = Fecha(tmp);
  }catch(const Fecha::Invalida& e){
    is.setstate(std::ios_base::failbit);
    throw;
  }
  return is;
}

std::ostream& operator <<(std::ostream& os, const Fecha& fecha) noexcept
{
	os << fecha.cadena();
	return os;
}
