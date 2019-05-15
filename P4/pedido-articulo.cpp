/**
 * @file pedido-articulo.cpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 12 May 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
#include "pedido-articulo.hpp"
#include <iomanip>

LineaPedido::LineaPedido(double precio, unsigned cantidad)
    : precio_(precio), cantidad_(cantidad) {}

/* Objetos Operadores para la ordenacion*/

bool OrdenaArticulos::operator()(const Articulo *pa, const Articulo *sa) const {
  return pa->referencia() < sa->referencia();
}

bool OrdenaPedidos::operator()(const Pedido *pp, const Pedido *sp) const {
  return pp->numero() < sp->numero();
}

/* Asociaciones */
void Pedido_Articulo::pedir(Pedido &p, Articulo &art, double pre,
                            unsigned cant) {
  pedido_articulo_[&p].insert(std::make_pair(&art, LineaPedido(pre, cant)));
  articulo_pedido_[&art].insert(std::make_pair(&p, LineaPedido(pre, cant)));
}

void Pedido_Articulo::pedir(Articulo &art, Pedido &p, double pre,
                            unsigned cant) {
  Pedido_Articulo::pedir(p, art, pre, cant);
}

/* Observadores Asociaciones*/

Pedido_Articulo::ItemsPedido &Pedido_Articulo::detalle(Pedido &pe) {
  return pedido_articulo_[&pe];
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo &art) {
  return articulo_pedido_[&art];
}

/*Insercion en el flujo*/

std::ostream &Pedido_Articulo::mostrarDetallePedidos(std::ostream &os) {
  double tmp_total = 0.0;
  for (const auto &[pedido, articulos] : pedido_articulo_) {
    os << "Pedido núm. " << pedido->numero()
       << "\nCliente: " << pedido->tarjeta()->titular()->nombre()
       << "\tFecha: " << pedido->fecha() << articulos;
    tmp_total += pedido->total();
  }
  return os << std::fixed << "\nTOTAL VENTAS:\t" << std::setprecision(2)
            << tmp_total << " €" << std::endl;
}

std::ostream &Pedido_Articulo::mostrarVentasArticulos(std::ostream &os) {
  // typeof articulos = Articulo*
  // typeof pedidos = map(Pedido*,LineaPedido)
  for (const auto &[articulos, pedidos] : articulo_pedido_) {
    os << "Ventas de [" << articulos->referencia() << "] \""
       << articulos->titulo() << "\"\n"
       << pedidos << std::endl;
  }
  return os;
}

/* Operadores de flujo*/

std::ostream &operator<<(std::ostream &os, const LineaPedido &li_pe) {
  return os << std::fixed << std::setprecision(2) << li_pe.precio_venta()
            << " €\t" << li_pe.cantidad();
}

std::ostream &operator<<(std::ostream &os,
                         const Pedido_Articulo::ItemsPedido &items) {
  double tmp_total = 0.0;
  os << '\n'
     << Cadena(80, '=') << std::endl
     << "  PVP\tCantidad\tArtículo\n"
     << Cadena(80, '=') << std::endl;
  // typeof articulo = Articulo*
  // typeof linea = LineaPedido
  for (const auto &[articulo, linea] : items) {
    os << linea << "\t\t[" << articulo->referencia() << "] \""
       << articulo->titulo() << "\"" << std::endl;
    tmp_total += linea.precio_venta() * linea.cantidad();
  }
  os << Cadena(80, '=') << std::endl
     << "Total\t" << std::fixed << std::setprecision(2) << tmp_total << " €\n"
     << std::endl;
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const Pedido_Articulo::Pedidos &pedidos) {
  unsigned tmp_cantidad = 0;
  double tmp_total = 0.0;
  os << "[Pedidos: " << pedidos.size() << "]\n"
     << Cadena(80, '=') << std::endl
     << "  PVP\tCantidad\tFecha de venta\n"
     << Cadena(80, '=') << std::endl;
  // typeof pedido = Pedido*
  // typeof linea = LineaPedido
  for (const auto &[pedido, linea] : pedidos) {
    os << linea << "\t\t" << pedido->fecha() << std::endl;
    tmp_total += linea.precio_venta() * linea.cantidad();
    tmp_cantidad += linea.cantidad();
  }
  os << Cadena(80, '=') << std::endl
     << std::fixed << std::setprecision(2) << tmp_total << " €\t"
     << tmp_cantidad << std::endl;
  return os;
}
