/**
* @file pedido-articulo.cpp
* @author Carlos Rodrigo Sanabria Flores
* @date 12 May 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#include <iomanip>
#include "pedido-articulo.hpp"

LineaPedido::LineaPedido(double precio,unsigned cantidad):
precio_(precio),cantidad_(cantidad){}

void Pedido_Articulo::pedir(Pedido& p, Articulo& art, double precio, unsigned cant)
{
    pedido_articulo_[&p].insert(std::make_pair(&art,LineaPedido(precio,cant)));
    articulo_pedido_[&art].insert(std::make_pair(&p,LineaPedido(precio,cant)));
}

void Pedido_Articulo::pedir(Articulo& art,Pedido& p, double precio, unsigned cant)
{
    Pedido_Articulo::pedir(p,art,precio,cant);
}

bool OrdenaArticulos::operator()(const Articulo* pa,const Articulo* sa) const
{
    return pa->referencia() < sa->referencia();
}

bool OrdenaPedidos::operator()(const Pedido* pp,const Pedido* sp) const
{
    return pp->numero() < sp->numero();
}

Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& pe)
{
    return pedido_articulo_[&pe];
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& art)
{
    return articulo_pedido_[&art];
}

std::ostream& Pedido_Articulo::mostrarDetallePedidos(std::ostream& os)
{
    // typeof obj = std::map<Pedido*,ItemsPedido,OrdenaPedidos>
    double tmp_total = 0.0;
    for(auto obj : pedido_articulo_)
    {
        os << "Pedido núm. " << obj.first->numero()
           << "\nCliente: " << obj.first->tarjeta()->titular()->nombre()
           << "\tFecha: " << obj.first->fecha() << obj.second;
        tmp_total += obj.first->total();
    }
    return os << std::fixed << "\nTOTAL VENTAS:\t"
              << std::setprecision(2) << tmp_total << " \u20AC" << std::endl;
}

std::ostream& Pedido_Articulo::mostrarVentasArticulos(std::ostream& os)
{
    // typeof obj =
    for(auto obj : articulo_pedido_)
    {
        os << "Pedidos: [" << obj.second.size() << "]\n"
           << obj.second << std::endl;
    }
    return os;
}

std::ostream& operator << (std::ostream& os, const LineaPedido& li_pe)
{
    return os << std::fixed << std::setprecision(2) << li_pe.precio_venta() << " \u20AC\t"
              << li_pe.cantidad();
}

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::ItemsPedido& items)
{
    double tmp_total = 0.0;
    os <<'\n'<<Cadena(66,'=') << std::endl
       << "  PVP\tCantidad\tArtículo\n"
       << Cadena(66,'=') << std::endl;
    for(auto i : items)
    {
        os << i.second.precio_venta() << "\u20AC\t"
           << i.second.cantidad() << "\t\t"
           << '[' << i.first->referencia() << "] \""
           << i.first->titulo() << "\"" << std::endl;

        tmp_total += i.second.precio_venta() * i.second.cantidad();
    }
    os << Cadena(66,'=') << std::endl
       << "Total\t" << std::fixed<< std::setprecision(2)
       << tmp_total << " \u20AC\n" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::Pedidos& pedidos)
{
    unsigned tmp_precio = 0;
    double tmp_total = 0.0;
    os << Cadena(66,'=') << std::endl
       << "  PVP\tCantidad\tFecha de venta\n"
       << Cadena(66,'=') << std::endl;
    for (auto p : pedidos)
    {
        os << p.second.precio_venta() << "€\t\t";
        os << p.second.cantidad() << "\t";
        os << p.first->fecha() << std::endl;
        tmp_total  += p.second.precio_venta() * p.second.cantidad();
        tmp_precio += p.second.cantidad();
    }
    os << Cadena(66,'=') << std::endl << std::fixed
    << std::setprecision(2) << tmp_total << " \u20AC\t" << tmp_precio <<std::endl;
    return os;
}
