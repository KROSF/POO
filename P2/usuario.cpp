extern "C"{
  #include <unistd.h>
}
#include <cstring>
#include <random>
#include <iomanip>
#include "usuario.hpp"
#define  CARACTERESVALIDOS "zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA9876543210/."
/* Clase Clave */
/* Inicio constructor Clave */
Clave::Clave(const char * pass)
{
  if(std::strlen(pass) < 5) throw Incorrecta(Clave::CORTA);
  static std::random_device rd;
  static std::uniform_int_distribution<std::size_t> dis(0, 63);
  static const char *const cv = CARACTERESVALIDOS;
  const char salt[] = { cv[dis(rd)], cv[dis(rd)] };
  if(const char* const encrypt = crypt(pass, salt)) password = encrypt;
  else throw Incorrecta(Razon::ERROR_CRYPT);
}
/* Fin Constructor Clave */
/* METODOS */
inline const Cadena& Clave::clave() const { return password; }
bool Clave::verifica(const char* pass) const
{
  if(const char* const pw = crypt(pass, password.c_str()))
    return  pw == password;
  throw Incorrecta(Razon::ERROR_CRYPT);
}

/* Clase Usuario */
Usuario::Usuarios Usuario::usuarios_;
/* Inicio construcutor Usuario */
Usuario::Usuario(const Cadena& id,
                 const Cadena& nombre,
                 const Cadena& apellidos,
                 const Cadena& dir,
                 const Clave& pass):
                 identificador_(id),
                 nombre_(nombre),
                 apellidos_(apellidos),
                 direccion_(dir),
                 password_(pass)

{ if (!usuarios_.insert(id).second) throw Id_duplicado(identificador_); }
/* Fin constructor */
void Usuario::es_titular_de(Tarjeta& card)
{ if(this == card.titular()) cards_.insert(std::make_pair(card.numero(),&card));}

void Usuario::no_es_titular_de(Tarjeta& card)
{
  card.anula_titular();
  cards_.erase(card.numero());
}

void Usuario::compra(Articulo&  artcl, unsigned cantidad)
{
    if (cantidad == 0) artcls_.erase(&artcl);
    //eliminar el ariculos si cantidad es 0
    else artcls_[&artcl] = cantidad;
    //si no agregar el articulo
}

/* DESTRUCTOR */

Usuario::~Usuario()
{
  auto iter = cards_.begin();
  while(iter != cards_.end()) {
    iter->second->anula_titular();
    iter++;
  }
  usuarios_.erase(identificador_);
}

/* OPERADOR FLUJO */

std::ostream& operator << (std::ostream&os,const Usuario& user)
{
  os << user.identificador_ << "[" << user.password_.clave().c_str() << "]" << user.nombre_
      << user.apellidos_ << std::endl << user.direccion_ << std::endl << "Tarjetas:" << std::endl;
  auto iter = user.tarjetas().begin();
   while(iter != user.tarjetas().end()) {
     os << *iter->second << std::endl;
     iter++;
   }
  return os;
}
std::ostream& mostrar_carro(std::ostream& os, const Usuario& user)
{
  os << "Carrito de compra de " << user.id() << " [Artículos: "
     << user.n_articulos() << "]" << std::endl;
     os << " Cant. Artículo" << std::endl
        << std::setw(95) << std::setfill('=') << '\n'  << std::setfill(' ');
  int tmp = user.n_articulos();
  while(tmp > 0)
  {
    for (auto const& i : user.compra())
        {
          os << std::setw(4) << i.second << "   "
             << "[" << (*i.first).referencia() << "] \""
             << (*i.first).titulo() << "\", " << (*i.first).f_publi().anno()
             << ". " << std::fixed << std::setprecision(2) << (*i.first).precio()
             << " €" << std::endl;
          --tmp;
        }
  }
  return os;
}
