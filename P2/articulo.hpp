#ifndef ARTICULO_HPP
#define ARTICULO_HPP
#include "../P1/fecha.hpp"
#include "../P1/cadena.hpp"
class Articulo{
public:
  explicit Articulo(const Cadena&,const Cadena&,const Fecha&,double,unsigned);
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
std::ostream& operator << (std::ostream& os,const Articulo& artcl);
#endif
