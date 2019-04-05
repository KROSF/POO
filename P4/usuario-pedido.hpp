/**
 * @file usuario-pedido.hpp
 * @author Carlos Rodrigo Sanabria Flores
 * @date 12 May 2018
 * @copyright 2018 Carlos Rodrigo Sanabria Flores
 */
#ifndef USUARIO_PEDIDO_HPP
#define USUARIO_PEDIDO_HPP
#include <set>
#include "pedido.hpp"

class Usuario_Pedido {
 public:
  /* Dedinicion del tipo Pedidos*/
  using Pedidos = std::set<Pedido *>;
  /* Asociaciones */
  void asocia(Usuario &us, Pedido &pe) {
    usuario_pedidos_[&us].insert(&pe);
    pedido_usuario_[&pe] = &us;
  }
  void asocia(Pedido &pe, Usuario &us) { asocia(us, pe); }
  /* Observadores de la Asociacion*/
  Pedidos pedidos(Usuario &us) { return usuario_pedidos_[&us]; }

  Usuario *cliente(Pedido &pedido) { return pedido_usuario_[&pedido]; }

 private:
  std::map<Usuario *, Pedidos> usuario_pedidos_;
  std::map<Pedido *, Usuario *> pedido_usuario_;
};
#endif
