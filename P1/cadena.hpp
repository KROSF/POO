#ifndef CADENA_H
#define CADENA_H
#include <iostream>
using namespace std;
class Cadena {
public:
    //Constructores
    explicit Cadena(size_t t = 0, char c = ' ');//
    Cadena( const char * cad );
    //Constructor de copia
    Cadena(const Cadena& cad);
    //Constructor de movimiento
    Cadena(Cadena&& cad);
    //destructor
    ~Cadena();
    //metodo para obtener una subcadena
    Cadena substr(size_t begindex, size_t len) const;
    //metodos observadores
    size_t length() const noexcept;
    //operadores indice que no producen exepciones
    char operator [] (size_t j) const noexcept;
    char& operator [] (size_t j) noexcept;
    //operadores indice que producen exepciones
    char at(size_t i) const;
    char& at(size_t i);
    //operadores de asignacion
    Cadena& operator += (const Cadena& cad);
    Cadena& operator = (const Cadena& cad);
    Cadena& operator = (const char * cad);
    Cadena& operator = ( Cadena&& cad );
    //operador de conversion
    const char * c_str() const;
    //iteradores
    typedef char* iterator;
	  typedef char const* const_iterator;
	  typedef std::reverse_iterator<iterator> reverse_iterator;
	  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin() noexcept {return s_;}
	  const_iterator cbegin() const noexcept { return s_; }
   	const_iterator begin() const noexcept{ return cbegin(); }

	  iterator end()noexcept { return s_ + tam_; }
	  const_iterator cend() const noexcept { return s_ + tam_; }
	  const_iterator end() const noexcept{ return cend(); }

	  const_reverse_iterator crbegin() const noexcept
	  { return const_reverse_iterator(cend()); }

	  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
	  const_reverse_iterator rbegin() const noexcept { return crbegin(); }

	  const_reverse_iterator crend() const noexcept
	  { return const_reverse_iterator(cbegin()); }

	  reverse_iterator rend()noexcept { return reverse_iterator(begin()); }
	  const_reverse_iterator rend() const noexcept{ return crend(); }
private:
    //Atributos de la clase Cadena
    char * s_;
    size_t tam_;
    Cadena(const Cadena&,size_t begindex,size_t endindex);
};

Cadena operator + (const Cadena& cad1, const Cadena& cad2);
//operadores de comparacion
bool operator == (const Cadena& cad1, const Cadena& cad2) noexcept;
bool operator != (const Cadena& cad1, const Cadena& cad2) noexcept;

bool operator > (const Cadena& cad1, const Cadena& cad2) noexcept;
bool operator < (const Cadena& cad1, const Cadena& cad2) noexcept;

bool operator <= (const Cadena& cad1, const Cadena& cad2) noexcept;
bool operator >= (const Cadena& cad1, const Cadena& cad2) noexcept;

ostream& operator << (std::ostream& os, const Cadena& cad);
istream& operator >> (std::istream& is, Cadena& cad);

#endif
