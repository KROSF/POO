/**
 * @file pedido.cpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 12 May 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
#include "pedido.hpp"
#include <iomanip>
#include "pedido-articulo.hpp"
#include "usuario-pedido.hpp"
// Incializacion variable estatica
int Pedido::n_pedidos_ = 0;

// Constructor
Pedido::Pedido(Usuario_Pedido &u_p, Pedido_Articulo &p_a, Usuario &u,
               const Tarjeta &t, const Fecha &fecha)
    : num_(n_pedidos_ + 1), tarjeta_(&t), date_(fecha), total_(0) {
  if (u.n_articulos() == 0) throw Pedido::Vacio(&u);
  if (t.titular() != &u) throw Pedido::Impostor(&u);
  if (t.caducidad() < fecha) throw Tarjeta::Caducada(t.caducidad());
  Usuario::Articulos carro = u.compra();
  bool pedido_vacio = true;
  for (auto &[articulo, cantidad] : carro) {
    if (auto *libroDig = dynamic_cast<LibroDigital *>(articulo)) {
      if (libroDig->f_expir() < Fecha())
        u.compra(*articulo, 0);
      else {
        p_a.pedir(*this, *libroDig, articulo->precio(), cantidad);
        total_ += articulo->precio() * cantidad;
        u.compra(*articulo, 0);
        pedido_vacio = false;
      }
    } else if (auto *artAlmc = dynamic_cast<ArticuloAlmacenable *>(articulo)) {
      if (artAlmc->stock() < cantidad) {
        const_cast<Usuario::Articulos &>(u.compra()).clear();
        throw Pedido::SinStock(articulo);
      }
      artAlmc->stock() -= cantidad;
      p_a.pedir(*this, *artAlmc, articulo->precio(), cantidad);
      total_ += articulo->precio() * cantidad;
      u.compra(*articulo, 0);
      pedido_vacio = false;
    } else
      throw std::logic_error("Error, Articulo Desconocido");
  }
  if (pedido_vacio) throw Vacio(&u);
  u_p.asocia(u, *this);
  ++n_pedidos_;
}

// Metodo que devuelve el total de peidos del programa
int Pedido::n_total_pedidos() noexcept { return n_pedidos_; }

// Operador de Flujo
std::ostream &operator<<(std::ostream &os, const Pedido &pe) {
  return os << std::left << std::setw(13) << "Núm. pedido: " << pe.numero()
            << std::endl
            << std::left << std::setw(13) << "Fecha:" << pe.fecha() << std::endl
            << std::left << std::setw(13)
            << "Pagado con:" << pe.tarjeta()->tipo()
            << " n.º: " << pe.tarjeta()->numero() << std::endl
            << std::left << std::setw(13) << "Importe:" << std::fixed
            << std::setprecision(2) << pe.total() << " €" << std::endl;
}
