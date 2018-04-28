/**
* @file articulo.cpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
* @brief  Definicion de constructores y metodos de la clase Articulo.
*/
#include <iomanip>
#include "articulo.hpp"

/* Constructor Articulo */
Articulo::Articulo( const Cadena& referencia,
                    const Cadena& titulo,
                    const Fecha& publicacion,
                    double precio,
                    unsigned num):
                    cod_ref_(referencia),
                    titulo_(titulo),
                    publicacion_(publicacion),
                    precio_(precio),
                    num_artcls(num) {}

/* Metodos */

inline Cadena Articulo::referencia() const { return cod_ref_; }

inline Cadena Articulo::titulo() const {return titulo_; }

inline Fecha Articulo::f_publi() const { return publicacion_; }

inline double Articulo::precio() const { return precio_; }

double& Articulo::precio() {return precio_;}

inline unsigned Articulo::stock() const { return num_artcls; }

unsigned& Articulo::stock(){ return num_artcls; }

/* Inserccion en el flujo */

std::ostream& operator <<(std::ostream& os,const Articulo& artcl)
{
   os << "[" << artcl.referencia() << "] \""
      << artcl.titulo() << "\", " << artcl.f_publi().anno()
      << ". " << std::fixed << std::setprecision(2) << artcl.precio()
      << " €" << std::endl;
  return os;
}
