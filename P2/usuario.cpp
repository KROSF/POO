#include <cstring>
#include <random>
#include <unistd.h>
#include <iomanip>
#include "usuario.hpp"
#define MD5 "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
/* Clase Clave */
Clave::Clave(const char * pass)
{
  if(std::strlen(pass) < 5) throw Incorrecta(Clave::CORTA);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 63);
  char const md5[] = MD5;
  char salt[2] = { md5[dis(rd)], md5[dis(rd)] };
  char* encrypt = crypt(pass, salt);
  if(encrypt == nullptr) throw Incorrecta(Razon::ERROR_CRYPT);
	password = encrypt;
}
/* METODOS */
inline const Cadena& Clave::clave() const{return password;}
inline bool Clave::verifica(const char* pass) const
{ return std::strcmp(crypt(pass, password.c_str()), password.c_str()) == 0 ; }

/* Clase Usuario */

Usuario::Usuario(const Cadena& idtfdr,const Cadena& nombre,const Cadena& apellido,
                 const Cadena& dir,const Clave& pass):identificador_(idtfdr),
                 nombre_(nombre),apellido_(apellido),direccion_(dir),password_(pass)
{if (!usuarios_.insert(idtfdr).second) throw Id_duplicado(identificador_);}

/* METODOS */

inline const Cadena& Usuario::id() const{return identificador_;}
inline const Cadena& Usuario::nombre() const{return nombre_;}
inline const Cadena& Usuario::apellidos() const{return apellido_;}
inline const Cadena& Usuario::direccion() const{return direccion_;}
inline const Usuario::Tarjetas& Usuario::tarjetas() const{return cards_;}
inline const Usuario::Articulos& Usuario::compra() const{return artcls_;}
inline size_t Usuario::n_articulos() const{return artcls_.size();}

void Usuario::es_titular_de(const Tarjeta& card)
{ if(card.titular() == this) cards_[card.numero()] = const_cast<Tarjeta*>(&card);}

void Usuario::no_es_titular_de(Tarjeta& card)
{ cards_.erase(card.numero()); }

void Usuario::compra(Articulo&  artcl, unsigned cantidad)
{
    if (cantidad == 0) artcls_.erase(const_cast<Articulo*>(&artcl));
    else artcls_[const_cast<Articulo*>(&artcl)] = cantidad;
}

/* DESTRUCTOR */

Usuario::~Usuario()
{
  for (auto& card : cards_) card.second->anula_titular();
  usuarios_.erase(identificador_);
}

/* OPERADOR FLUJO */

std::ostream& operator << (std::ostream&os,const Usuario& user)
{
  os << user.identificador_ << "[" << user.password_.clave() << "]" << user.nombre_
      << user.apellido_ << std::endl << user.direccion_ << std::endl << "Tarjetas:" << std::endl;
   for (auto& card : user.cards_) os << *card.second << std::endl;
  return os;
}

void mostrar_carro(std::ostream& os, const Usuario& user)
{
  os << "Carrito de compra de " << user.id() << " [Artículos: "
     << user.n_articulos() << "]" << std::endl;
  while(user.n_articulos())
  {
    os << " Cant. Artículo" << std::endl
       << std::setw(95) << std::setfill('=') << '\n'  << std::setfill(' ');

    for (auto const& i : user.compra())
        os << std::setw(4) << i.second << "   "<< *i.first << std::endl;

    os << std::endl;
  }
}
