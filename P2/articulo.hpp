#ifndef ARTICULO_HPP
#define ARTICULO_HPP
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
class Articulo{
public:
  Articulo(const Cadena&,const Cadena&,const Fecha&,unsigned,double);
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
  unsigned num_artcls;
  double precio_;
};
std::ostream& operator << (std::ostream& os,const Articulo& artcl);
#endif
