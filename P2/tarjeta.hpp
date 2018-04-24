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
  Numero(Cadena);
  operator const char*() const {return num_.c_str();}
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

class Usuario;//Declaracion anticipada de usuario
/* Clase Tarjeta */
class Tarjeta{
public:
  enum Tipo {VISA,Mastercard, Maestro, JCB, AmericanExpress};
  /* CONSTRUCTORES */
  Tarjeta(Tipo t,const Numero& n,Usuario& u,const Fecha& f);
  /* Delete */
  Tarjeta(const Tarjeta& ) = delete;
  Tarjeta& operator= (const Tarjeta &) = delete;
  /* METODOS */
  Tipo tipo() const { return tipo_; }
  Numero numero() const { return numero_; }
  const Usuario* titular() const{ return titular_; }
  Fecha caducidad() const{ return caducidad_; }
  Cadena titular_facial() const{ return titular_facial_; }

  void anula_titular(){ titular_= nullptr; }
  /* DESTRUCTOR */
  ~Tarjeta();
  /* Clase Execpcion Tarjeta */
  class Caducada
  {
  public:
      Caducada(const Fecha& caducada) : caducada_(caducada) {}
      Fecha cuando() const { return caducada_;}
  private:
      Fecha caducada_;
  };
private:
  Tipo tipo_;
  Numero numero_;
  const Usuario* titular_;
  Fecha caducidad_;
  Cadena titular_facial_;
};
/* OPERADORES*/
bool operator< (const Tarjeta& card,const Tarjeta& card2);
std::ostream& operator <<(std::ostream& os,const Tarjeta::Tipo&  tipo);
std::ostream& operator <<(std::ostream& os,const Tarjeta& card);
#endif
