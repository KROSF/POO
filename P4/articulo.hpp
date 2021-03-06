/**
 * @file articulo.hpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 27 Apr 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
#ifndef ARTICULO_HPP
#define ARTICULO_HPP
#include <map>
#include <set>
#include "cadena.hpp"
#include "fecha.hpp"

class Autor {
 public:
  Autor(const Cadena &, const Cadena &, const Cadena &);
  const Cadena &nombre() const noexcept { return nom_; }
  const Cadena &apellidos() const noexcept { return ape_; }
  const Cadena &direccion() const noexcept { return dir_; }

 private:
  Cadena nom_, ape_, dir_;
};

class Articulo {
 public:
  using Autores = std::set<Autor *>;

  class Autores_vacios {};

  Articulo(Autores, const Cadena &, const Cadena &, const Fecha &, double);

  const Autores &autores() const;
  const Cadena &referencia() const;
  const Cadena &titulo() const;
  const Fecha &f_publi() const;
  double precio() const;
  double &precio();

  virtual void impresion_especifica(std::ostream &os) const noexcept = 0;
  virtual ~Articulo() {}

 private:
  const Autores autores_;
  Cadena cod_ref_, titulo_;
  Fecha publicacion_;
  double precio_;
};

std::ostream &operator<<(std::ostream &os, const Articulo &art) noexcept;

class ArticuloAlmacenable : public Articulo {
 public:
  ArticuloAlmacenable(Autores, const Cadena &, const Cadena &, const Fecha &,
                      double, unsigned s = 0);
  unsigned stock() const { return stock_; }
  unsigned &stock() { return stock_; }
  virtual ~ArticuloAlmacenable() {}

 protected:
  unsigned stock_;
};

class Libro : public ArticuloAlmacenable {
 public:
  Libro(Autores, const Cadena &, const Cadena &, const Fecha &, double,
        unsigned, unsigned s = 0);
  unsigned n_pag() const { return n_pag_; }
  virtual void impresion_especifica(std::ostream &os) const noexcept {
    os << n_pag_ << " págs., " << stock_ << " unidades.";
  }

 private:
  const unsigned n_pag_;
};

class LibroDigital : public Articulo {
 public:
  LibroDigital(Autores, const Cadena &, const Cadena &, const Fecha &, double,
               const Fecha &);
  const Fecha &f_expir() const { return f_expir_; }
  virtual void impresion_especifica(std::ostream &os) const noexcept {
    os << "A la venta hasta el " << f_expir_ << '.';
  }

 private:
  Fecha f_expir_;
};

class Cederron : public ArticuloAlmacenable {
 public:
  Cederron(Autores, const Cadena &, const Cadena &, const Fecha &, double,
           unsigned, unsigned s = 0);

  unsigned tam() const { return tam_; }
  virtual void impresion_especifica(std::ostream &os) const noexcept {
    os << tam_ << " MB, " << stock_ << " unidades.";
  }

 private:
  const unsigned tam_;
};
#endif
