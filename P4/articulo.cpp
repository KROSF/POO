/**
* @file articulo.cpp
* @author Carlos Rodrigo Sanabria Flores
* @date 27 Apr 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
* @brief  Definicion de constructores y metodos de la clase Articulo.
*/
#include <iomanip>
#include "articulo.hpp"

/* Autor */

Autor::Autor(const Cadena& nom,const Cadena& ape,const Cadena& dir)
                    :nom_(nom),ape_(ape),dir_(dir){}

/* Articulo */
Articulo::Articulo(Autores autores,
                          const Cadena& ref,
                          const Cadena& title,
                          const Fecha& publi,double precio):
                          autores_(autores),cod_ref_(ref),titulo_(title),
                          publicacion_(publi),precio_(precio)
{
    if(autores_.empty()) throw Autores_vacios();
}

inline const Articulo::Autores& Articulo::autores() const { return autores_; }

inline const Cadena& Articulo::referencia() const { return cod_ref_; }

inline const Cadena& Articulo::titulo() const {return titulo_; }

inline const Fecha& Articulo::f_publi() const { return publicacion_; }

inline double Articulo::precio() const { return precio_; }

double& Articulo::precio() {return precio_;}


std::ostream& operator <<(std::ostream& os,const Articulo& artcl) noexcept
{
   os << "[" << artcl.referencia() << "] \""
    << artcl.titulo() << "\", de ";
    auto autor = artcl.autores().begin();
    os << (*autor)->apellidos();
    for(++autor; autor != artcl.autores().end(); ++autor)
        os << ", " << (*autor)->apellidos();
    os << ". ";
    os << artcl.f_publi().anno()<< ". " << std::fixed << std::setprecision(2)
    << artcl.precio() << " \u20AC"<<std::endl;
    artcl.impresion_especifica(os);
    return os;
}

/* Articulo Almacenable */
ArticuloAlmacenable::ArticuloAlmacenable(Autores autores,
                                                const Cadena& ref,
                                                const Cadena& title,
                                                const Fecha& publi,
                                                double precio,unsigned stock)
:Articulo(autores,ref,title,publi,precio),stock_(stock){}

/* Libro */

Libro::Libro(Autores autores,
                    const Cadena& ref,
                    const Cadena& title,
                    const Fecha& publi,
                    double precio,unsigned paginas,unsigned stock):
                    ArticuloAlmacenable(autores,ref,title,publi,precio,stock),
                    n_pag_(paginas){}

/* Libro Digital */

LibroDigital::LibroDigital(Autores autores,
                        const Cadena& ref,
                        const Cadena& title,
                        const Fecha& publi,double precio,const Fecha& exp):
                        Articulo(autores,ref,title,publi,precio),f_expir_(exp){}

/* Cederron */

Cederron::Cederron(Autores autores,const Cadena& ref,
                    const Cadena& title,
                    const Fecha& publi,
                    double precio,unsigned tam,unsigned stock):
                    ArticuloAlmacenable(autores,ref,title,publi,precio,stock),
                    tam_(tam){}
