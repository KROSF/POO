#ifndef TARJETA_HPP
#define TARJETA_HPP
#include <ostream>
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
#include "usuario.hpp"
/* Clase Numero */
class Numero{
public:
  enum Razon {LONGITUD,DIGITOS,NO_VALIDO};
  Numero(Cadena num);
  operator const char*() const{return num_.c_str();}
  friend bool operator < (const Numero& num,const Numero& num2);
/* Clase Execpcion Numero */
  class Incorrecto{
  public:
    Incorrecto(Razon rzn):rzn_(rzn){}
    Razon razon() const {return rzn_;}
  private:
    Razon rzn_;
  };
private:
  Cadena num_;
};
class Usuario;
/* Clase Tarjeta */
class Tarjeta{
public:
  enum Tipo {VISA,Mastercard, Maestro, JCB,AmericanExpress};
  /* CONSTRUCTORES */
  Tarjeta(Tipo tipo,const Numero& num,Usuario& titular,const Fecha& fecha);
  Tarjeta(const Tarjeta& ) = delete;
  /* OPERADORES */
  Tarjeta& operator= (const Tarjeta &) = delete;
  /* METODOS */
  const Tipo& tipo() const;
  const Numero& numero() const;
  Usuario* titular() const;
  const Fecha& caducidad() const;
  const Cadena& titular_facial() const;
  void anula_titular();
  /* DESTRUCTOR */
  ~Tarjeta();
  /* Clase Execpcion Tarjeta */
  class Caducada
  {
  public:
      Caducada(const Fecha& caducada) : caducada_(caducada) {}
      const Fecha& cuando() const { return caducada_;}
  private:
      Fecha caducada_;
  };
private:
  Tipo tipo_;
  Numero numero_;
  Usuario* const titular_;
  Fecha caducidad_;
  Cadena titular_facial_;
};


/* OPERADORES*/
bool operator< (const Tarjeta& card,const Tarjeta& card2);
std::ostream& operator <<(std::ostream& os,const Tarjeta::Tipo& tipo);
std::ostream& operator <<(std::ostream& os,const Tarjeta& card);
#endif
