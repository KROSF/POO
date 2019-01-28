/**
* @file usuario.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#ifndef USUARIOS_HPP
#define USUARIOS_HPP
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "../P1/cadena.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
class Numero;
class Tarjeta;
/***************************** Clase Clave ************************************/
class Clave
{
  public:
    /* enum */
    enum Razon
    {
        CORTA,
        ERROR_CRYPT
    };
    /* Constructor */
    Clave(const char *);
    /* Metodos */
    const Cadena &clave() const;
    bool verifica(const char *validate) const;
    /* Clase Execpcion */
    class Incorrecta
    {
      public:
        Incorrecta(Razon rzn);
        Razon razon() const;

      private:
        Razon rzn_;
    };

  private:
    Cadena password;
};

/***************************** Clase Usuario **********************************/

class Usuario
{
  public:
    /* Definicion de tipos */
    typedef std::map<Numero, Tarjeta *> Tarjetas;
    typedef std::unordered_map<Articulo *, unsigned> Articulos;
    typedef std::unordered_set<Cadena> Usuarios;
    /* Constructor */
    Usuario(const Cadena &, const Cadena &,
            const Cadena &, const Cadena &,
            const Clave &);
    /* No se permite la copia de Usuario */
    Usuario(const Usuario &) = delete;
    Usuario &operator=(const Usuario &) = delete;

    /* Metodos */

    void es_titular_de(Tarjeta &);           //Vincular una tarjeta con Usuario
    void no_es_titular_de(Tarjeta &);        //Desvicular tarjeta de usuario
    void compra(Articulo &, unsigned s = 1); //Agregar un articulo

    /* Observadores */

    Cadena id() const { return identificador_; }
    Cadena nombre() const { return nombre_; }
    Cadena apellidos() const { return apellidos_; }
    Cadena direccion() const { return direccion_; }
    size_t n_articulos() const { return artcls_.size(); }
    const Tarjetas &tarjetas() const { return cards_; }
    const Articulos &compra() const { return artcls_; }

    /* Amigos de la clase */
    friend std::ostream &operator<<(std::ostream &os, const Usuario &user);

    /* Destructor*/
    ~Usuario();

    /* Clase Execpcion */
    class Id_duplicado
    {
      public:
        Id_duplicado(const Cadena &duplicate) : duplicate_(duplicate) {}
        const Cadena &idd() const { return duplicate_; }

      private:
        Cadena duplicate_;
    };

  private:
    Cadena identificador_, nombre_, apellidos_, direccion_;
    Clave password_;
    Tarjetas cards_;
    Articulos artcls_;
    static Usuarios usuarios_;
};
/* Operador de flujo */
std::ostream &mostrar_carro(std::ostream &os, const Usuario &user);
#endif
