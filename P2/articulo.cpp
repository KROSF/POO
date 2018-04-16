#include "articulo.hpp"
#include <iomanip>

Articulo::Articulo(const Cadena& referencia,const Cadena& titulo,const Fecha& publicacion,double precio,unsigned num):
cod_ref_(referencia),titulo_(titulo),publicacion_(publicacion),precio_(precio),num_artcls(num){}

inline Cadena Articulo::referencia() const { return cod_ref_; }
inline Cadena Articulo::titulo() const {return titulo_; }
inline Fecha Articulo::f_publi() const { return publicacion_; }
inline double Articulo::precio() const { return precio_; }
inline double& Articulo::precio() {return precio_;}
inline unsigned Articulo::stock() const { return num_artcls; }
inline unsigned& Articulo::stock(){ return num_artcls; }

std::ostream& operator <<(std::ostream& os,const Articulo& artcl)
{
  os<<'['<<artcl.referencia()<<"] \""
    <<artcl.titulo()<<"\", "<<artcl.f_publi().anno()<<'.'
    <<std::put_money(artcl.precio())<<std::endl;
  return os;
}
