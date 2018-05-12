/**
* @file pedido.hpp
* @author Carlos Rodrigo Sanabria Flores
* @date 12 May 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#ifndef PEDIDO_HPP
#define PEDIDO_HPP
#include <ostream>
#include "../P1/fecha.hpp"
#include "tarjeta.hpp"

class Usuario_Pedido;
class Pedido_Articulo;

class Pedido{
public:
    Pedido(Usuario_Pedido& u_p,
        Pedido_Articulo& p_a,
        Usuario& u,
        const Tarjeta& t,
        const Fecha& fecha = Fecha());
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
    int numero() const noexcept { return num_; }
    Tarjeta const* tarjeta() const noexcept{ return tarjeta_; }
    Fecha fecha() const noexcept { return date_; }
    double total() const noexcept { return total_; }
    static int n_total_pedidos() noexcept;
private:
    int num_;
    const Tarjeta* tarjeta_;
    Fecha date_;
    double total_;
    static int n_pedidos_;
};
std::ostream& operator << (std::ostream& os,const Pedido& pe);
#endif
