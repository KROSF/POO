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
    // typeof c = pair<Articulo*,unsigned>
    // typeof c.first = Articulo*
    // typeof c.second = unsigned
    std::set<Articulo*> expirados;
    for(auto c : u.compra())
    {
        if (auto* libroDig = dynamic_cast<LibroDigital*>(c.first))
        {
            if (libroDig->f_expir() < Fecha())
                expirados.insert(c.first);
        }
        else if(dynamic_cast<ArticuloAlmacenable*>(c.first)->stock() < c.second)
        {
            const_cast<Usuario::Articulos&>(u.compra()).clear();
            throw Pedido::SinStock(c.first);
        }
    }

    for (auto art : expirados)
            u.compra(*art, 0);

    if (!u.compra().size()) throw Vacio(&u);

    Usuario::Articulos carro = u.compra();

    for(auto c : carro)
    {
        Articulo* pa = c.first;
        unsigned int cantidad = c.second;
        double precio = pa->precio();
        if(auto* Artalm = dynamic_cast<ArticuloAlmacenable*>(pa))
            Artalm->stock() -= cantidad;
        p_a.pedir(*this,*pa,precio,cantidad);
        total_+= precio * cantidad;
        u.compra(*pa,0);
    }
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
    << std::setprecision(2) << pe.total() << " \u20AC"<< std::endl;
}
