#include "pedido.hpp"
size_t Pedido::last_ = 0;
Pedido::Pedido(Usuario_Pedido& u_p,
               Pedido_Articulo& p_a,
               Usuario& u,
               const Tarjeta& t,
               Fecha today)
:num_(last_+1),card_(&t),date_(today),total_(0)
{
    if( u.n_articulos() == 0) throw Pedido::Vacio(&u);
    if(t.titular() != &u) throw Pedido::Impostor(&u);
    if(t.caducidad() < today) throw Tarjeta::Caducada(t.caducidad());
    for(auto c : u.compra())//c = pair<Articulo*,unsigned>(cantidad)
        if(c.first->stock()<c.second)
        {   //no hay articulos.
            const_cast<Usuario::Articulos&>(u.compra()).clear();
            throw Pedido::SinStock(c.first);
        }
    Usuario::Articulos carro = u.compra();
    for(auto c : carro)
    {
        Articulo* pa = c.first;
        unsigned int cantidad = c.second;
        double precio = pa->precio();
        pa->stock() -= cantidad;
        p_a.pedir(*this,*pa,precio,cantidad);
        total_+=total_*cantidad;
        u.compra(*pa,0);

    }
    u_p.asocia(u,*this);
    ++last_;    
}
