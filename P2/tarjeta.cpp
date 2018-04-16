#include "tarjeta.hpp"
#include <algorithm>
#include <cctype>
#define REMOVE std::remove_if(num.begin(),num.end(),[](unsigned char x){return std::isspace(x);})
#define FIND std::find_if(num.begin(),num.end(),(int(*)(int))std::isdigit)

bool luhn(const Cadena& num, size_t n);
/*******************************  NUMERO **********************************/
//> CONSTRUCTOR
Numero::Numero(Cadena num)
{
  if (!num.length()) throw Incorrecto(Razon::LONGITUD);
  num = num.substr(0,REMOVE - num.begin());
  if(FIND != num.end()) throw Incorrecto(Razon::DIGITOS);
  if(num.length()< 13 || num.length() > 19) throw Incorrecto(Razon::LONGITUD);
  if(!luhn(num,num.length())) throw Incorrecto(Razon::NO_VALIDO);
  num_ = num;
}
//> OPERADOR
bool operator< (const Numero& num,const Numero& num2)
{return num.num_ < num2.num_;}
/******************************** TARJETA ********************************/

Tarjeta::Tarjeta(Tipo tipo,const Numero& num,Usuario& titular,const Fecha& fecha):
tipo_(tipo),numero_(num),titular_(nullptr)
{
  if(fecha < caducidad_ )throw Caducada(fecha);
}

Tarjeta::~Tarjeta()
{ if (titular_) titular_->no_es_titular_de(*this); }

/* METODOS */

void Tarjeta::anula_titular() { const_cast<Usuario*&>(titular_) = nullptr; }
const Tarjeta::Tipo& Tarjeta::tipo() const { return tipo_; }
const Numero& Tarjeta::numero() const { return numero_; }
Usuario* Tarjeta::titular() const { return titular_; }
const Fecha& Tarjeta::caducidad() const { return caducidad_; }
const Cadena& Tarjeta::titular_facial() const { return titular_facial_; }

//> OPERADOR
bool operator< (const Tarjeta& card,const Tarjeta& card2)
{return card.numero()< card2.numero();}

std::ostream& operator <<(std::ostream& os,const Tarjeta::Tipo& tipo)
{
  switch (static_cast<int>(tipo)) {
    case 0:	os << " VISA ";
				break;
		case 1: os << " Mastercard ";
				break;
		case 2: os << " Maestro ";
				break;
		case 3: os << " JCB	";
				break;
		case 4: os << " AmericanExpress ";
				break;
  }
  return os;
}
std::ostream& operator <<(std::ostream& os,const Tarjeta& card)
{
  return os;
}
