/**
* @file tarjeta.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#ifndef TARJETA_HPP
#define TARJETA_HPP
#include <ostream>
#include <functional>
#include <cctype>
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
#include "usuario.hpp"

class Numero
{
  public:
    /* Razones De Execpcion */
    enum Razon
    {
        LONGITUD,
        DIGITOS,
        NO_VALIDO
    };
    /* CONSTRUCTOR */
    Numero(Cadena);
    /* Operador de Conversion a cadena de caracteres */
    operator const char *() const;
    /* Amigos de la clase */
    friend bool operator<(const Numero &num, const Numero &num2);
    /* Funcion unaria con operador objeto*/
    struct EsDigito : public std::unary_function<const unsigned char, bool>
    {
        bool operator()(const unsigned char &x) const { return std::isdigit(x); }
    };

    /* Clase Execpcion Numero */
    class Incorrecto
    {
      public:
        Incorrecto(Razon rzn);
        Razon razon() const;

      private:
        Razon rzn_;
    };

  private:
    Cadena num_;
};

//Declaracion anticipada.
class Usuario;

class Tarjeta
{
  public:
    /* Tipo de Tarjetas */
    enum Tipo
    {
        VISA,
        Mastercard,
        Maestro,
        JCB,
        AmericanExpress
    };
    /* CONSTRUCTOR */
    Tarjeta(Tipo t, const Numero &n, Usuario &u, const Fecha &f);
    /* No se puede copiar una tarjeta */
    Tarjeta(const Tarjeta &) = delete;
    Tarjeta &operator=(const Tarjeta &) = delete;
    /* METODOS */
    Tipo tipo() const;
    Numero numero() const;
    const Usuario *titular() const;
    Fecha caducidad() const;
    Cadena titular_facial() const;
    void anula_titular();
    /* DESTRUCTOR */
    ~Tarjeta();
    /* Clase Execpcion Tarjeta */

    class Caducada
    {
      public:
        Caducada(const Fecha &caducada);
        Fecha cuando() const;

      private:
        Fecha caducada_;
    };

  private:
    Tipo tipo_;
    Numero numero_;
    const Usuario *titular_;
    Fecha caducidad_;
    Cadena titular_facial_;
};
/* OPERADORES*/
bool operator<(const Tarjeta &card, const Tarjeta &card2);
std::ostream &operator<<(std::ostream &os, const Tarjeta::Tipo &tipo);
std::ostream &operator<<(std::ostream &os, const Tarjeta &card);
#endif
