#ifndef USUARIOS_HPP
#define  USUARIOS_HPP
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "../P1/cadena.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
/* Clase Clave */
class Clave{
public:
  enum Razon { CORTA,ERROR_CRYPT };
  /* CONSTRUCTOR */
  Clave(const char* );
  /* METODOS */
  const Cadena& clave() const;
  bool verifica(const char* validate) const;
  /* Clase Execpcion */
  class Incorrecta{
  public:
    Incorrecta(Razon rzn):rzn_(rzn){}
    Razon razon()const {return rzn_;}
  private:
    Razon rzn_;
  };
private:
  Cadena password;
};

/* Clase Usuario */
class Numero;
class Tarjeta;
class Usuario{
public:
  /* TIPOS */
  typedef std::map<Numero, Tarjeta*> Tarjetas;
  typedef std::unordered_map<Articulo*, unsigned> Articulos;
  typedef std::unordered_set<Cadena> Usuarios;
  /* CONSTRUCTORES */
  Usuario(const Cadena& idtfdr,const Cadena& nombre,
          const Cadena& apellido,const Cadena& dir,
          const Clave& pas);
  Usuario(const Usuario&) = delete;
  /* OPERADORES */
  Usuario& operator=(const Usuario&) = delete;
  /* METODOS */
  void es_titular_de(const Tarjeta &);
  void no_es_titular_de(Tarjeta &);
  void compra(Articulo&, unsigned s = 1);

  const Cadena& id() const;
  const Cadena& nombre() const;
  const Cadena& apellidos() const;
  const Cadena& direccion() const;
  const Tarjetas& tarjetas() const;
  const Articulos& compra() const;
  size_t n_articulos() const;

  friend std::ostream& operator << (std::ostream&os,const Usuario& user);

  /* Clase Execpcion */
  class Id_duplicado{
  public:
    Id_duplicado(const Cadena& duplicate) : duplicate_(duplicate) {}
    const Cadena& idd() const { return duplicate_; }
  private:
    Cadena duplicate_;
  };
~Usuario();
private:
  Cadena identificador_, nombre_, apellido_, direccion_;
  Clave password_;
  Tarjetas cards_;
  Articulos artcls_;
  static Usuarios usuarios_;
};
void mostrar_carro(std::ostream& os, const Usuario& user);
#endif
