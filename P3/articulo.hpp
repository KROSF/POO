/**
* @file articulo.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
* @brief Declaracion de constructores y metodos de la clase Articulo.
*/
#ifndef ARTICULO_HPP
#define ARTICULO_HPP
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
class Articulo{
public:
  /* Constructor */
  explicit Articulo(const Cadena&,const Cadena&,const Fecha&,double,unsigned);
  /* Metodos */
  Cadena referencia() const;
  Cadena titulo() const;
  Fecha f_publi() const;
  double precio() const;
  double& precio();
  unsigned stock() const;
  unsigned& stock();
private:
  Cadena cod_ref_,titulo_;
  Fecha publicacion_;
  double precio_;
  unsigned num_artcls;
};
std::ostream& operator <<(std::ostream& os,const Articulo& artcl);
#endif
