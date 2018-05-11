#ifndef PEDIDO_ARTICULO_HPP
#define PEDIDO_ARTICULO_HPP
#include <ostream>
#include "pedido.hpp"
class LineaPedido{
public:
    explicit LineaPedido(double precio, unsigned cantidad = 1);
    double precio_venta(void) const {return precio_;}
    unsigned cantidad(void) const {return cantidad_;}
private:
    double precio_;
    unsigned cantidad_;
};

std::ostream& operator <<(std::ostream& os, const LineaPedido& li_pe);

struct OrdenaPedidos: std::binary_function<Pedido*,Pedido*,bool>{
    bool operator()(const Pedido* pp,const Pedido* sp) const;
};

struct OrdenaArticulos: std::binary_function<Articulo*,Articulo*,bool>{
    bool operator()(const Articulo* pa,const Articulo* sa) const;
};

class Pedido_Articulo{
public:
    typedef std::map<Articulo*,LineaPedido,OrdenaArticulos> ItemsPedido;
    typedef std::map<Pedido*,LineaPedido,OrdenaPedidos> Pedidos;
    void pedir(Pedido& pedido, Articulo& articulo, double precio, unsigned cantidad = 1);
    void pedir(Articulo& articulo,Pedido& pedido, double precio, unsigned cantidad = 1);
    ItemsPedido& detalle(Pedido&);
	Pedidos ventas(Articulo&);
	std::ostream& mostrarDetallePedidos(std::ostream&);
	std::ostream& mostrarVentasArticulos(std::ostream&);
private:
    std::map<Pedido*,ItemsPedido,OrdenaPedidos> pedido_articulo_;
    std::map<Articulo*,Pedidos,OrdenaArticulos> articulo_pedido_;
};

std::ostream& operator << (std::ostream& os, const Pedido_Articulo::ItemsPedido& items);
std::ostream& operator << (std::ostream& os, const Pedido_Articulo::Pedidos& pedidos);
#endif
