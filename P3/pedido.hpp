#ifndef PEDIDO_HPP
#define PEDIDO_HPP
#include "../P1/fecha.hpp"
#include "tarjeta.hpp"
class Usuario_Pedido;
class Pedido_Articulo;
class Pedido{
public:
  Pedido(Usuario_Pedido& u_p,Pedido_Articulo& p_a,Usuario& u,Tarjeta& t,Fecha today = Fecha());
  /* Exepcion */
  class Vacio{
  public:
    Vacio(Usuario const* u) : empty_(u) {}
    const Usuario & usuario() const noexcept { return *empty_ ; }
  private:
    Usuario const* empty_;
  };

  class Impostor{
  public:
    Impostor(Usuario const* u) : impostor_(u) {}
    const Usuario& usuario() const noexcept { return *impostor_; }
  private:
    Usuario const* impostor_;
  };

  class SinStock{
  public:
    SinStock(Articulo const* a) : sinstock_(a) {}
    const Articulo& articulo() const noexcept { return *sinstock_; }
  private:
    Articulo const* sinstock_;
  };

  /* Observadores */
  size_t numero() const noexcept { return last_; }
  Tarjeta const* tarjeta() const noexcept{ return card_; }
  Fecha fecha() const noexcept { return date_; }
  double total() const noexcept { return total_; }
  size_t n_total_pedidos() noexcept { return num_; }
private:
  size_t num_;
  Tarjeta const* card_;
  Fecha date_;
  double total_;
  size_t last_;
};














#endif
