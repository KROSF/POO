/**
* @file pedido-articulo.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 12 May 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#ifndef PEDIDO_ARTICULO_HPP
#define PEDIDO_ARTICULO_HPP
#include <ostream>
#include "pedido.hpp"
class LineaPedido{
public:

    /* Construtor */

    explicit LineaPedido(double precio, unsigned cantidad = 1);

    /* Observadores */

    double precio_venta(void) const {return precio_;}
    unsigned cantidad(void) const {return cantidad_;}
private:
    double precio_;
    unsigned cantidad_;
};

/* Operador de flujo pedido articulo*/
std::ostream& operator <<(std::ostream& os, const LineaPedido& li_pe);

/* Clase con opereador objeto para ordenar los pedidos*/
struct OrdenaPedidos: public std::binary_function<const Pedido*,const Pedido*,bool>{
    bool operator()(const Pedido* pp,const Pedido* sp) const;
};

/* Clase con opereador objeto para ordenar los articulos*/
struct OrdenaArticulos: public std::binary_function<const Articulo*,const Articulo*,bool>{
    bool operator()(const Articulo* pa,const Articulo* sa) const;
};

class Pedido_Articulo{
public:

    /* Definicion de tipos publicos */

    typedef std::map<Articulo*,LineaPedido,OrdenaArticulos> ItemsPedido;
    typedef std::map<Pedido*,LineaPedido,OrdenaPedidos> Pedidos;

    /* Asociaciones */

    void pedir(Pedido& pedido, Articulo& articulo, double precio, unsigned cantidad = 1);
    void pedir(Articulo& articulo,Pedido& pedido, double precio, unsigned cantidad = 1);

    /* Observadores de Asociaciones*/
    ItemsPedido& detalle(Pedido&);
	Pedidos ventas(Articulo&);

    /* Isercion en el flujo*/
	std::ostream& mostrarDetallePedidos(std::ostream&);
	std::ostream& mostrarVentasArticulos(std::ostream&);
private:
    std::map<Pedido*,ItemsPedido,OrdenaPedidos> pedido_articulo_;
    std::map<Articulo*,Pedidos,OrdenaArticulos> articulo_pedido_;
};

/* Operadores de flujo */
std::ostream& operator << (std::ostream& os, const Pedido_Articulo::ItemsPedido& items);
std::ostream& operator << (std::ostream& os, const Pedido_Articulo::Pedidos& pedidos);
#endif
