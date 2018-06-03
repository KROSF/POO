/**
* @file pedido.cpp
* @author Carlos Rodrigo Sanabria Flores
* @date 12 May 2018
* @copyright 2018 Carlos Rodrigo Sanabria Flores
*/
#include <iomanip>
#include "pedido.hpp"
#include "usuario-pedido.hpp"
#include "pedido-articulo.hpp"
// Incializacion variable estatica
int Pedido::n_pedidos_ = 0;

//Constructor
Pedido::Pedido(Usuario_Pedido& u_p,
               Pedido_Articulo& p_a,
               Usuario& u,
               const Tarjeta& t,
               const Fecha& fecha)
:num_(n_pedidos_+1),tarjeta_(&t),date_(fecha),total_(0)
{
    if( u.n_articulos() == 0) throw Pedido::Vacio(&u);
    if(t.titular() != &u) throw Pedido::Impostor(&u);
    if(t.caducidad() < fecha) throw Tarjeta::Caducada(t.caducidad());
    Usuario::Articulos carro = u.compra();
    bool pedido_vacio = true;
    // typeof c = pair<Articulo*,unsigned>
    // typeof c.first = Articulo*
    // typeof c.second = unsigned
    for(auto& c : carro)
    {
        if (auto* libroDig = dynamic_cast<LibroDigital*>(c.first))
        {
            if (libroDig->f_expir() < Fecha())
                u.compra(*c.first, 0);
            else
            {
                p_a.pedir(*this,*libroDig,c.first->precio(),c.second);
                total_+= c.first->precio() * c.second;
                u.compra(*c.first,0);
                pedido_vacio = false;
            }
        }
        else if(auto* artAlmc = dynamic_cast<ArticuloAlmacenable*>(c.first))
        {
            if(artAlmc->stock() < c.second )
            {
                const_cast<Usuario::Articulos&>(u.compra()).clear();
                throw Pedido::SinStock(c.first);
            }
            artAlmc->stock() -= c.second;
            p_a.pedir(*this,*artAlmc,c.first->precio(),c.second);
            total_+= c.first->precio() * c.second;
            u.compra(*c.first,0);
            pedido_vacio = false;
        }
        else throw std::logic_error("Error, Articulo Desconocido");
    }
    if (pedido_vacio) throw Vacio(&u);
    u_p.asocia(u,*this);
    ++n_pedidos_;
}

//Metodo que devuelve el total de peidos del programa
int Pedido::n_total_pedidos() noexcept
{
	return n_pedidos_;
}

//Operador de Flujo
std::ostream& operator << (std::ostream& os ,const Pedido& pe)
{
    return os
    << std::left << std::setw(13) << "Núm. pedido: " << pe.numero() << std::endl
    << std::left << std::setw(13) << "Fecha:" << pe.fecha() << std::endl
    << std::left << std::setw(13) << "Pagado con:" << pe.tarjeta()->tipo()
    <<" n.º: "<<pe.tarjeta()->numero()<< std::endl
    << std::left << std::setw(13) << "Importe:" << std::fixed
    << std::setprecision(2) << pe.total() << " €"<< std::endl;
}
