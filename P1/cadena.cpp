#include <cstring>
#include <stdexcept>
#include "cadena.hpp"

/* CONSTRUCTORES */

Cadena::Cadena(size_t tam, const char c) : s_(new char[tam+1]),tam_(tam)
{
  for (unsigned i = 0; i < tam; ++i) s_[i] = c;
  s_[tam_]='\0';
}

Cadena::Cadena(const char * cc) : s_(new char[strlen(cc)+1]),tam_(std::strlen(cc)) {std::strcpy(s_, cc);}

Cadena::Cadena(const Cadena &cad ) : s_(new char[cad.tam_+1]),tam_(cad.tam_) {std::strcpy(s_, cad.s_);}

Cadena::Cadena(Cadena&& cad ):s_(cad.s_),tam_(cad.tam_)
{
  cad.tam_ = 0;
  cad.s_= nullptr;
}

/* METODOS */

Cadena Cadena::substr(size_t begindex, size_t len) const
{
  if (begindex >= this->length() || begindex + len > this->length() || begindex + len < begindex)
      throw std::out_of_range("Error de rango");
  char* tmp = new char[len+1];
  std::strncpy(tmp, s_+begindex,len);
  tmp[len]='\0';
  Cadena ctmp(tmp);
  delete [] tmp;
  return ctmp;
}

inline size_t Cadena::length() const noexcept { return tam_; }

inline const char * Cadena::c_str() const noexcept{ return s_; }

/* OPERADORES  DE ACCESO */

char Cadena::operator [] (size_t i) const noexcept { return s_[i]; }

char& Cadena::operator [] (size_t i) noexcept { return s_[i]; }

char Cadena::at(size_t i) const
{
  if (i >= tam_) throw std::out_of_range("Fuera de rango");
  return s_[i];
}

char& Cadena::at(size_t i)
{
  if (i >= tam_) throw std::out_of_range("Fuera de rango");
  return s_[i];
}

/* OPERADORES DE ASIGNACIÓN */

Cadena& Cadena::operator += (const Cadena &cad)
{
  Cadena t = *this;
  tam_ = t.tam_+cad.tam_;
  delete [] s_;
  s_ = new char[tam_+1];
  std::strcpy(s_,t.s_);
  std::strcat(s_,cad.s_);
  return *this;
}

Cadena& Cadena::operator = (Cadena&& cad)
{
  if(this != &cad)
  {
    delete[] s_;
    tam_ = cad.tam_;
    s_= cad.s_;
    cad.s_ = nullptr;
    cad.tam_ = 0;
  }
    return *this;
}

Cadena& Cadena::operator = (const Cadena & cad )
{
  if(this != &cad)
  {
    delete [] s_;
    tam_=cad.tam_;
    s_=new char[tam_+1];
    std::strcpy(s_,cad.s_);
  }
  return *this;
}

Cadena& Cadena::operator = (const char* cad)
{
  delete[] s_;
  tam_ = std::strlen(cad);
  s_ = new char[tam_ + 1];
  std::strcpy(s_, cad);
  return *this;
}

/* OPERADOR ARIMETICO */

Cadena operator + (const Cadena& cad1, const Cadena& cad2) {return Cadena(cad1) += cad2;}

/* OPERADORES DE COMPARACIÓN */

bool operator == (const Cadena& cad1, const Cadena& cad2) noexcept
{
    return (std::strcmp(cad1.c_str(), cad2.c_str()) == 0);
}

bool operator != (const Cadena& cad1, const Cadena& cad2) noexcept { return !(cad1==cad2);}

bool operator > (const Cadena& cad1, const Cadena& cad2) noexcept
{ return (std::strcmp(cad1.c_str(),cad2.c_str()) > 0 ); }

bool operator < (const Cadena& cad1, const Cadena& cad2) noexcept
{ return !(cad1 == cad2) && !(cad1 > cad2);}

bool operator <= (const Cadena& cad1, const Cadena& cad2) noexcept { return !(cad1 > cad2);}

bool operator >= (const Cadena& cad1, const Cadena& cad2) noexcept
{ return (cad1 == cad2) || (cad1 > cad2); }

/* OPERADORES DE FLUJO */

std::ostream& operator << (std::ostream& os, const Cadena& cad) noexcept
{
	os << cad.c_str();
	return os;
}

std::istream &operator>>(std::istream& is,Cadena& cad) noexcept
{
  char* tmp = new char[33];
  is.width(33);
  is >> tmp;
  cad = tmp;
  delete [] tmp;
  return is;
}

/* DESTRUCTOR */

Cadena::~Cadena() { delete[] s_;}
