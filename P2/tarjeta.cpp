#include <algorithm>
#include <cctype>
#include <iomanip>
#include "tarjeta.hpp"
#define REMOVE std::remove_if(num.begin(),num.end(),[](unsigned char x){return std::isspace(x);})
#define COUNT std::count_if(num.begin(), num.end(), static_cast<int(*)(int)>(std::isdigit))
//Declaracion anticipada de luhn
bool luhn(const Cadena& numero);
/*******************************  NUMERO **********************************/
//> CONSTRUCTOR
Numero::Numero(Cadena num)
{
  if (num.length() == 0) throw Incorrecto(Razon::LONGITUD);
  num = num.substr(0,REMOVE - num.begin());
  if(COUNT != num.length()) throw Incorrecto(Razon::DIGITOS);
  if(num.length()< 13 || num.length() > 19) throw Incorrecto(Razon::LONGITUD);
  if(!luhn(num))throw Incorrecto(Razon::NO_VALIDO);
  num_ = num;
}
//> OPERADOR
bool operator< (const Numero& num,const Numero& num2)
{return num.num_ < num2.num_;}
/******************************** TARJETA ********************************/

Tarjeta::Tarjeta(Tipo t,const Numero& n,Usuario& u,const Fecha& fecha):
tipo_(t),numero_(n),titular_(&u),caducidad_(fecha),titular_facial_(u.nombre()+" "+u.apellidos())
{
  if(caducidad_ < Fecha() )throw Caducada(caducidad_);
  u.es_titular_de(*this);
}

Tarjeta::~Tarjeta()
{
  if(Usuario* us = const_cast<Usuario*>(titular_))
      us->no_es_titular_de(*this);
}

//> OPERADOR
bool operator< (const Tarjeta& card,const Tarjeta& card2)
{return card.numero()< card2.numero();}

std::ostream& operator <<(std::ostream& os,const Tarjeta::Tipo&  tipo)
{
  switch (tipo) {
    case Tarjeta::Tipo::VISA:	os << " VISA ";break;
		case Tarjeta::Tipo::Mastercard: os << " Mastercard ";break;
		case Tarjeta::Tipo::Maestro: os << " Maestro ";break;
		case Tarjeta::Tipo::JCB: os << " JCB ";break;
		case Tarjeta::Tipo::AmericanExpress: os << " AmericanExpress ";break;
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
