/**
* @file tarjeta.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
* @brief Declaracion de constructores y metodos de las clases Numero y Tarjeta.
*/
#ifndef TARJETA_HPP
#define TARJETA_HPP
#include <ostream>
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
#include "usuario.hpp"
//> Macro quitar espacios cadena.
#define REMOVE std::remove_if(num.begin(),num.end(),[](unsigned char x){return std::isspace(x);})
//> Macro contar digitos en cadena.
#define FIND std::find_if(num.begin(), num.end(),[](unsigned char x){return !std::isdigit(x);})
/*******************************  NUMERO **********************************/
class Numero{
public:
  enum Razon {LONGITUD,DIGITOS,NO_VALIDO};
  Numero(Cadena);
  operator const char*() const;
  friend bool operator < (const Numero& num,const Numero& num2);
/* Clase Execpcion Numero */
  class Incorrecto{
  public:
    Incorrecto(Razon rzn);
    Razon razon() const;
  private:
    Razon rzn_;
  };
private:
  Cadena num_;
};

//Declaracion anticipada clase usuario.
class Usuario;

/******************************** TARJETA ********************************/

class Tarjeta{
public:
  enum Tipo {VISA,Mastercard, Maestro, JCB, AmericanExpress};
  /* CONSTRUCTOR */
  Tarjeta(Tipo t,const Numero& n,Usuario& u,const Fecha& f);
  /* Delete */
  Tarjeta(const Tarjeta& ) = delete;
  Tarjeta& operator= (const Tarjeta &) = delete;
  /* METODOS */
  Tipo tipo() const;
  Numero numero() const;
  const Usuario* titular() const;
  Fecha caducidad() const;
  Cadena titular_facial() const;
  void anula_titular();
  /* DESTRUCTOR */
  ~Tarjeta();
  /* Clase Execpcion Tarjeta */
  class Caducada
  {
  public:
      Caducada(const Fecha& caducada);
      Fecha cuando() const;
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
