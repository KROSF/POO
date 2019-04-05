/**
 * @file tarjeta.cpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 27 Apr 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
#include "tarjeta.hpp"
#include <algorithm>
#include <iomanip>
// Macros para evitar lineas muy largas
// Quitar caracteres espaciadores de la cadena num.
#define ULTIMA                           \
  std::remove_if(num.begin(), num.end(), \
                 [](unsigned char x) { return std::isspace(x); })
// Ver si un caracteres es de tipo no numerico.
#define FIN std::find_if(num.begin(), num.end(), std::not1(EsDigito()))
//> Declaracion anticipada de luhn.
bool luhn(const Cadena &numero);

//> Constructor.
Numero::Numero(Cadena num) {
  if (num.length() == 0) throw Incorrecto(Razon::LONGITUD);

  // La funcion find if no quita el ultimo espacio
  // Lo quitamos con una subcadena.
  // ULTIMA Macro para quitar los espacios ver en linea 12-13
  num = num.substr(0, ULTIMA - num.begin());

  // FIN Macro que busca los caractres no numericos ver en linea 15
  if (FIN != num.end()) throw Incorrecto(Razon::DIGITOS);
  if (num.length() < 13 || num.length() > 19) throw Incorrecto(Razon::LONGITUD);
  if (!luhn(num)) throw Incorrecto(Razon::NO_VALIDO);
  // Asignamos la cadena cuado cumple todas las condiciones.
  num_ = num;
}

//> Operadores.

inline Numero::operator const char *() const { return num_.c_str(); }

bool operator<(const Numero &num, const Numero &num2) {
  return num.num_ < num2.num_;
}

//> Incorrecto.

Numero::Incorrecto::Incorrecto(Razon rzn) : rzn_(rzn) {}

Numero::Razon Numero::Incorrecto::razon() const { return rzn_; }

/******************************** TARJETA ********************************/

//> Constructor.

Tarjeta::Tarjeta(Tipo t, const Numero &n, Usuario &u, const Fecha &fecha)
    : tipo_(t),
      numero_(n),
      titular_(&u),
      caducidad_(fecha),
      titular_facial_(u.nombre() + " " + u.apellidos()) {
  if (Fecha() > caducidad_) throw Caducada(caducidad_);
  u.es_titular_de(*this);
}

//> Observadores

inline Tarjeta::Tipo Tarjeta::tipo() const { return tipo_; }
inline Numero Tarjeta::numero() const { return numero_; }
const Usuario *Tarjeta::titular() const { return titular_; }
inline Fecha Tarjeta::caducidad() const { return caducidad_; }
inline Cadena Tarjeta::titular_facial() const { return titular_facial_; }

//> Relacion Tarjeta

void Tarjeta::anula_titular() { titular_ = nullptr; }

//> Destructor.
Tarjeta::~Tarjeta() {
  if (Usuario *us = const_cast<Usuario *>(titular_))
    us->no_es_titular_de(*this);
}

//> Caducada.

Tarjeta::Caducada::Caducada(const Fecha &caducada) : caducada_(caducada) {}

Fecha Tarjeta::Caducada::cuando() const { return caducada_; }

//> Operadores.

bool operator<(const Tarjeta &card, const Tarjeta &card2) {
  return card.numero() < card2.numero();
}

std::ostream &operator<<(std::ostream &os, const Tarjeta::Tipo &tipo) {
  switch (tipo) {
    case Tarjeta::Tipo::VISA:
      os << "VISA";
      break;
    case Tarjeta::Tipo::Mastercard:
      os << "Mastercard";
      break;
    case Tarjeta::Tipo::Maestro:
      os << "Maestro";
      break;
    case Tarjeta::Tipo::JCB:
      os << "JCB";
      break;
    case Tarjeta::Tipo::AmericanExpress:
      os << "AmericanExpress";
      break;
  }
  return os;
}
std::ostream &operator<<(std::ostream &os, const Tarjeta &card) {
  return os << card.tipo() << std::endl
            << card.numero() << std::endl
            << card.titular_facial() << std::endl
            << "Caduca: " << std::setfill('0') << std::setw(2)
            << card.caducidad().mes() << '/' << std::setw(2)
            << (card.caducidad().anno() % 100);
}
