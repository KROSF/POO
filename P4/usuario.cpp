/**
 * @file usuario.cpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 27 Apr 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
extern "C" {
#include <unistd.h>
}
#include <cstring>
#include <iomanip>
#include <random>
#include "usuario.hpp"
/* define con los caracteres validos para el cifrado */
#define CHARS "zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA9876543210/."
/***************************** Clase Clave ************************************/

//> Constructor.

Clave::Clave(const char *pass) {
  if (std::strlen(pass) < 5) throw Incorrecta(Clave::CORTA);

  static std::random_device rd;
  static std::uniform_int_distribution<std::size_t> dis(0, 63);
  static const char *const cv = CHARS;  // CHARS en linea 16
  const char salt[] = {cv[dis(rd)], cv[dis(rd)]};
  if (const char *const encrypt = crypt(pass, salt))
    password = encrypt;
  else
    throw Incorrecta(Razon::ERROR_CRYPT);
}

//> Metodos
inline const Cadena &Clave::clave() const { return password; }

bool Clave::verifica(const char *pass) const {
  if (const char *const pw = crypt(pass, password.c_str()))
    return pw == password;
  throw Incorrecta(Razon::ERROR_CRYPT);
}

//> Incorrecta.

Clave::Incorrecta::Incorrecta(Razon rzn) : rzn_(rzn) {}

Clave::Razon Clave::Incorrecta::razon() const { return rzn_; }

/***************************** Clase Usuario **********************************/

//> Inicializacion usuarios_
Usuario::Usuarios Usuario::usuarios_;

//> Constructor.
Usuario::Usuario(const Cadena &id, const Cadena &nombre,
                 const Cadena &apellidos, const Cadena &dir, const Clave &pass)
    : identificador_(id),
      nombre_(nombre),
      apellidos_(apellidos),
      direccion_(dir),
      password_(pass)

{
  if (!usuarios_.insert(id).second) throw Id_duplicado(identificador_);
}

//> Metodos.

void Usuario::es_titular_de(Tarjeta &card) {
  if (this == card.titular())
    cards_.insert(std::make_pair(card.numero(), &card));
}

void Usuario::no_es_titular_de(Tarjeta &card) {
  card.anula_titular();
  cards_.erase(card.numero());
}

void Usuario::compra(Articulo &artcl, unsigned cantidad) {
  if (cantidad == 0) artcls_.erase(&artcl);
  // eliminar el ariculos si cantidad es 0
  else
    artcls_[&artcl] = cantidad;
  // si no agregar el articulo
}

//> Destructor.

Usuario::~Usuario() {
  for (auto &[numero, tarjeta] : cards_) tarjeta->anula_titular();
  usuarios_.erase(identificador_);
}

//> Operadores de flujo

std::ostream &operator<<(std::ostream &os, const Usuario &user) {
  os << user.identificador_ << "[" << user.password_.clave().c_str() << "]"
     << user.nombre_ << user.apellidos_ << std::endl
     << user.direccion_ << std::endl
     << "Tarjetas:" << std::endl;
  for (const auto &[numero, tarjeta] : user.tarjetas())
    os << *tarjeta << std::endl;
  return os;
}

std::ostream &mostrar_carro(std::ostream &os, const Usuario &user) {
  os << "Carrito de compra de " << user.id()
     << " [Artículos: " << user.n_articulos() << "]" << std::endl
     << " Cant. Artículo" << std::endl
     << Cadena(80, '=') << std::endl;
  for (const auto &[articulo, cantidad] : user.compra()) {
    os << std::setw(4) << cantidad << "   "
       << "[" << articulo->referencia() << "] \"" << articulo->titulo()
       << "\", " << articulo->f_publi().anno() << ". " << std::fixed
       << std::setprecision(2) << articulo->precio() << " €" << std::endl;
  }
  return os;
}
