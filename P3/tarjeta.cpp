/**
* @file tarjeta.cpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
* @brief Definicion de constructores y metodos de las clases Numero y Tarjeta.
*/
#include <algorithm>
#include <iomanip>
#include "tarjeta.hpp"
//> Declaracion anticipada de luhn.
bool luhn(const Cadena& numero);
/*******************************  NUMERO **********************************/
//> Cosntructor.
Numero::Numero(Cadena num)
{
  if (num.length() == 0) throw Incorrecto(Razon::LONGITUD);
  num = num.substr(0,REMOVE - num.begin());
  if(FIND != num.end()) throw Incorrecto(Razon::DIGITOS);
  if(num.length()< 13 || num.length() > 19) throw Incorrecto(Razon::LONGITUD);
  if(!luhn(num))throw Incorrecto(Razon::NO_VALIDO);
  num_ = num;
}

//> Operadores.

inline Numero::operator const char*() const {return num_.c_str();}

bool operator< (const Numero& num,const Numero& num2){return num.num_ < num2.num_;}

//> Incorrecto.

Numero::Incorrecto::Incorrecto(Razon rzn):rzn_(rzn){}
Numero::Razon Numero::Incorrecto::razon() const {return rzn_;}

/******************************** TARJETA ********************************/

//> Constructor.

Tarjeta::Tarjeta(Tipo t,const Numero& n,Usuario& u,const Fecha& fecha):
tipo_(t),numero_(n),titular_(&u),caducidad_(fecha),titular_facial_(u.nombre()+" "+u.apellidos())
{
  if(Fecha() > caducidad_ )throw Caducada(caducidad_);
  u.es_titular_de(*this);
}

//> Observadores

inline Tarjeta::Tipo Tarjeta::tipo() const { return tipo_; }
inline Numero Tarjeta::numero() const { return numero_; }
const Usuario* Tarjeta::titular() const{ return titular_; }
inline Fecha Tarjeta::caducidad() const{ return caducidad_; }
inline Cadena Tarjeta::titular_facial() const{ return titular_facial_; }

//> Relacion Tarjeta

void Tarjeta::anula_titular(){ titular_= nullptr; }

//> Destructor.
Tarjeta::~Tarjeta()
{
  if(Usuario* us = const_cast<Usuario*>(titular_))
      us->no_es_titular_de(*this);
}

//> Caducada.

Tarjeta::Caducada::Caducada(const Fecha& caducada) : caducada_(caducada) {}

Fecha Tarjeta::Caducada::cuando() const { return caducada_;}

//> Operadores.

bool operator< (const Tarjeta& card,const Tarjeta& card2)
{return card.numero()< card2.numero();}

std::ostream& operator <<(std::ostream& os,const Tarjeta::Tipo&  tipo)
{
  switch (tipo) {
    case Tarjeta::Tipo::VISA:	os << "VISA";break;
		case Tarjeta::Tipo::Mastercard: os << "Mastercard";break;
		case Tarjeta::Tipo::Maestro: os << "Maestro";break;
		case Tarjeta::Tipo::JCB: os << "JCB";break;
		case Tarjeta::Tipo::AmericanExpress: os << "AmericanExpress";break;
  }
  return os;
}
std::ostream& operator <<(std::ostream& os,const Tarjeta& card)
{
  os<<card.tipo()<<std::endl;
  os<<card.numero()<<std::endl;
  os<<card.titular_facial()<<std::endl;
  os<<"Caduca: "<< std::setfill('0') << std::setw(2) << card.caducidad().mes()
     << '/' << std::setw(2) << (card.caducidad().anno() % 100);
  return os;
}
