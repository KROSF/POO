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

/* Operadores de ordenacion*/

bool OrdenaArticulos::operator()(const Articulo* pa,const Articulo* sa) const
{
    return pa->referencia() < sa->referencia();
}

bool OrdenaPedidos::operator()(const Pedido* pp,const Pedido* sp) const
{
    return pp->numero() < sp->numero();
}

/* Asociaciones */
void Pedido_Articulo::pedir(Pedido& p,Articulo& art,double pre, unsigned cant)
{
    pedido_articulo_[&p].insert(std::make_pair(&art,LineaPedido(pre,cant)));
    articulo_pedido_[&art].insert(std::make_pair(&p,LineaPedido(pre,cant)));
}

void Pedido_Articulo::pedir(Articulo& art,Pedido& p, double pre, unsigned cant)
{
    Pedido_Articulo::pedir(p,art,pre,cant);
}

/* Observadores Asociaciones*/

Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& pe)
{
    return pedido_articulo_[&pe];
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& art)
{
    return articulo_pedido_[&art];
}

/*Insercion en el flujo*/

std::ostream& Pedido_Articulo::mostrarDetallePedidos(std::ostream& os)
{
    double tmp_total = 0.0;
    // typeof obj = pair(Pedido*,map(Articulo*,LineaPedido))
    // typeof obj.first = Pedido*
    // typeof obj.second = map(Articulo*,LineaPedido)
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
    // typeof obj = pair(Articulo*,map(Pedido*,LineaPedido))
    // typeof obj.first = Articulo*
    // typeof obj.second = map(Pedido*,LineaPedido)
    for(auto obj : articulo_pedido_)
    {
        os << "Ventas de [" << obj.first->referencia() << "] \""
           << obj.first->titulo()<< "\"\n"
           << obj.second << std::endl;
    }
    return os;
}

/* Operadores de flujo*/

std::ostream& operator << (std::ostream& os, const LineaPedido& li_pe)
{
    return os << std::fixed << std::setprecision(2) << li_pe.precio_venta()
              << " \u20AC\t" << li_pe.cantidad();
}

std::ostream& operator<<(std::ostream& os,const Pedido_Articulo::ItemsPedido& items)
{
    double tmp_total = 0.0;
    os <<'\n'<<Cadena(80,'=') << std::endl
       << "  PVP\tCantidad\tArtículo\n"
       << Cadena(80,'=') << std::endl;
    // typeof i = std::pair(Articulo*,LineaPedido)
    // typeof i.first = Articulo*
    // typeof i.second = LineaPedido
    for(auto i : items)
    {
        os << i.second << "\t\t["<< i.first->referencia() << "] \""
           << i.first->titulo() << "\"" << std::endl;
        tmp_total += i.second.precio_venta() * i.second.cantidad();
    }
    os << Cadena(80,'=') << std::endl
       << "Total\t" << std::fixed<< std::setprecision(2)
       << tmp_total << " \u20AC\n" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::Pedidos& pedidos)
{
    unsigned tmp_cantidad = 0;
    double tmp_total = 0.0;
    os << "[Pedidos: " << pedidos.size() << "]\n"
       << Cadena(80,'=') << std::endl
       << "  PVP\tCantidad\tFecha de venta\n"
       << Cadena(80,'=') << std::endl;
    // typeof p = std::pair(Pedido*,LineaPedido)
    // typeof p.first = Pedido*
    // typeof p.second = LineaPedido
    for (auto p : pedidos)
    {
        os << p.second << "\t\t" << p.first->fecha() << std::endl;
        tmp_total  += p.second.precio_venta() * p.second.cantidad();
        tmp_cantidad += p.second.cantidad();
    }
    os << Cadena(80,'=') << std::endl << std::fixed
    << std::setprecision(2) << tmp_total << " \u20AC\t" << tmp_cantidad <<std::endl;
    return os;
}
