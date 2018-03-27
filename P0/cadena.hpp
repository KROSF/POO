#ifndef CADENA_H
#define CADENA_H
#include <iostream>
class Cadena {
public:
    //Constructores
    explicit Cadena(size_t t = 0, char c=' ');//
    Cadena( const char * cad );
    //Constructor de copia
    Cadena(const Cadena& cad);
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
    //operador de conversion
    operator const char * () const noexcept;
    const char * c_str() const;
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

//ostream& operator << (std::ostream& os, const Cadena& cad);
#endif
