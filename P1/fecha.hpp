/**
 * @file fecha.hpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 27 Apr 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 * @brief Declaraciones clase Fecha.
 */

#ifndef FECHA_H
#define FECHA_H
#include <clocale>
#include <ctime>
#include <iostream>

/* CLASE PARA OBTENER LA HORA DEL SISTEMA */
class Hrsys {
 public:
  // La hora del sistema que se obtiene es utc.
  Hrsys() : t1(std::time(nullptr)), t2(new std::tm(*std::gmtime(&t1))) {
    std::setlocale(LC_TIME, "es_ES.UTF-8");
  }
  // Constructor de conversión de d,m,a,n a hora del sistema.
  // n numero de dias a sumar a la fecha.
  Hrsys(int d, int m, int a, int n = 0)
      : t1(0), t2(new std::tm{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {
    t2->tm_mday = (d + n);
    t2->tm_mon = m - 1;
    t2->tm_year = a - 1900;
    std::mktime(t2);
  }
  // Observadores
  int dia() const noexcept { return t2->tm_mday; }
  int mes() const noexcept { return t2->tm_mon + 1; }
  int anio() const noexcept { return t2->tm_year + 1900; }

  // Convierte una hora del sistema a cadena de caracteres.
  const char *toString() const noexcept {
    static char s[40];
    std::strftime(s, 40, "%A %d de %B de %Y", t2);
    return s;
  }

  // Destructor
  ~Hrsys() { delete t2; }

 private:
  std::time_t t1;
  std::tm *t2;
};

class Fecha {
 public:
  /* EXEPCIONES */
  class Invalida {
   public:
    explicit Invalida(const char *msg) : _msg(msg) {}
    const char *por_que() const { return _msg; }

   private:
    const char *_msg;
  };
  /* CONSTANTES */
  static const int AnnoMinimo = 1902;
  static const int AnnoMaximo = 2037;
  /* CONSTRUCTORES */
  explicit Fecha(int d = 0, int m = 0, int a = 0);
  Fecha(const char *);
  Fecha(const Fecha &) = default;
  /* METODOS */
  int dia() const noexcept;
  int mes() const noexcept;
  int anno() const noexcept;
  const char *cadena() const;
  /* OPERADORES */
  Fecha &operator=(const Fecha &) = default;
  Fecha &operator+=(int);
  Fecha &operator-=(int);
  Fecha operator+(int) const;
  Fecha operator-(int) const;
  Fecha &operator++();    // prefijo
  Fecha operator++(int);  // posfijo
  Fecha &operator--();
  Fecha operator--(int);
  ~Fecha() = default;

 private:
  int _d, _m, _a;
  void es_valida() const;
};
/* OPERADORES DE COMPARACIÓN */
bool operator==(const Fecha &, const Fecha &) noexcept;
bool operator!=(const Fecha &, const Fecha &) noexcept;
bool operator<(const Fecha &, const Fecha &) noexcept;
bool operator>(const Fecha &, const Fecha &) noexcept;
bool operator<=(const Fecha &, const Fecha &) noexcept;
bool operator>=(const Fecha &, const Fecha &) noexcept;
/* OPERADORES DE FLUJO */
std::istream &operator>>(std::istream &is, Fecha &f);
std::ostream &operator<<(std::ostream &os, const Fecha &f) noexcept;
#endif
